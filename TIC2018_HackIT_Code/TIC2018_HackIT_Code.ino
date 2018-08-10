#include <SPI.h>       // this is needed for display
#include <telstraiot.h>
#include <TelstraWeb.h>
#include <connection4g.h>
#include <TelstraM1Device.h>
#include <TelstraM1Interface.h>
#include <mqttpackets.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define TFT_MOSI 11
#define TFT_CLOCK 13

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ctp = Adafruit_FT6206();

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLOCK);

// If using ICSP header for SPI communication make sure ICSP jumpers on TFT shield are shorted
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

String xyCord;
String response[] = {"null","null","null","null","null","null"};
void questions();

void setup(void) {
  while (!Serial);     // used for leonardo debugging

  Serial.begin(9600);
  Serial.println(F("Aijonet device activated"));
  Serial.println(" ");
  Serial.println(" ");

tft.begin();

  if (! ctp.begin(40)) {
    Serial.println("Couldn't start touchscreen");
    while (1);
  }

  Serial.println("Screen Ready");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("waiting for user........");


  tft.fillScreen(ILI9341_BLACK);

}

void loop() {
  response[0] = "null";  response[1] = "null";  response[2] = "null";
  response[3] = "null";  response[4] = "null";  response[5] = "null";
  // Wait for a touch
  if (! ctp.touched()) {
    return;
  }
  delay(500);
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

    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println(Question);

    Serial.println(Question);
    Serial.println(" ");
    Serial.println(" ");
    if(! ctp.touched()) {
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
