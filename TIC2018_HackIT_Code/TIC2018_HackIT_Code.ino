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

String response[] = {"null","null","null","null","null","null"};
int deviceID = 000001;

// examples for how to POST to a webpage
//#define SECURE_CONNECTION true
//#define POSTEXAMPLE "User-Agent: Arduino\r\nContent-Type: application/json\r\nX-IS-AccessKey: v0NmtMreZ7ahD8db7OjOhObbQCztaEEP\r\nX-IS-BucketKey: ARKC7VQRGNSV\r\nAccept-Version: ~0\r\nContent-Length: 30\r\n\r\n{\"key\":\"pickle\",\"value\":\"3.0\"}"
//char host[] = "httpbin.org";
//char path[] = "/ip";
//int port = 80;

char host[] = "hackit.iotdev.telstra.com"; // website on telstra's network data is being posted to
char id[] = "10413"; // change this depending on device
char tenant[] = "hackit"; // username for account
char username[] = "device"; // device account username
char password[] = "HackITdevice1"; // password for device

// setting up the telstra libraries
TelstraM1Interface commsif;
TelstraM1Device IoTDevice(&commsif);
Connection4G conn(true,&commsif);
TelstraWeb WebIoT(&conn,&IoTDevice);
TelstraIoT iotPlatform(&conn,&IoTDevice);

void questions();
void setup() {
  while (!Serial);     // used for leonardo debugging
  tft.begin(); // start the touchscreen
  tft.setRotation(2);

  Serial.begin(9600);
  Serial.println(F("Aijonet device activated"));
  Serial.println(" ");
  Serial.println(" ");

  tft.fillScreen(ILI9341_WHITE);
  tft.setCursor(0, 5); //need to figure out how to rotate screen
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println(" Device Activated ");
  delay(1000);

  if (! ctp.begin(40)) {
    Serial.println("Couldn't start touchscreen");
    tft.setCursor(0, 25); //need to figure out how to rotate screen
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(" couldn't start touchscreen ");
    delay(500);
    while (1);
  }

  Serial.println("Screen Ready");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("waiting for user........");

  tft.fillScreen(ILI9341_WHITE);
  tft.setCursor(0, 5); //need to figure out how to rotate screen
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println(" touch to connect ");
  delay(500);
  if ( ctp.touched()) {
    Serial.println("Waiting until Cellular System is ready...");
    if(!IoTDevice.isCellularSystemReady())
    {
      unsigned long timer = micros();
      Serial.println("waiting for IoTDevice ...");
      tft.fillScreen(ILI9341_WHITE);
      tft.setCursor(0, 0); //need to figure out how to rotate screen
      tft.setTextColor(ILI9341_BLACK);
      tft.setTextSize(3);
      tft.println("waiting for iot");
      delay(500);
      // put a delay in
      IoTDevice.waitUntilCellularSystemIsReady();
    } else {
      Serial.println("IoTDevice ready!");
      tft.setCursor(10, 0); //need to figure out how to rotate screen
      tft.setTextColor(ILI9341_BLACK);
      tft.setTextSize(3);
      tft.println("iot ready");
      delay(500);
    }
   // code for posting to IoT
   //read credentials
     IoTDevice.readCredentials(id,tenant,username,password);
     // set credentials
     iotPlatform.setCredentials(id,tenant,username,password,"");
   // set host
     iotPlatform.setHost(host,443);
     conn.openTCP(host,443);
  }
  tft.fillScreen(ILI9341_WHITE);
  tft.setCursor(0, 5);
  tft.setTextColor(ILI9341_BLUE);
  tft.setTextSize(2);
  tft.println(" Touch to start");
}

void loop() {
  response[0] = "null";  response[1] = "null";  response[2] = "null";
  response[3] = "null";  response[4] = "null";  response[5] = "null";
  // Wait for a touch
  if (! ctp.touched()) {
    return;
  }
  delay(500);
  Questions("mood",1);
  Questions("pain",2);
  Questions("comfort",3);
  Questions("stress",4);
  Questions("constipation",5);
  Questions("diarrhea",6);
  //Serial Print Array
  Serial.print("{");Serial.print(response[0]);Serial.print(",");Serial.print(response[1]);Serial.print(",");
  Serial.print(response[2]);Serial.print(",");Serial.print(response[3]);Serial.print(",");Serial.print(response[4]);
  Serial.print(",");Serial.print(response[5]);Serial.println("}");
}

void Questions(String Question, int QuestionNumber) {
    tft.fillScreen(ILI9341_WHITE);
    tft.setCursor(0, 5);
    tft.setTextColor(ILI9341_BLUE);
    tft.setTextSize(2);
    tft.println(" Please rate your");
    tft.println(" " + Question);
    tft.fillCircle(60, 140, 50, ILI9341_GREEN);
    tft.fillCircle(180, 140, 50, ILI9341_YELLOW);
    tft.fillCircle(60, 260, 50, ILI9341_MAGENTA);
    tft.fillCircle(180, 260, 50, ILI9341_RED);

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
    // tft.fillCircle(60, 140, 50, ILI9341_GREEN);
    // tft.fillCircle(180, 140, 50, ILI9341_YELLOW);
    // tft.fillCircle(60, 260, 50, ILI9341_MAGENTA);
    // tft.fillCircle(180, 260, 50, ILI9341_RED);

    if(r.x > 120 && r.x < 240)  {
      if(r.y > 80 && r.y < 200)  {
        response[QuestionNumber - 1] = "ok";
      } else if(r.y > 240 || r.y < 320) {
        response[QuestionNumber - 1]  = "slight discomfort";
      }
    }else if(r.x < 120 || r.x > 240){
      if(r.y > 80 && r.y < 200)  {
        response[QuestionNumber - 1] = "neutral";
      } else if(r.y > 240 || r.y < 320) {
        response[QuestionNumber - 1]  = "not ok";
      }
    }
    r.x = 0;
    r.y = 0;
    ctp.getPoint();
}

void sendData () {
  // <-- Code to post to webpage -->
  //  while(Serial.available() || (digitalRead(BUTTON)));
  //  Serial.println(" Opening TCP connection!");
  //  if(conn.openTCP(host,port)==CONNECTION4G_STATUS_OK)
  //  {
  //    Serial.println(" Success!");
  //    delay(1000);
  //    // Build HTTPS request.
  //    WebIoT.post(POSTEXAMPLE);
  //    delay(2000);
  //    conn.closeTCP();
  //    } else {
  //     Serial.println(" OpenTCP() failed.");
  //  }
  //iotPlatform.sendMeasurement("UserInput", "UserInput", "Response", arrResponse, "");

}
