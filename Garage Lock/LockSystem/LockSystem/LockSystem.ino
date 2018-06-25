/*

Demonstrates the use a 16x2 LCD display and
4x4 LCD display.  T

The  Arduino circuit connection for LCD:

LCD RS  pin to analog pin A0

LCD  Enable pin to analog pin A1

LCD D4  pin to analog pin A2

LCD D5  pin to analog pin A3

LCD D6  pin to analog pin A4

LCD D7  pin to analog pin A5

The  Arduino circuit connection for MAtrix Key Pad:

ROW1 pin  to digital pin 5

ROW2 pin  to digital pin 4

ROW3 pin  to digital pin 3

ROW4 pin  to digital pin 2

COLUMN1  pin to digital pin 6

COLUMN2  pin to digital pin 7

COLUMN3  pin to digital pin 8

COLUMN4  pin to digital pin 9

Name:-  M.Pugazhendi

Date:-   04thJul2016

Version:-  V0.1

e-mail:-  muthuswamy.pugazhendi@gmail.com

*/

// include the library code:

#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>

#include <Keypad.h>

//initialize the library with the numbers of the  interface pins

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

//4x4 Matrix key pad

const byte ROWS = 4; // Four rows

const byte COLS = 4; // Four columns

					 // Define the Keymap

char keys[ROWS][COLS] =

{
	{ '1', '2', '3', 'A' },
	{ '4', '5', '6', 'B' },
	{ '7', '8', '9', 'C' },
	{ '*', '0', '#', 'D' }
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to Arduino pins.


byte rowPins[ROWS] = { 9, 8, 7, 6 }; //connect to the column pinouts of the keypad
byte colPins[COLS] = { 5, 4, 3 }; //connect to the row pinouts of the keypad
								  // Connect keypad COL0, COL1, COL2 and COL3 to Arduino pins.



								  // Create the Keypad

Keypad kpd = Keypad(makeKeymap(keys), rowPins,
	colPins, ROWS, COLS);


String pin = "";

/*
citac kartice
*/

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


									/*test servo*/

#include <Servo.h>
Servo myservo;  // create servo object to control a servo



void setup(){
	Serial.begin(9600);   // Initiate a serial communication

						  // set up  the LCD's number of columns and rows:


	lcd.begin(16, 2);

	// Print a message to the LCD.


	pisiLcdPrviRed("Prisloni karticu..");

	/*Citac kartice*/
	SPI.begin();      // Initiate  SPI bus
	mfrc522.PCD_Init();   // Initiate MFRC522
	Serial.println("Approximate your card to the reader...");
	Serial.println();


	/*Servo*/
	myservo.attach(2);  // attaches the servo on pin 9 to the servo object

}

void loop(){
	lcd.setCursor(0, 1);

	// print the number of seconds since reset:


	lcd.print("SEC= ");


	lcd.print(millis() / 1000);

	String pinUcitani = ucitajPin();


	if (pinUcitani.length() == 4) {

		if (provjeriPravaPin(pinUcitani, "1234")) {

			postaviServoNaStupnjeve(30);
			delay(3000);
			pisiLcdPrviRed("Prisloni karticu..");
		}
		else {
			postaviServoNaStupnjeve(90);
			delay(3000);
			pisiLcdPrviRed("Prisloni karticu..");

		}
		pin = "";
	}

	String uid = dajUIDAkoPostojiKartica();
	if (uid.length() != 0) {

		if (provjeriPrava(uid, "29 60 5A A9")) {

			postaviServoNaStupnjeve(30);
			delay(3000);
			pisiLcdPrviRed("Prisloni karticu..");
		}
		else {
			postaviServoNaStupnjeve(90);
			delay(3000);
			pisiLcdPrviRed("Prisloni karticu..");

		}

	}




}



String ucitajPin() {
	char key = kpd.getKey();

	if (key) {
		pisi(String::String(key));
		if (key == '*') {
			pin = "";
		}
		else if (key == '#') {
			//potvrdi unos
		}
		else {
			pin = pin + key;
		}

		// set the cursor to column 9, line 1
		// (note: line 1 is the second row, since counting begins with 0):
		pisiLcdDrugiRed(pin, 9);

	}
	return pin;
}
void pisi(String text) {
	Serial.println(text);
}
void postaviServoNaStupnjeve(int val) {
	Serial.println("Servo mjenjam...");
	myservo.write(val);                  // sets the servo position according to the scaled value
	delay(15);                           // waits for the servo to get there
}
void pisiLcdPrviRed(String text) {
	lcd.setCursor(0, 0);
	lcd.print("                ");
	lcd.setCursor(0, 0);
	lcd.print(text);

}

void pisiLcdDrugiRed(String text, int poz) {
	lcd.setCursor(poz, 1);
	lcd.print("                ");
	lcd.setCursor(poz, 1);
	lcd.print(text);

}
boolean provjeriPrava(String content, String key) {
	Serial.println();
	Serial.print("Message : ");
	content.toUpperCase();
	Serial.println("Sadržaj:_" + content + "_");
	if (content.substring(1) == key) //change here the UID of the card/cards that you want to give access
	{
		Serial.println("Authorized access");
		pisiLcdPrviRed("Dozvoljeno");
		Serial.println();

		return true;
	}

	else {
		Serial.println(" Access denied");
		pisiLcdPrviRed("Zabranjeno");
		return false;
	}

}

boolean provjeriPravaPin(String content, String key) {


	Serial.println();
	Serial.print("Message : ");
	content.toUpperCase();
	Serial.println("Sadržaj:_" + content + "_");
	if (content == key) //change here the UID of the card/cards that you want to give access
	{
		Serial.println("Authorized access");
		pisiLcdPrviRed("Dozvoljeno");
		Serial.println();

		return true;
	}

	else {
		Serial.println(" Access denied");
		pisiLcdPrviRed("Zabranjeno");
		return false;
	}

}

String dajUIDAkoPostojiKartica() {
	/*citac kartice*/
	// Look for new cards
	if (!mfrc522.PICC_IsNewCardPresent())
	{
		return "";
	}
	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial())
	{
		return "";
	}

	pisiLcdPrviRed("Citam karticu..");
	//Show UID on serial monitor
	Serial.print("UID tag :");
	String content = "";
	byte letter;
	for (byte i = 0; i < mfrc522.uid.size; i++)
	{
		Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
		Serial.print(mfrc522.uid.uidByte[i], HEX);
		content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
		content.concat(String(mfrc522.uid.uidByte[i], HEX));
	}

	return content;
}