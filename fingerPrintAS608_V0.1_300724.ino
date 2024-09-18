//#add to the finder print data into the google sheet 
#include <WiFiClient.h> 
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "CEMD";
const char *password = "cemd1811";



// Google script Web_App_URL.
String Web_App_URL = "REPLACE_WITH_YOUR_WEB_APP_URL";


#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "logo.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define Finger_Tx 0 //D3
#define Finger_Rx 2 //D4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SoftwareSerial mySerial(Finger_Rx, Finger_Tx);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//int FingerID = 0;
uint8_t id;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

//  display.display();
  display.clearDisplay();
  display.drawBitmap(0, 0, welcom_scan_bits, welcom_scan_width, welcom_scan_height, WHITE);
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();

  connectToWiFi(); 
  
  finger.begin(57600);
  display.clearDisplay();
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    display.drawBitmap(34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
    display.display();
  } else {
    Serial.println("Did not find fingerprint sensor :");
    display.drawBitmap(32, 0, FinPr_failed_bits, FinPr_failed_width, FinPr_failed_height, WHITE);
    display.display();
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains ");
  Serial.print(finger.templateCount);
  Serial.println(" templates");
  Serial.println("Waiting for valid finger...");


//display.clearDisplay();
//    display.drawBitmap(32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
//    display.display();
//    Serial.println("1");
//delay(2000);
//    
//display.clearDisplay();
//    display.drawBitmap(34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
//    display.display();
//    Serial.println("2");
//    delay(2000);
//display.clearDisplay();
//    display.drawBitmap(32, 0, FinPr_failed_bits, FinPr_failed_width, FinPr_failed_height, WHITE); 
//    display.display();
//    Serial.println("3");
//    delay(2000);

  
}

void loop() {
  int result = getFingerprintID();
  if (result != 0) {
    Serial.print("Fingerprint ID: ");
    Serial.println(result);
    DisplayFingerprintID(result);
  } else {
    display.clearDisplay();
    display.drawBitmap(32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
    display.display();
//    Serial.println("No valid fingerprint detected.");
  }
  delay(1000); // Increase delay to avoid rapid polling
}
void DisplayFingerprintID(int id) {
  display.clearDisplay();
  if (id > 0) {
    display.drawBitmap(34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
    display.display();
    Serial.println(id);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(15, 0);
    display.print("Welcome");
    display.setCursor(0, 20);
    display.print(id == 1 ? "Pratik" : "User");
    display.display();
  } else {
    display.drawBitmap(id == -1 ? 34 : 32, 0, id == -1 ? FinPr_invalid_bits : FinPr_failed_bits, id == -1 ? FinPr_invalid_width : FinPr_failed_width, id == -1 ? FinPr_invalid_height : FinPr_failed_height, WHITE);
    display.display();
//    if (id == 0) {
//    display.clearDisplay();
//    display.drawBitmap(32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
//    display.display();
//  } else if (id == -1) {
//    display.clearDisplay();
//    display.drawBitmap(34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
//    display.display();
//  } else if (id == -2) {
//    display.clearDisplay();
//    display.drawBitmap(32, 0, FinPr_failed_bits, FinPr_failed_width, FinPr_failed_height, WHITE);
//    display.setCursor(0, 0);
//    display.print(F("Not"));
//    display.setCursor(80, 0);
//    display.print(F("Find"));
//    display.display();
//  }


    
  }
}


//void loop() {
//  FingerID = getFingerprintID();
//  DisplayFingerprintID();
//  delay(500); // Add a delay to avoid continuous rapid polling
//}
//
//void DisplayFingerprintID() {
//  if (FingerID > 0) {
//    display.clearDisplay();
//    display.drawBitmap(34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
//    display.display();
//    Serial.println(FingerID);
//
//    display.clearDisplay();
//    display.setTextSize(2);
//    display.setTextColor(WHITE);
//    display.setCursor(15, 0);
//    display.print(F("Welcome"));
//    display.setCursor(0, 20);
//
//    if(FingerID==1){
//      display.print("Pratik"); // Replace with actual user name if available
//    }
//    else
//      display.print("User"); // Replace with actual user name if available
//    display.display();
//  }
//  
//  else if (FingerID == 0) {
//    display.clearDisplay();
//    display.drawBitmap(32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
//    display.display();
//  } else if (FingerID == -1) {
//    display.clearDisplay();
//    display.drawBitmap(34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
//    display.display();
//  } else if (FingerID == -2) {
//    display.clearDisplay();
//    display.drawBitmap(32, 0, FinPr_failed_bits, FinPr_failed_width, FinPr_failed_height, WHITE);
//    display.setCursor(0, 0);
//    display.print(F("Not"));
//    display.setCursor(80, 0);
//    display.print(F("Find"));
//    display.display();
//  }
//}

//********************Get the Fingerprint ID******************
int getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
//      Serial.println("No finger detected");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
      return -2;
    case FINGERPRINT_IMAGEFAIL:
//      Serial.println("Imaging error");
      return -2;
    default:
//      Serial.println("Unknown error");
      return 0;
  }
  // OK success!
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
//      Serial.println("Image too messy");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
      return -2;
    case FINGERPRINT_FEATUREFAIL:
//      Serial.println("Could not find fingerprint features");
      return 0;
    case FINGERPRINT_INVALIDIMAGE:
//      Serial.println("Could not find fingerprint features");
      return -2;
    default:
      //Serial.println("Unknown error");
      return 0;
  }
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return -2;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    return -1;
  } else {
    //Serial.println("Unknown error");
    return 0;
  }   
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}
//******************Check if there a Fingerprint ID to delete******************



//int getFingerprintID() {
//  uint8_t p = finger.getImage();
//  if (p != FINGERPRINT_OK) {
//    return handleFingerprintError(p);
//  }
//
//  p = finger.image2Tz();
//  if (p != FINGERPRINT_OK) {
//    return handleFingerprintError(p);
//  }
//
//  p = finger.fingerFastSearch();
//  if (p != FINGERPRINT_OK) {
//    return handleFingerprintError(p);
//  }
//
//  return finger.fingerID;
//}

//int handleFingerprintError(uint8_t error) {
//  switch (error) {
//    case FINGERPRINT_NOFINGER:
//      return 0;
//    case FINGERPRINT_PACKETRECIEVEERR:
//    case FINGERPRINT_NOTFOUND:{
//         Serial.println("Not Find");
//         return -2;}
//    case FINGERPRINT_IMAGEFAIL:
//    case FINGERPRINT_IMAGEMESS:
//    case FINGERPRINT_FEATUREFAIL:
//    case FINGERPRINT_INVALIDIMAGE:         
//      return -5;
//    default:
//      return -2;
//  }
//}


//********************connect to the WiFi******************
void connectToWiFi(){
    WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.print(F("Connecting to \n"));
    display.setCursor(0, 50);   
    display.setTextSize(2);          
    display.print(ssid);
    display.drawBitmap( 73, 10, Wifi_start_bits, Wifi_start_width, Wifi_start_height, WHITE);
    display.display();
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected");
    
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(8, 0);             // Start at top-left corner
    display.print(F("Connected \n"));
    display.drawBitmap( 33, 15, Wifi_connected_bits, Wifi_connected_width, Wifi_connected_height, WHITE);
    display.display();
    
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP

}
