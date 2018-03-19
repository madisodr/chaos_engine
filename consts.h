/*** GENERAL ***/
#define UPDATES_PER_SECOND 50

/*** LED ***/

// default led pin for arduino boards
#define LED_PIN_A 5
#define LED_PIN_B 6

// number of leds in the strip to control
#define NUM_LEDS 15

// default brightness for leds
#define BRIGHTNESS 255

// led type for specs

#define LED_TYPE WS2811

// color order
#define COLOR_ORDER GRB

/**** MPU6050 ****/
//#define OUTPUT_READABLE_EULER
//#define OUTPUT_READABLE_YAWPITCHROLL
//#define OUTPUT_DEBUG_DATA_WORLD
//#define OUTPUT_DEBUG_DATA

// how many state values to maintain. This is for smoothing out the transition between states so its less jittery
// or prone to error.
#define N_VALUES_TO_STORE 30
#define N_VALUES_TO_CHECK 20

// what speed should the accelerometer be considered "moving" at.
#define MOVING_THRESHOLD 500

// state of the acceleometer
enum ACC_STATE {
    ACCEL_STATUS_STOPPED = 0,
    ACCEL_STATUS_MOVING = 1
};

extern CRGBPalette16 myRedPalette;
extern const TProgmemPalette16 myRedPalette_p PROGMEM;

const TProgmemPalette16 myRedPalette_p PROGMEM =
{
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::Red,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::Red
};
