/*
Many thanks to nikxha from the ESP8266 forum
Modify : phuoctri@gmail.com - https://www.xda.vn/
*/

#include <ESP8266WiFi.h>
#include <SPI.h>
#include "MFRC522.h"

/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = GPIO2   = D4
SDA(SS) = GPIO15  = D8
MOSI    = GPIO13  = D7
MISO    = GPIO12  = D6
SCK     = GPIO14  = D5
GND     = GND
3.3V    = 3.3V
*/

#define RELAY_PIN  4  // D2
#define RST_PIN    2  // D4
#define SS_PIN	  15  // D8

const char *ssid =	"wifi";	    // change according to your Network - cannot be longer than 32 characters!
const char *pass =	"pass";	// change according to your Network

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance

void setup() {
  Serial.begin(115200);    // Initialize serial communications
  delay(250);
  Serial.println();
  Serial.println();
  Serial.println(F("======================================================")); 
  Serial.println(F("PhuocTri MFRC522 Access Card Loading...."));
  
  SPI.begin();	         // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  
  Serial.print(F("Connecting to ["));
  Serial.print(ssid);
  Serial.print(F("]"));
  WiFi.begin(ssid, pass);
  
  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("Done."));
  }
  else
  {
    Serial.println(F("Error!"));
  }
  
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));
  Serial.println(F("-----"));
}

void loop() { 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(500);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  String strID = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID: ["));
  Serial.print(strID);
  Serial.println(F("]"));

  if (strID == "39.FB.36.D5")
  {
    Serial.println(F("Door Opened..."));
    analogWrite(RELAY_PIN, 767);   // turn the LED on
    delay(2000);
    analogWrite(RELAY_PIN, 0);   // turn the LED off
    Serial.println(F("Door Closed."));
    Serial.println(F("-----"));
  }
  else
  {
    analogWrite(RELAY_PIN, 0);   // turn the LED off
    Serial.println(F("Access Denied!"));
    Serial.println(F("-----"));
  }
}

// Helper routine to dump a byte array as hex values to Serial
String dump_byte_array(byte *buffer, byte bufferSize) {
  String content= "";
  for (byte i = 0; i < bufferSize; i++) {
    content.concat(String(buffer[i] < 0x10 ? "0" : ""));
    content.concat(String(buffer[i], HEX));
    if (i < bufferSize - 1)
    {
      content.concat(".");
    }
  }
  content.toUpperCase();
  return content;
}
