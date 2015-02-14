#include "Adafruit_MCP23017/Adafruit_MCP23017.h"
#include "MQTT/MQTT.h"

byte serverip[] = {192, 168, 1, 181};
void callback(char* topic, byte* payload, unsigned int length);
MQTT client(serverip, 1883, callback);

Adafruit_MCP23017 mcp;

byte ledPin=D7;
//byte SparkIntPin=D3;    // Interrupts from the MCP will be handled by this PIN

char str[42];
//volatile boolean interrupted=false;
//volatile uint8_t flashes=0;
volatile uint16_t pinab=65535;

#define flag1 1
#define flag2 2
#define flag3 4
#define flag4 8
#define flag5 16
#define flag6 32 
#define flag7 64
#define flag8 128
#define flag9 256
#define flag10 512
#define flag11 1024
#define flag12 2048
#define flag13 4096
#define flag14 8192
#define flag15 16384
#define flag16 32768

void setup() {
    RGB.control(true);
    
    // connect to the server
    client.connect("mirchiclient");

    // publish/subscribe
    if (client.isConnected()) {
        client.publish("outTopic","mirchi is online");
        client.subscribe("inTopic");
        RGB.color(0, 255, 0);
    } else {
        RGB.color(255, 0, 0);
    }
    
    //Serial.begin(9600);
    //Serial.println("MCP23017 test application");

    //pinMode(SparkIntPin,INPUT);
    pinMode(ledPin,OUTPUT);     // use the LED if any MCP pin is active
    mcp.begin();                // use default address 0

    // We mirror INTA and INTB, so that only one line is required between MCP and Arduino for int reporting
    // The INTA/B will not be Floating
    // INTs will be signaled with a LOW
    //mcp.setupInterrupts(true,false,LOW);

    // Configure port-A (pin 0-7), port-B (8-15)
    for(int pin=0; pin<16; ++pin) {
        mcp.pinMode(pin,INPUT);
        mcp.pullUp(pin,HIGH);  // turn on a 100K pullup internally
        //mcp.setupInterruptPin(pin,CHANGE);
    }

    // Spark Interupt
    //attachInterrupt(SparkIntPin,handleInterrupt,CHANGE);
    pinab = mcp.readGPIOAB();
}

void loop() {
    if (client.isConnected()) {
        client.loop();
    }
    //if (interrupted) {
        uint16_t tmp_pinab=mcp.readGPIOAB();
        if(pinab != tmp_pinab && pinab < 65536) {
            pinab = tmp_pinab;
            //Serial.println(pinab,BIN);
            sprintf(str,"%d ",pinab);
            if (pinab & flag1) strcat(str, "1 ");
            if (pinab & flag2) strcat(str, "2 ");
            if (pinab & flag3) strcat(str, "3 ");
            if (pinab & flag4) strcat(str, "4 ");
            if (pinab & flag5) strcat(str, "5 ");
            if (pinab & flag6) strcat(str, "6 ");
            if (pinab & flag7) strcat(str, "7 ");
            if (pinab & flag8) strcat(str, "8 ");
            if (pinab & flag9) strcat(str, "9 ");
            if (pinab & flag10) strcat(str, "10 ");
            if (pinab & flag11) strcat(str, "11 ");
            if (pinab & flag12) strcat(str, "12 ");
            if (pinab & flag13) strcat(str, "13 ");
            if (pinab & flag14) strcat(str, "14 ");
            if (pinab & flag15) strcat(str, "15 ");
            if (pinab & flag16) strcat(str, "16 ");
            if (client.isConnected()) {
                client.publish("outTopic",str);
                RGB.color(0, 255, 0);
            } else {
                RGB.color(255, 0, 0);
            }
            //Serial.println(str);
            //flashes=1;
        }
        if(pinab == 65535) {
            digitalWrite(ledPin,LOW);    
        } else {
            digitalWrite(ledPin,HIGH);
        }
        //interrupted = false;  // reset in all cases
    //}
    /*
    if (flashes > 0) {
        delay(100);
        digitalWrite(ledPin,HIGH);
        delay(100);
        digitalWrite(ledPin,LOW);
        flashes--;
    }*/
}

//void handleInterrupt() {
//    interrupted = true;
//}

// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);

    if (message.equals("RED"))    
        RGB.color(255, 0, 0);
    else if (message.equals("GREEN"))    
        RGB.color(0, 255, 0);
    else if (message.equals("BLUE"))    
        RGB.color(0, 0, 255);
    else    
        RGB.color(255, 255, 255);
    delay(1000);
}