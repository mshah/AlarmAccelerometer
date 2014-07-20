/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 This example code is in the public domain.
 */
 #include <Servo.h>
#include <math.h>   
#include <EEPROM.h>

// set up for the accelerometer
const int X_Pin = 1;  // arduino pin for x A0
const int Y_Pin = 2;  // arduino pin for y A1
const int Z_Pin = 3;  // arduino pin for z A2
int xMid, yMid, zMid, xRng, yRng, zRng;
int xr = 0, yr = 0, zr = 0;    // x raw etc
float x = 0;
float y = 0;
float z = 0;
float tiltx, tilty, tiltz;
float prevTiltx, prevTilty, prevTiltz;
const float tiltThreshold = .05;
// set up for alarm
const int alarmPin = 9;
const int alarmFreq = 700;
const int alarmTime = 5000;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  setupAccelerometer();
}

void setupAccelerometer(){
    // read calibration values from EEprom - won't work until calibrated
    xMid = EEPROM.read(1000) << 8; 
    xMid = xMid | EEPROM.read(1001);
    xRng = EEPROM.read(1002) << 8;
    xRng = xRng | EEPROM.read(1003);
    yMid = EEPROM.read(1004) << 8; 
    yMid = yMid | EEPROM.read(1005);
    yRng = EEPROM.read(1006) << 8;
    yRng = yRng | EEPROM.read(1007);
    zMid = EEPROM.read(1008) << 8; 
    zMid = zMid | EEPROM.read(1009);
    zRng = EEPROM.read(1010) << 8;
    zRng = zRng | EEPROM.read(1011);

    // print calibration values. x, y, z pairs
    // they should look like ~300 ~160  ~300 ~160   ~300 ~160   
    
    Serial.println("Calibratrion constants: xMid, xRng, yMid, yRng, zMid, zRng");
    Serial.print(xMid);
    Serial.print("\t"); 
    Serial.print(xRng); 
    Serial.print("\t\t");
    Serial.print(yMid);
    Serial.print("\t"); 
    Serial.print(yRng); 
    Serial.print("\t\t");
    Serial.print(zMid); 
    Serial.print("\t"); 
    Serial.println(zRng);
    delay(2000);  
  
}

void  readAccelerometer(){
    xr = analogRead(X_Pin);      // read analog pin for x raw
    //     Serial.print(xr );       
    //     Serial.print("  ");
    yr = analogRead(Y_Pin);      // read analog pin for y raw
    //     Serial.print(yr ); 
    //     Serial.print(" ");     
    zr = analogRead(Z_Pin);      // read analog pin for z raw   
    //     Serial.println(zr );              

    x = (xr - xMid) / (float)xRng;
    //    Serial.print("   "); 
    y = (yr - yMid) / (float)yRng;
    //    Serial.print("         "); 
    z = (zr - zMid) / (float)zRng;

    tiltx = atan2( sqrt((y*y) + (z*z)), x);
    tilty = atan2( sqrt((x*x) + (z*z)), y);
    tiltz = atan2( sqrt((y*y) + (x*x)), z);
    
    Serial.print("tiltX: ");
    Serial.print(tiltx); // use "tiltx", "tilty" etc in your program
    Serial.print("\t tiltY: ");  // "\t" is tab character
    Serial.print(tilty); 
    Serial.print("\t tiltZ: "); 
    Serial.println(tiltz); 
    Serial.println();

    delay(50);           // delay for serial monitor only    
    
    // check if this is over the threshold: ie cat has hit the treats!!
    if (tiltOverThreshold(tiltx, tilty, tiltz)){
       // sound the alarm 
      Serial.print("Sound Alarm!");
      soundAlarm();      
    }
    
    // update the previous values
    prevTiltx = tiltx;
    prevTilty = tilty;
    prevTiltz = tiltz;
}

boolean tiltOverThreshold(float inTiltX, float inTiltY, float inTiltZ){
  
   if (abs(inTiltX - prevTiltx) > tiltThreshold){
      return true;
   }
  
   if (abs(inTiltY - prevTilty) > tiltThreshold){
      return true;
   } 
   
   if (abs(inTiltZ - prevTiltz) > tiltThreshold){
      return true; 
   }
   
   return false;
}

void soundAlarm(){
  tone(alarmPin, alarmFreq, alarmTime);  
}

// the loop routine runs over and over again forever:
void loop() {

  readAccelerometer();
}
