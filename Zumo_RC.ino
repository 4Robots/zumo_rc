// 22.10.2013 kkostyuk
// Version 0.2 with motor control by external lib
// External lib IRremote - https://github.com/shirriff/Arduino-IRremote
#include <IRremote.h>
#include <IRremoteInt.h>
// External lib ArduMoto - https://github.com/4Robots/arduino-ardumotor-library
#include <ArduMoto.h>
#include <defines.h>

#define FW 0
#define FW 1

//#define MAXSPEED 50

//Pin config
const int buttonPin = 2; //button 
//TSOP
const int TSOP_Pin = 8; //button 
IRrecv irrecv(TSOP_Pin);
decode_results results;

//Motor
ArduMoto Moto;
byte motorAspeed = 0;
byte motorBspeed = 0;

byte changeSpeed(byte motorSpeed, byte change, byte dir)
{
  boolean _dir = (motorSpeed < 0);
  
  byte _speed = abs(motorSpeed);
  if (dir == FW) {
    _speed += change;
    return _speed;}
  else {
    _speed -= change;
    return -_speed;}
   
}


void setup() {                
  Serial.begin(115200);
//---Motors init
  Moto.begin(); 
//---StartButton
  pinMode(buttonPin, INPUT); 
//---TSOP
  irrecv.enableIRIn(); // Start the receiver  
  
Serial.println("Init Complete.");
}

void loop() {

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
   //MAin case
    switch(lowByte(results.value)){
    //Go forward
    case B00000011:
    case B00100000:
      Moto.setSpeeds(100,100);    
    Serial.println("Forward");
    break;
    //Go rotate left
    case B00000001:
    case B00010000:
      Moto.setSpeeds(0,50);
      Serial.println("Left");
    break;
    //Go rotate right
    case B00000010:
    case B00010001:
      Moto.setSpeeds(50, 0);
      Serial.println("Right");
    break;
    //Go to back
    case B00100001:
      Moto.setSpeeds(-50,-50);
      Serial.println("Back");
    break;
    //Stop    
    case B00000000:
    case B00111011:
      //stopBot();
      Moto.stop('A');
      Moto.stop('B');      
      Serial.println("Stop");
    default:
      Serial.print("Incorrect comand: ");
      Serial.println(results.value, BIN);
    };
    irrecv.resume(); // Receive the next value
  }  
}
