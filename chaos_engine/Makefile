PROJECT_DIR       = /Users/tetra/led-projects/burner_coat

### ARDMK_DIR
# REQUIRES https://github.com/sudar/Arduino-Makefile to be downloaded
ARDMK_DIR         = /Users/tetra/Arduino-Makefile

### ARDUINO_DIR
ARDUINO_DIR       = /Applications/Arduino.app/Contents/Java

### USER_LIB_PATH
### Path to where the your project's libraries are stored.
USER_LIB_PATH    :=  $(PROJECT_DIR)/lib

USER_LIB_PATH    :=  /Users/tetra/Documents/Arduino/libraries

### BOARD_TAG
### It must be set to the board you are currently using. (i.e uno, mega2560, etc.)
BOARD_TAG         = attiny85

### MONITOR_BAUDRATE
MONITOR_BAUDRATE  = 115200

### AVR_TOOLS_DIR
AVR_TOOLS_DIR     = $(ARDUINO_DIR)/hardware/tools/avr

### AVRDUDE
AVRDUDE          = /usr/local/bin/avrdude

### CFLAGS_STD
CFLAGS_STD        = -std=gnu11

### CXXFLAGS_STD
CXXFLAGS_STD      = -std=gnu++11

### CXXFLAGS
CXXFLAGS         += -pedantic -Wall -Wextra

### MONITOR_PORT
### The port your board is connected to. Using an '*' tries all the ports and finds the right one.
MONITOR_PORT := /dev/null
ISP_PROG := usbtiny
ISP_PORT          = /dev/ttyACM0
BOARD_TAG         = uno

### CURRENT_DIR
CURRENT_DIR       = $(shell basename $(CURDIR))

### OBJDIR
OBJDIR            = $(PROJECT_DIR)/bin/$(BOARD_TAG)/$(CURRENT_DIR)

### Do not touch - the path to Arduino.mk, inside the ARDMK_DIR
include $(ARDMK_DIR)/Arduino.mk
