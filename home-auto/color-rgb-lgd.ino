#include "rgb_led.h"

// http://docs.spark.io/assets/images/spark-pinout.png
#define RED_1 10
#define GREEN_1 11
#define BLUE_1 16

#define EEPROM_RED_1 1
#define EEPROM_GREEN_1 2
#define EEPROM_BLUE_1 3
#define EEPROM_BRIGHT_1 4

AntiLogLed::RgbLed RGBControl_1(RED_1, GREEN_1, BLUE_1);
bool doLoop = false;
int swirlSpeed = 100;

void setup() {
    pinMode(RED_1, OUTPUT);
    pinMode(GREEN_1, OUTPUT);
    pinMode(BLUE_1, OUTPUT);

    loadEEPROM();
    Spark.function("setcolor", setColor);
    Spark.function("setbright", setBrightness);
    Spark.function("doloop", doLoopChange);    
}

void loop() {
    int r, g, b;
    if (doLoop) {
        // fade from blue to violet
        for (r = 0; r < 256; r++) { 
            analogWrite(RED_1, r);
            delay(swirlSpeed);
        } 
        // fade from violet to red
        for (b = 255; b > 0; b--) { 
            analogWrite(BLUE_1, b);
            delay(swirlSpeed);
        } 
        // fade from red to yellow
        for (g = 0; g < 256; g++) { 
            analogWrite(GREEN_1, g);
            delay(swirlSpeed);
        } 
        // fade from yellow to green
        for (r = 255; r > 0; r--) { 
            analogWrite(RED_1, r);
            delay(swirlSpeed);
        } 
        // fade from green to teal
        for (b = 0; b < 256; b++) { 
            analogWrite(BLUE_1, b);
            delay(swirlSpeed);
        } 
        // fade from teal to blue
        for (g = 255; g > 0; g--) { 
            analogWrite(GREEN_1, g);
            delay(swirlSpeed);
        } 
    }
}

int setColor(String args) {
    //red,green,blue
    //"255,255,255"
    int i = args.indexOf(',');
    int red = args.substring(0, i).toInt();
    int j = args.lastIndexOf(',');
    int green = args.substring(i + 1, j).toInt();
    int blue = args.substring(j + 1).toInt();
    RGBControl_1.color(red, green, blue);
    saveColors(red, green, blue);
    return blue;
}

int setBrightness(String args) {
    int brightValue = args.toInt();
    RGBControl_1.brightness(brightValue);
    EEPROM.write(EEPROM_BRIGHT_1, brightValue);
    return brightValue;    
}

int doLoopChange(String args) {
    int i = args.toInt();
    swirlSpeed = i;
    if (i > 0) { 
        doLoop = true;
    } else {
        doLoop = false;
        loadEEPROM();   
    }
    return i;
}

void loadEEPROM() {
    uint8_t redValue = EEPROM.read(EEPROM_RED_1);
    uint8_t greenValue = EEPROM.read(EEPROM_GREEN_1);
    uint8_t blueValue = EEPROM.read(EEPROM_BLUE_1);
    RGBControl_1.color(redValue, greenValue, blueValue); 
    
    uint8_t brightValue = EEPROM.read(EEPROM_BRIGHT_1);
    RGBControl_1.brightness(brightValue); 
}

void saveColors(int redValue, int greenValue, int blueValue) {
    EEPROM.write(EEPROM_RED_1, redValue);
    EEPROM.write(EEPROM_GREEN_1, greenValue);
    EEPROM.write(EEPROM_BLUE_1, blueValue);
}