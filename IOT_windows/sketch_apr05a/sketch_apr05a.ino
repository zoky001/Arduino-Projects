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

//sens pin IN
int photoRPin = 0; // analog pin 0 - alert o/p
int rainRPin = 1; // digital pin 1 - alert o/p

//leds pin OUT
int alertLightLed= 8; // digital pin 8 - alert o/p
int alertRainLed= 9; // digital pin 9 - alert o/p
int alertCommonLed= 7; // digital pin 7 - alert o/p




int lightSens;
int rainSens;



// threshold value
const int RAIN_THRESHOLD_LEVEL = 600;
const int LIGHT_THRESHOLD_LEVEL = 800;


//time
unsigned long currentMillis = 0; 

unsigned long sendCloseWindowsSignal_START = 0;

unsigned long previousAutoSignal = 0;



const long INTERVAL_AUTO_SEND = 30000;           // interval suto send every 2h
const long INTERVAL = 0;           // interval for checking sensors


boolean sendSignal = false;
boolean signal_Already_Sent = false;
boolean needSendAutoSignal = false;



void setup() {
 Serial.begin(9600);
 
   //Read light value
   lightSens=analogRead(photoRPin);
 
   //Read rain value
   rainSens=analogRead(rainRPin);

   // initialize digital pin LED_BUILTIN as an output.
   pinMode(LED_BUILTIN, OUTPUT);
  
   pinMode(alertLightLed, OUTPUT);
   pinMode(alertRainLed, OUTPUT);
   pinMode(alertCommonLed, OUTPUT);
      
   sendSignal = false;
   signal_Already_Sent = false;
   previousAutoSignal = millis();
}

void loop(){
  currentMillis = millis();


  
    //auto-adjust the minimum and maximum limits in real time
      lightSens=analogRead(photoRPin);
      rainSens=analogRead(rainRPin);

 
 
    if(checkLight(lightSens) || checkRain(rainSens)){
     sendCloseWindowsSignal(true);  
     }else{
       sendCloseWindowsSignal(false);   
         }


    //send signal every 2 h
    sendAutoSignal();
    

  
 //slow down the transmission for effective Serial communication.
 delay(INTERVAL);
}


boolean checkLight(int value){
 if(value > LIGHT_THRESHOLD_LEVEL) //under threshold
 {       
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(alertLightLed, HIGH);
      return true;
  }
  else{                           //above threshold
     digitalWrite(LED_BUILTIN, LOW);
     digitalWrite(alertLightLed, LOW); // NO_LIGHT
     return false;
    }
  
}

boolean checkRain(int value){
   if (value < RAIN_THRESHOLD_LEVEL){ // while raining at threshold level - see text
        digitalWrite(alertRainLed, HIGH);
        digitalWrite(LED_BUILTIN, HIGH); 
        return true;
   }
   else{ // if not raining at threshold level
        digitalWrite(alertRainLed, LOW);
        digitalWrite(alertRainLed, LOW); 
        return false;
   }   
}


void sendAutoSignal (){
  needSendAutoSignal = false;
if(currentMillis > previousAutoSignal + INTERVAL_AUTO_SEND && !signal_Already_Sent){
  previousAutoSignal = currentMillis;
  needSendAutoSignal = true;
  }else{
    needSendAutoSignal = false;  
    }
  
    if(!sendSignal && needSendAutoSignal){  
         sendSignal = true;  
         sendCloseWindowsSignal_START = currentMillis;
         digitalWrite(alertCommonLed, HIGH); 
    }else if (sendSignal && currentMillis > sendCloseWindowsSignal_START + 1990 && currentMillis < sendCloseWindowsSignal_START + 3990 ){
          digitalWrite(alertCommonLed, LOW);
      }
      else if (sendSignal && currentMillis > sendCloseWindowsSignal_START + 3990 && currentMillis < sendCloseWindowsSignal_START + 5990 ){
               digitalWrite(alertCommonLed, HIGH); 
      }
          else if (sendSignal && currentMillis > sendCloseWindowsSignal_START + 5990){
                 digitalWrite(alertCommonLed, LOW); 
                  sendSignal = false; 
                  previousAutoSignal = millis(); 
      }  
}

void sendCloseWindowsSignal(boolean needSend){
   
  if(!sendSignal && needSend && !signal_Already_Sent){  
        sendSignal = true;  
        sendCloseWindowsSignal_START = currentMillis;
        digitalWrite(alertCommonLed, HIGH);   
    }
    else if (sendSignal && currentMillis > sendCloseWindowsSignal_START + 1990 && currentMillis < sendCloseWindowsSignal_START + 3990 ){
        digitalWrite(alertCommonLed, LOW);
      }
      else if (sendSignal && currentMillis > sendCloseWindowsSignal_START + 3990 && currentMillis < sendCloseWindowsSignal_START + 5990 ){
      digitalWrite(alertCommonLed, HIGH);  
      }
      else if (sendSignal && currentMillis > sendCloseWindowsSignal_START + 5990){
                digitalWrite(alertCommonLed, LOW); 
                sendSignal = false;  
      }  

        if(!signal_Already_Sent && needSend){
          signal_Already_Sent = true;    
         }else if(signal_Already_Sent && !needSend){
            signal_Already_Sent = false; 
       }
  }

