/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

//led setup
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, PIN, NEO_GRB + NEO_KHZ800);
int lwait = 0;

//serial setup
SoftwareSerial impSerial(8, 9); // RX on 8, TX on 9
char inData[100]; 
char inChar=-1; 
byte index = 0; 
String input = "";
boolean newdata = false;

//var setup
int blueTotal = 50;
int redTotal = 50;

void setup()
{
  Serial.begin(19200);
  Serial.println("==STARTING==");
  
  impSerial.begin(19200);
  
   // Start up the LED strip
  strip.begin();
    //clear strip for startup
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);//off  
  strip.show();
  delay(250);
  colorWipe(strip.Color(  255, 0, 0), lwait, 0, redTotal);
  colorWipe(strip.Color(  0, 0, 255), lwait, (100-blueTotal), 100);
  
  
}

void loop(){
    if (impSerial.available() > 0){
      inChar = impSerial.read()
      Serial.println(inChar);
      parseVote(inChar);
    }
}

// custom wipe to Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait, int fills, int fille) {
  //for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);   //clear all leds
  //int i;

  for (int i=fills; i < strip.numPixels(); i++) {
    if(i < fille){
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
  }
  
}

void parseVote(char vote){
    //Serial.println("parsing a vote");
    switch (vote) {
      case \r:
        if (redTotal < 100) {
          redTotal = redTotal + 1;
          blueTotal = blueTotal - 1;
        } else {
           redTotal = 100;
        }
        break;
      case \b:
        if(blueTotal < 100) {
          blueTotal = blueTotal + 1;
          redTotal = redTotal - 1;
        } else {
          blueTotal = 100;
        }
        break;
    }
    Serial.print("Red=");
    Serial.println(redTotal);
    Serial.print("Blue=");
    Serial.println(blueTotal);

    colorWipe(strip.Color(  255, 0, 0), lwait, 0, redTotal);
    colorWipe(strip.Color(  0, 0, 255), lwait, (100-blueTotal), 100);
  
}


