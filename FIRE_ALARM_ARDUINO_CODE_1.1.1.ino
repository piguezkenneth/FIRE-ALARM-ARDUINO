
// SIM INCLUDED
// GPS INCLUDED
// 2, 3, 7, 8, 11, 12





//////////////////////////////////////////////////////////////////////////
//                  MODULES                        PIN NAMES
//                 GPS                               
//
//
//
//
//
//




#include <SoftwareSerial.h>
#include <ezButton.h>
#include <TinyGPS++.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Arduino Uno Software Serial
//#define TX 2
//#define RX 3
//SoftwareSerial arduinoSerial(RX, TX);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  GSM SIM 900A CODE
//SoftwareSerial sim(7, 8);
int _timeout;
String _buffer;
String number = "+639162634029"; //-> change with your number
String receivedMessage = "";


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GPS CODE
int RXPin = 10;
int TXPin = 9;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////BUTTONS
ezButton buttonClassB(12);
ezButton buttonClassA(11);
byte buttonState_class_B = 0;
byte buttonState_class_A = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// BUZZER
#define BUZZER_PIN 4



void setup() {
  Serial.begin(115200);
  Serial2.begin(115200); ////////////////////////////////// FOR ESP32 WIFI MODULE COMMUNICATION
  Serial3.begin(115200); ////////////////////////////////// FOR GSM MODULE COMMUNICATION
  //arduinoSerial.begin(115200);
  //sim.begin(115200);
  gpsSerial.begin(115200);
  _buffer.reserve(50);
  buttonClassB.setDebounceTime(50);
  buttonClassA.setDebounceTime(50);
  pinMode(BUZZER_PIN, OUTPUT);
  delay(1000);
}




void loop() {

  char inByte = Serial2.read();
  Serial.println(inByte);
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  /*if (millis() > 5000 && gps.charsProcessed() < 10)
    {
    Serial.println("No GPS detected");
    while(true);
    }*/
  delay(200);

  ////////////////////////////////////////////////////////////////////////////////////////////////// BUTTONS CONDITION & SIM900
  CheckIncomingSMS();
  buttonClassB.loop();
  buttonClassA.loop();
  int readClassB = buttonClassB.getState();
  int readClassA = buttonClassA.getState();

  
    if(readClassB == HIGH){
      Serial.println("CLASS B INITILAZE");
    }
    else if(readClassA == HIGH){
      Serial.println("CLASS A INITILAZE");
    }
    else if(readClassB == HIGH && readClassA == HIGH){
      // DO SOMETHING HERE KENNETH
      Serial.println("BOTHN CLASS A AND B INITILAZE");
    }
    else if(readClassB == LOW && readClassA == LOW){
      Serial.println("CLASS A AND B NOT INITILAZE");
    }
    else{
      Serial.println("NOT PRESSED");
    }

  
  
  
 
  
  if (inByte == 'T') {

    Serial.println ("Sending Message");
    Serial3.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(200);
    //Serial.println ("Set SMS Number");
    Serial3.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
    delay(200);
    String SMS = "TEMPERATURE HIGH";
    Serial3.println(SMS);
    delay(100);
    Serial3.println((char)26);// ASCII code of CTRL+Z
    delay(200);
    _buffer = _readSerial();
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1000);
  }

  else if (inByte == 'S') {

    Serial.println ("Sending Message");
    Serial3.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(200);
    //Serial.println ("Set SMS Number");
    Serial3.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
    delay(200);
    String SMS = "SMOKE HIGH";
    Serial3.println(SMS);
    delay(100);
    Serial3.println((char)26);// ASCII code of CTRL+Z
    delay(200);
    _buffer = _readSerial();
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1000);
  }
  else if (inByte == 'F') {

    Serial.println ("Sending Message");
    Serial3.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(200);
    //Serial.println ("Set SMS Number");
    Serial3.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
    delay(200);
    String SMS = "FLAME HIGH";
    Serial3.println(SMS);
    delay(100);
    Serial3.println((char)26);// ASCII code of CTRL+Z
    delay(200);
    _buffer = _readSerial();
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1000);
  }
  else if (inByte == 'K') {
    Serial.println ("Sending Message");
    Serial3.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(200);
    //Serial.println ("Set SMS Number");
    Serial3.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
    delay(200);
    String SMS = "Critical Condition, all sensors detect high value";
    Serial3.println(SMS);
    delay(100);
    Serial3.println((char)26);// ASCII code of CTRL+Z
    delay(200);
    _buffer = _readSerial();
    digitalWrite(BUZZER_PIN, HIGH);
    delay(5000);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1000);
  } else {
    Serial.println("NORMAL");
  }

}


void SendMessage() {

  //Serial.println ("Sending Message");
  Serial3.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  Serial3.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Hello, kenneth";
  Serial3.println(SMS);
  delay(100);
  Serial3.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}


void CheckIncomingSMS() {
  if (Serial3.available()) {
    char c = Serial3.read();
    receivedMessage += c;

    // Check if the message is complete
    if (receivedMessage.endsWith("\r\n")) {
      Serial.println("Received SMS: " + receivedMessage);

      // Process the receivedMessage here
      if (receivedMessage.indexOf("LOCATION") != -1) {
        Serial3.println(gps.location.lat());
        delay(100);
        Serial3.println(gps.location.lng());
        delay(100);
        Serial3.println(gps.altitude.meters());

      } else if (receivedMessage.indexOf("OWNER") != -1) {
        Serial3.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
        delay(200);
        //Serial.println ("Set SMS Number");
        Serial3.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
        delay(200);
        Serial3.println("NO USERS YET");
        delay(100);
        Serial3.println((char)26);// ASCII code of CTRL+Z
        delay(200);
        _buffer = _readSerial();
      }
      else {
        Serial.println("unknown command");
      }

      receivedMessage = ""; // Clear the buffer, please dont edit this also so you'll save a memory storage from your SIM Card.
    }
  }
}


String _readSerial() {
  _timeout = 0;
  while  (!Serial3.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (Serial3.available()) {
    return Serial3.readString();
  }
}


void callNumber() {
  Serial3.print (F("ATD"));
  Serial3.print (number);
  Serial3.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}




void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
  }

  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(200);
}
