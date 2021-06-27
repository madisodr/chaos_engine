#include <FastLED.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#include "consts.h"

MPU6050 mpu;
CRGB leds[NUM_LEDS];

int last_value_itr = 0;
int last_values[N_VALUES_TO_CHECK];
bool curr_status = ACCEL_STATUS_STOPPED;
bool last_status = ACCEL_STATUS_STOPPED;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion quat;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;    // [x, y, z]            world-frame accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

volatile bool mpuInterrupt = false;
void dmpDataReady()
{
    mpuInterrupt = true;
}

void check_status()
{

    if (last_value_itr >= N_VALUES_TO_CHECK) {
        last_value_itr = 0;
    }

    last_values[last_value_itr] = curr_status;
    last_value_itr++;

    int count = 0;
    if (last_status == ACCEL_STATUS_STOPPED) {
        for (uint16_t i = 0; i < N_VALUES_TO_STORE; i++) {
            if (last_values[i] == ACCEL_STATUS_MOVING) {
                count++;
            }
        }
        if (count >= N_VALUES_TO_CHECK) {
            Serial.println("MOVING");
            last_status = ACCEL_STATUS_MOVING;
        }
    } else if (last_status == ACCEL_STATUS_MOVING) {
        for (uint16_t i = 0; i < N_VALUES_TO_STORE; i++) {
            if (last_values[i] == ACCEL_STATUS_STOPPED) {
                count++;
            }
        }
        if (count >= N_VALUES_TO_CHECK) {
            Serial.println("STOPPING");
            last_status = ACCEL_STATUS_STOPPED;
        }
    }
}

void setup()
{

#ifdef OUTPUT_DEBUG_DATA
    Serial.begin(57600);
#endif

    for (int i = 0; i < N_VALUES_TO_STORE; i++) {
        last_values[i] = 0;
    }

    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN_A, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<LED_TYPE, LED_PIN_B, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);

    currentPalette = myRedPalette_p;
    currentBlending = LINEARBLEND;

    // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif


    // initialize device

#ifdef OUTPUT_DEBUG_DATA
    Serial.println(F("Initializing devices..."));
#endif

    mpu.initialize();

#ifdef OUTPUT_DEBUG_DATA
    Serial.println(mpu.testConnection() ? F("MPU6050: successful") : F("MPU6050: failed"));
#endif

    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(88);
    mpu.setYGyroOffset(-57);
    mpu.setZGyroOffset(-6);
    mpu.setXAccelOffset(-3502);
    mpu.setYAccelOffset(519);
    mpu.setZAccelOffset(1595);

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    }

}

void accelerometer_handler()
{
    // if programming failed, don't try to do anything
    if (!dmpReady) { return; }

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        //
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        curr_status = ACCEL_STATUS_STOPPED;
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) {
            fifoCount = mpu.getFIFOCount();
        }

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        // track FIFO count here in case there is > 1 packet available
        fifoCount -= packetSize;

#ifdef OUTPUT_READABLE_EULER
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetEuler(euler, &q);
        Serial.print("euler\t");
        Serial.print(euler[0] * 180 / M_PI);
        Serial.print("\t");
        Serial.print(euler[1] * 180 / M_PI);
        Serial.print("\t");
        Serial.println(euler[2] * 180 / M_PI);
#endif

#ifdef OUTPUT_READABLE_YAWPITCHROLL
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        Serial.print("ypr\t");
        Serial.print(ypr[0] * 180 / M_PI);
        Serial.print("\t");
        Serial.print(ypr[1] * 180 / M_PI);
        Serial.print("\t");
        Serial.println(ypr[2] * 180 / M_PI);
#endif
        mpu.dmpGetQuaternion(&quat, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &quat);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &quat);

#ifdef OUTPUT_DEBUG_DATA_WORLD
        accelerometer_output("world", aaWorld);
#endif

        if (
                abs(aaWorld.x) > MOVING_THRESHOLD ||
                abs(aaWorld.y) > MOVING_THRESHOLD ||
                abs(aaWorld.z) > MOVING_THRESHOLD
           ) {
            curr_status = ACCEL_STATUS_MOVING;
        } else {
            curr_status = ACCEL_STATUS_STOPPED;
        }

        return;
    }
}

#ifdef OUTPUT_DEBUG_DATA
void accelerometer_output(const char* val, VectorInt16 accel)
{
    Serial.print(val);
    Serial.print("\t");
    Serial.print(accel.x);
    Serial.print("\t");
    Serial.print(accel.y);
    Serial.print("\t");
    Serial.println(accel.z);
}
#endif

void loop()
{
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    accelerometer_handler();
    check_status();

    switch (last_status) {
        case ACCEL_STATUS_STOPPED:
            currentPalette = myRedPalette_p;
            fill_leds_from_palette(startIndex);
            digitalWrite(LED_BUILTIN, LOW);
            break;

        case ACCEL_STATUS_MOVING:
            currentPalette = RainbowStripeColors_p;
            fill_leds_from_palette(startIndex);
            digitalWrite(LED_BUILTIN, HIGH);
            break;
    }

    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void fill_leds_from_palette(uint8_t colorIndex)
{
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, currentBlending);
        colorIndex += 3;
    }
}

