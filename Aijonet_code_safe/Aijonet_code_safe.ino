/***************************************************
  This is our touchscreen painting example for the Adafruit ILI9341
  captouch shield
  ----> http://www.adafruit.com/products/1947

  Check out the links above for our tutorials and wiring diagrams

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

 
/* Aijonet Palliative Solution
 *  
 * This is a device created by Bond University's team: HackIT
 * 
 * This device is aimed to aide palliative care nurses by increasing the 
 * amount of communication between the hospital and at-home patients in 
 * an easy, intuitive way.
 * 
 * Adafruit's CapTouchPaint example for their 2.8" Capacative Touch 
 * LCD TFT display as well as Telstra's IoT libraries were used to make
 * this device.
 * 
 * A quick guide to this device:
 * the antennae are assumed to be the top of the device, the device 
 * waits for a user to interact with it before it starts asking questions
 * once all questions have been asked the device sends the information to 
 * Telstra's IoT platform.
 * 
 * Written by Lucas Brown for HackIT
 * MIT license, all text above must be included in any redistribution
 */

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>       // this is needed for display
#include <Adafruit_ILI9341.h>
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>
#include "OLED.h"
#include <telstraiot.h>
#include <TelstraWeb.h>
#include <connection4g.h>
#include <TelstraM1Device.h>
#include <TelstraM1Interface.h>
#include <mqttpackets.h>
// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ctp = Adafruit_FT6206();

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

OLED OLED;
String xyCord;
String response[] = {"null","null","null","null","null","null"};
void questions();

void setup(void) {
  while (!Serial);     // used for leonardo debugging
 
  Serial.begin(115200);
  Serial.println(F("Aijonet device activated"));
  Serial.println(" ");
  Serial.println(" ");
  
  tft.begin();

  if (! ctp.begin(40)) {  // pass in 'sensitivity' coefficient
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);
  }

  Serial.println("Screen Ready");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("waiting for user........");
  
  tft.fillScreen(ILI9341_BLACK);
  
  //Initialize the OLED controller
  OLED.begin();
  OLED.fill_OLED(0x00,0x00,0x00); // Clear screen
}

void loop() {
  response[0] = "null";  response[1] = "null";  response[2] = "null";
  response[3] = "null";  response[4] = "null";  response[5] = "null";
  // Wait for a touch
  if (! ctp.touched()) {
    return;
  }
  delay(500);
//    OLED.fill_OLED(0x00,0x00,0x00);
//    OLED.drawString(xyCord, 15, 15, 255, 255, 255,1);
  Questions("Please rate your mood",1);
  Questions("Please rate your pain",2);
  Questions("Please rate your comfort",3);
  Questions("Please rate your stress",4);
  Questions("Please rate your constipation",5);
  Questions("Please rate your diarrhea",6);
  //Serial Print Array
  Serial.print("{");Serial.print(response[0]);Serial.print(",");Serial.print(response[1]);Serial.print(",");
  Serial.print(response[2]);Serial.print(",");Serial.print(response[3]);Serial.print(",");Serial.print(response[4]);
  Serial.print(",");Serial.print(response[5]);Serial.println("}");
}

void Questions(String Question, int QuestionNumber) {
    OLED.fill_OLED(0x00,0x00,0x00);
    OLED.drawString(Question, 15, 15, 255, 255, 255,1);
    Serial.println(Question);
    Serial.println(" ");
    Serial.println(" ");
    while(! ctp.touched()) {
      //delay(150);
      //Serial.print(". ");
    }
   // Serial.println("/");
    
    // Retrieve a point 
    TS_Point r = ctp.getPoint();
    delay(750);
    // flip it around to match the screen.
    r.x = map(r.x, 0, 240, 240, 0);
    r.y = map(r.y, 0, 320, 320, 0);    

    
    if(r.x > 120 && r.x < 240)  {
      if(r.y > 160 && r.y < 320)  {
        response[QuestionNumber - 1] = "ok"; 
      } else if(r.y < 160 || r.y > 320) {
        response[QuestionNumber - 1]  = "slight discomfort";
      }
    }else if(r.x < 120 || r.x > 240){
      if(r.y > 160 && r.y < 320)  {
        response[QuestionNumber - 1] = "neutral";
      } else if(r.y < 160 || r.y > 320) {
        response[QuestionNumber - 1]  = "not ok";
      }
    }
}

