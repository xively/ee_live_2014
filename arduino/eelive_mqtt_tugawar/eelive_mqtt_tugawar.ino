/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>


#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, PIN, NEO_GRB + NEO_KHZ800);
int lwait = 0;

// Update these with values suitable for your network.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE2 };
byte server[] = { 192, 168, 214, 161 };
byte ip[]     = { 192, 168, 1, 10 };
//String stringone = "api.xively.com";

int blueTotal = 50;
int redTotal = 50;

void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.println(topic);
  //convert byte to char
  payload[length] = '\0';
  String strPayload = String((char*)payload);
  
  Serial.println(strPayload);
  
  //int teamint = strPayload.toInt();
  //Serial.println(strPayload);
  
  parseVote(strPayload);
  
  
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup()
{
  Serial.begin(19200);
  Serial.println("==STARTING==");

   // Start up the LED strip
  strip.begin();
    //clear strip for startup
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);//off  
  strip.show();
  delay(250);
  colorWipe(strip.Color(  255, 0, 0), lwait, 0, redTotal);
  colorWipe(strip.Color(  0, 0, 255), lwait, (100-blueTotal), 100);
  
  Ethernet.begin(mac);
  // start the Ethernet connection:
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
//    // no point in carrying on, so do nothing forevermore:
//    // try to congifure using IP address instead of DHCP:
//    Ethernet.begin(mac, ip);
//  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
  
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  
  //delay(500);
  boolean con = client.connect("arduinoMQTT");
  while(con != 1){
    Serial.println("no con-while");
    con = client.connect("arduinoMQTT"); 
    delay(1000);
  }
  //Serial.println(con);
  if(con){
    Serial.println("got con");
    //client.publish("outTopic","hello world");
    client.subscribe("vote");
  }else Serial.println("no con");
  
}

void loop()
{
  client.loop();
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

void parseVote(String vote){
    //Serial.println("parsing a vote");
    if(vote.equals("red")){
      if(redTotal < 100){
        redTotal = redTotal + 1;
        blueTotal = blueTotal - 1;
      }else{
         redTotal = 100;
      }
    }
    if(vote.equals("blue")){
       if(blueTotal < 100){
         blueTotal = blueTotal + 1;
         redTotal = redTotal - 1;
       }else{
         blueTotal = 100;
       }
    }
    Serial.print("Red=");
    Serial.println(redTotal);
    Serial.print("Blue=");
    Serial.println(blueTotal);
    
    colorWipe(strip.Color(  255, 0, 0), lwait, 0, redTotal);
    colorWipe(strip.Color(  0, 0, 255), lwait, (100-blueTotal), 100);
  
}


