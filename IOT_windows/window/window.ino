/* ==========================================================
  Project : Send Photo resistor values to computer
  Author: ScottC
  Created: 25th June 2011
  Description: This sketch will make the arduino read Photo resistor
             values on analog pin A0. The analog readings will
             be dependent on the amount of light reaching the
             sensor. The Analog readings will be sent to the
             computer via the USB cable using Serial communication.
  ==============================================================
*/
//#include <EEPROM.h>
#include <stdarg.h>
#include <Arduino.h>
//sens pin IN
int photoRPin = 0; // analog pin 0 - alert o/p
int rainRPin = 1; // digital pin 1 - alert o/p

//leds pin OUT
int alertLightLed = 8; // digital pin 8 - alert o/p
int alertRainLed = 9; // digital pin 9 - alert o/p
int alertCommonLed = 7; // digital pin 7 - alert o/p

int alertOpenWindowLed = 6; // digital pin 6 - alert o/p


int lightSens;
int rainSens;



// threshold value
const int RAIN_THRESHOLD_LEVEL = 600; // ca. 0  - 1000
const int LIGHT_THRESHOLD_LEVEL = 800; // ca. 0  -  1000


//time
unsigned long currentMillis = 0;

unsigned long sendCloseWindowsSignal_START = 0;

unsigned long previousAutoSignal = 0;



const long INTERVAL_AUTO_SEND_OPEN = 20000;           // interval suto send every 2h
const long INTERVAL_AUTO_SEND_CLOSE = 10000;           // interval suto send every 2h
const long INTERVAL = 0;           // interval for checking sensors


boolean sendSignal = false;
boolean sendSignalOpenClose = false;
boolean signal_Already_Sent = false;
boolean needSendAutoSignal = false;

//test
// start reading from the first byte (address 0) of the EEPROM
int address = 0;
byte value;
//end test


unsigned long timerStart = false;
unsigned long timerValue = millis();

void setup() {
  Serial.begin(9600);
  //START
  //Read light value
  lightSens = analogRead(photoRPin);

  //Read rain value
  rainSens = analogRead(rainRPin);



  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(alertLightLed, OUTPUT);
  pinMode(alertRainLed, OUTPUT);
  pinMode(alertCommonLed, OUTPUT);

  pinMode(alertOpenWindowLed, OUTPUT);

  sendSignal = false;
  signal_Already_Sent = false;
  previousAutoSignal = millis();

  Serial.println("****************************************************** \nSETTINGS: \n******************************************************" );

  Serial.print("RAIN THRESHOLD: ");
  Serial.print(RAIN_THRESHOLD_LEVEL);
  Serial.print("  \n");

  Serial.print("LIGHT THRESHOLD: ");
  Serial.print(LIGHT_THRESHOLD_LEVEL);
  Serial.print("  \n");

  Serial.print("AUTO OPEN: ");
  Serial.print(INTERVAL_AUTO_SEND_OPEN / 1000);
  Serial.print("  s\n");

  Serial.print("AUTO CLOSE: ");
  Serial.print(INTERVAL_AUTO_SEND_CLOSE / 1000);
  Serial.print("  s");
  
  Serial.print("  \n****************************************************** \n \n");


}

void loop() {
  currentMillis = millis();

  //auto-adjust the minimum and maximum limits in real time
  lightSens = analogRead(photoRPin);
  rainSens = analogRead(rainRPin);

  //need delete
  lightSens = 650;
  rainSens = 800;

  sendMessageEverySec();


  if (checkLight(lightSens) || checkRain(rainSens)) {
    sendCloseWindowsSignal(true);
  } else {
    sendCloseWindowsSignal(false);
  }


  //send signal every 2 h
  //sendAutoSignal();
  sendAutoSignalOpen();


  //slow down the transmission for effective Serial communication.
  delay(INTERVAL);
}
void sendMessageEverySec() {
  if (! timerStart) {
    timerStart = true;
    timerValue = millis();
  }

  if ( millis() >= timerValue + 1000 ) {
    Serial.print("RAIN LEVEL:" );
    Serial.println(rainSens);
    Serial.print("LIGHT LEVEL: ");
    Serial.println(lightSens);
    Serial.println();
    timerStart = false;

  }

}

boolean checkLight(int value) {
  if (value > LIGHT_THRESHOLD_LEVEL) //under threshold
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(alertLightLed, HIGH);
    return true;
  }
  else {                          //above threshold
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(alertLightLed, LOW); // NO_LIGHT
    return false;
  }

}

