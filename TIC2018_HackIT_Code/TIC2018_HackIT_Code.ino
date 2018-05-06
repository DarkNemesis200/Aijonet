// <--    libraries     --> 
#include "OLED.h"
#include <telstraiot.h>
#include <TelstraWeb.h>
#include <connection4g.h>
#include <TelstraM1Device.h>
#include <TelstraM1Interface.h>
#include <mqttpackets.h>
// <-- end of libraries -->

// <--    declarations     -->
const int btnA = 2;
const int btnB = 3;
const int btnC = 6;
const int btnD = 7;
// <-- end of declarations -->
int response = 0;
// <--    arrays and global variables    -->
// device id
int deviceID = 000001;
// all questions in an array
String arrQuestions[] = {
  "diarrhea", // 1
  "constipation", // 2
  "nausea", // 3
  "sleep", // 4
  "mood", // 5
  "pain", // 6
  "other", // 7
  "", // 8
  "", // 9
  ""  // 10
  };
int arrLikert[] = { // 1-4 correspond to response happy -> distressed
  1, // smiley
  2, // neutral
  3, // uncomfortable
  4  // sad/ distraught
};
// possible users
int arrUser[3] = { // array sets whether patient/ carer/ other are using device
  0, //patient
  1, //carer
  2  //relative
};
// object to decide which questions are asked
bool arrAsk[] = { // 1-6 are true by default
  true,  // 1 - default
  true,  // 2 - default
  true,  // 3 - default
  true,  // 4 - default
  true,  // 5 - default
  true,  // 6 - default
  false, // 7
  false, // 8
  false, // 9
  false  // 10
  };
int arrSet[] = { // array changed by IoT
  1, // question 1
  1, // question 2
  1, // question 3
  1, // question 4
  1, // question 5
  1, // question 6
  0, // question 7
  0, // question 8
  0, // question 9
  0  // question 10
};

char arrResponse[10];
// <-- end of global variables and arrays -->

// examples for how to POST to a webpage
//#define SECURE_CONNECTION true
//#define POSTEXAMPLE "User-Agent: Arduino\r\nContent-Type: application/json\r\nX-IS-AccessKey: v0NmtMreZ7ahD8db7OjOhObbQCztaEEP\r\nX-IS-BucketKey: ARKC7VQRGNSV\r\nAccept-Version: ~0\r\nContent-Length: 30\r\n\r\n{\"key\":\"pickle\",\"value\":\"3.0\"}"
//char host[] = "httpbin.org";
//char path[] = "/ip";
//int port = 80;

char host[] = "hackit.iotdev.telstra.com";
char id[] = "10413"; //id may be wrong this is the device id on the telstra IoT webpage
char tenant[] = "hackit";
char username[] = "device";
char password[] = "HackITdevice1";


TelstraM1Interface commsif;
TelstraM1Device IoTDevice(&commsif);
Connection4G conn(true,&commsif);
TelstraWeb WebIoT(&conn,&IoTDevice);
TelstraIoT iotPlatform(&conn,&IoTDevice);
OLED OLED;


void setup() {
  commsif.begin(); // Must include for proper SPI communication
  delay(2000);
  // put your setup code here, to run once:
  // connect to IoT and check needed hardware is ok
  // update question array on startup so it's up to date
  pinMode(btnA, INPUT);
  pinMode(btnB, INPUT);
  pinMode(btnC, INPUT);
  pinMode(btnD, INPUT);

//  Serial.println("Waiting until Cellular System is ready...");
//  if(!IoTDevice.isCellularSystemReady())
//  {
//    Serial.println("waiting for IoTDevice ...");
//    IoTDevice.waitUntilCellularSystemIsReady();
//  } else {
//    Serial.println("IoTDevice ready!");   
//  }

// code for posting to IoT
//read credentials
  IoTDevice.readCredentials(id,tenant,username,password);
  
// set credentials
  iotPlatform.setCredentials(id,tenant,username,password,"");
// set host
  iotPlatform.setHost(host,443);
  
  conn.openTCP(host,443);
  
  //Initialize the OLED controller
  OLED.begin();
  OLED.fill_OLED(0x00,0x00,0x00); // Clear screen
  
  Serial.begin(115200);
  Serial.println("Setup Finished");
}

void loop() {
  // put your main code here, to run repeatedly:
  // listen for button press or something triggering the proximity sensor then gets ready to run questions()
  // if no interaction within 2 minutes then go back to waiting

//  Serial.println("loop running");

  questions();
   

  
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

}

void qSet(){
  // takes deviceID and returns an array and sets it to global variable
  for(int x = 0; x < sizeof(arrAsk); x++){
     if(arrSet[x] == 1){
      arrAsk[x] = true;
    } else {
      arrAsk[x] = false;
    }
  }    
}

void questions(){
  Serial.println("questions() called");
  // on call runs qSet() 
  // uses object from qSet() to decide which questions are asked
  qSet();

  Serial.println("Questions:");
  for(int x = 0; x < sizeof(arrQuestions); x++){
    Serial.println(arrQuestions[x]);
    OLED.fill_OLED(0x00,0x00,0x00);
    OLED.drawString(arrQuestions[x], 15, 15, 255, 255, 255,1);
    delay(2000);
    while(digitalRead(btnA) == 0 || digitalRead(btnB) == 0 || digitalRead(btnC) == 0 || digitalRead(btnD) == 0){};
    response = 0;
    if(digitalRead(btnA)){response = 1;};
    if(digitalRead(btnB)){response = 2;};
    if(digitalRead(btnC)){response = 3;};
    if(digitalRead(btnD)){response = 4;};
    
    switch (response){
    case 1:
    Serial.print("response A");
    arrResponse[x] = 1;
    delay(1000);
    break;
    case 2:
    Serial.print("response B");
    arrResponse[x] = 2;
    delay(1000);
    break;
    case 3:
    Serial.print("response C");
    arrResponse[x] = 3;
    delay(1000);
    break;
    case 4:
    Serial.print("response D");
    arrResponse[x] = 4;
    delay(1000);
    break;
    default:
    break;
    }};
    iotPlatform.sendMeasurement("UserInput", "UserInput", "Response", arrResponse, "");
}

void IoTquestionUpdate(){
  // sends deviceID to IoT
  // recieves all questions
  // iterates through to find all questions on IoT that aren't in the local array
  // appends new questions to the local array
}