boolean checkRain(int value) {
  if (value < RAIN_THRESHOLD_LEVEL) { // while raining at threshold level - see text
    digitalWrite(alertRainLed, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    return true;
  }
  else { // if not raining at threshold level
    digitalWrite(alertRainLed, LOW);
    digitalWrite(alertRainLed, LOW);
    return false;
  }
}


void sendAutoSignalOpen () {
  needSendAutoSignal = false;
  if (currentMillis > previousAutoSignal + INTERVAL_AUTO_SEND_OPEN && !signal_Already_Sent) {
    previousAutoSignal = currentMillis;
    needSendAutoSignal = true;
  } else {
    needSendAutoSignal = false;
  }

  //OPEN BEGIN
  if (!sendSignalOpenClose && needSendAutoSignal && !sendSignal) {
    sendSignalOpenClose = true;
    sendCloseWindowsSignal_START = currentMillis;
    digitalWrite(alertOpenWindowLed, HIGH);
   // Serial.print("OPEN 1");
  } else if (sendSignalOpenClose && currentMillis > sendCloseWindowsSignal_START + 1990 && currentMillis < sendCloseWindowsSignal_START + 3990 ) {
    digitalWrite(alertOpenWindowLed, LOW);
    //Serial.print("OPEN 2");
  }
  else if (sendSignalOpenClose && currentMillis > sendCloseWindowsSignal_START + 3990 && currentMillis < sendCloseWindowsSignal_START + 5990 ) {
    digitalWrite(alertOpenWindowLed, HIGH);
    //Serial.print("OPEN 3");

  }
  else if (sendSignalOpenClose && currentMillis > sendCloseWindowsSignal_START + 5990 && currentMillis < sendCloseWindowsSignal_START + INTERVAL_AUTO_SEND_CLOSE) {
    digitalWrite(alertOpenWindowLed, LOW);
   // Serial.print("OPEN 4");

    //sendSignalOpenClose = false;
    //previousAutoSignal = millis();
  }
  //OPEN END

  //close WINDOWS BEGIN
  else if (sendSignalOpenClose && currentMillis >= sendCloseWindowsSignal_START + INTERVAL_AUTO_SEND_CLOSE && currentMillis < sendCloseWindowsSignal_START + INTERVAL_AUTO_SEND_CLOSE + 1990) {
    digitalWrite(alertCommonLed, HIGH);
    //Serial.print("CLOSE 1");
    //sendSignalOpenClose = false;
    //previousAutoSignal = millis();
  }
  else if (sendSignalOpenClose && currentMillis > sendCloseWindowsSignal_START + INTERVAL_AUTO_SEND_CLOSE + 1990 && currentMillis < sendCloseWindowsSignal_START + INTERVAL_AUTO_SEND_CLOSE + 3990) {
    digitalWrite(alertCommonLed, LOW);
   // Serial.print("CLOSE 2");
    //sendSignalOpenClose = false;
    //previousAutoSignal = millis();
  }
  else if (sendSignalOpenClose && currentMillis > sendCloseWindowsSignal_START + INTERVAL_AUTO_SEND_CLOSE + 3990 && currentMillis < sendCloseWindowsSignal_START + INTERVAL_AUTO_SEND_CLOSE + 5990) {
    digitalWrite(alertCommonLed, HIGH);
    //Serial.print("CLOSE 3");
    //sendSignalOpenClose = false;
    //previousAutoSignal = millis();
  }
  else if (sendSignalOpenClose && currentMillis > sendCloseWindowsSignal_START + INTERVAL_AUTO_SEND_CLOSE + 5990) {
    digitalWrite(alertCommonLed, LOW);
    //Serial.print("CLOSE 4");
    sendSignalOpenClose = false;
    previousAutoSignal = millis();
  }
}


void sendCloseWindowsSignal(boolean needSend) {

  if (!sendSignal && needSend && !signal_Already_Sent) {
    sendSignal = true;
    sendCloseWindowsSignal_START = currentMillis;
    digitalWrite(alertCommonLed, HIGH);
  }
  else if (sendSignal && currentMillis > sendCloseWindowsSignal_START + 1990 && currentMillis < sendCloseWindowsSignal_START + 3990 ) {
    digitalWrite(alertCommonLed, LOW);
  }
  else if (sendSignal && currentMillis > sendCloseWindowsSignal_START + 3990 && currentMillis < sendCloseWindowsSignal_START + 5990 ) {
    digitalWrite(alertCommonLed, HIGH);
  }
  else if (sendSignal && currentMillis > sendCloseWindowsSignal_START + 5990) {
    digitalWrite(alertCommonLed, LOW);
    sendSignal = false;
    previousAutoSignal = millis();
  }

  if (!signal_Already_Sent && needSend) {
    signal_Already_Sent = true;
  } else if (signal_Already_Sent && !needSend) {
    signal_Already_Sent = false;
  }
}

