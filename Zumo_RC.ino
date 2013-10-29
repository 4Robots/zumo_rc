// 29.10.2013 kkostyuk
// Version 0.3 motor control depending of time
// External lib IRremote - https://github.com/shirriff/Arduino-IRremote
#include <IRremote.h>
#include <IRremoteInt.h>
// External lib ArduMoto - https://github.com/4Robots/arduino-ardumotor-library
#include <ArduMoto.h>
#include <defines.h>

#define FW 0
#define FW 1

#define MAX_SPEED 100
#define STEP_SPEED 20
#define ACTION_DELEY 300
#define STEP_DELEY 50

//Pin config
const int buttonPin = 2; //button 
//TSOP
const int TSOP_Pin = 8; //button 
IRrecv irrecv(TSOP_Pin);
decode_results results;

//Motor
ArduMoto Moto;
int motorAspeed = 0;
int motorBspeed = 0;

//Time
long int currentTime = 0;
long int actionTime = 0;
long int diffTime = 0;

//Get motor direction +motorSpeed = TRUE and -motorSpeed = False
boolean getDir(int motorSpeed)
{
  if (motorSpeed > 0)
    return true;
  else 
    return false;
}

//Change motorSpeed to dalta with checks directions
int changeSpeed(int currentSpeed, int deltaSpeed, int abs_maxSpeed)
{ 
  int newSpeed = currentSpeed; 
  newSpeed += deltaSpeed;
  if (currentSpeed >= 0){    
    if (newSpeed > abs_maxSpeed) newSpeed = abs_maxSpeed;
  }
  else{
    if (newSpeed < -abs_maxSpeed) newSpeed = -abs_maxSpeed;
  };
  
  return newSpeed; 
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
  currentTime = millis();

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    actionTime = currentTime + ACTION_DELEY;
   //MAin case
    switch(lowByte(results.value)){
    //Go forward
    case B00000011:
    case B00100000:
      motorAspeed=changeSpeed(max(motorAspeed,motorBspeed), STEP_SPEED, MAX_SPEED);
      motorBspeed=changeSpeed(max(motorAspeed,motorBspeed), STEP_SPEED, MAX_SPEED);
      Moto.setSpeeds(motorAspeed,motorBspeed);	  
      Serial.println("Forward");
   break;
    //Go rotate left
    case B00000001:
    case B00010000:
      motorAspeed=changeSpeed(motorAspeed, -STEP_SPEED, MAX_SPEED);
      motorBspeed=changeSpeed(motorBspeed, STEP_SPEED, MAX_SPEED);      
      Moto.setSpeeds(motorAspeed,motorBspeed);
      Serial.println("Left");
    break;
    //Go rotate right
    case B00000010:
    case B00010001:
      motorAspeed=changeSpeed(motorAspeed, STEP_SPEED, MAX_SPEED);
      motorBspeed=changeSpeed(motorBspeed, -STEP_SPEED, MAX_SPEED);      
      Moto.setSpeeds(motorAspeed,motorBspeed);
      Serial.println("Right");
    break;
    //Go to back
    case B00100001:
      motorAspeed=changeSpeed(min(motorAspeed,motorBspeed), -STEP_SPEED, MAX_SPEED);
      motorBspeed=changeSpeed(min(motorAspeed,motorBspeed), -STEP_SPEED, MAX_SPEED);
      Moto.setSpeeds(motorAspeed,motorBspeed);
      Serial.println("Back");
    break;
    //Stop    
    case B00000000:
    case B00111011:
      //stopBot();
      Moto.stop('A');
      motorAspeed=0;
      Moto.stop('B');   
      motorBspeed=0;   
      Serial.println("Stop");
    default:
      Serial.print("Incorrect comand: ");
      Serial.println(results.value, BIN);
    };
  irrecv.resume(); // Receive the next value
      #ifdef DEBUG_MOTORS
        Serial.print("Setting MotorA: ");
        Serial.println(motorAspeed);
        Serial.print("Setting MotorB: ");
        Serial.println(motorBspeed);
      #endif
  }  
  diffTime=actionTime-currentTime;
  if (diffTime<0)
    {
     actionTime=currentTime+STEP_DELEY;
     if (motorAspeed != 0){
       if (motorAspeed > 0) motorAspeed=changeSpeed(motorAspeed, -1, MAX_SPEED);
         else motorAspeed=changeSpeed(motorAspeed, 1, MAX_SPEED);
       Moto.setSpeed('A',motorAspeed);
     };     
     if (motorBspeed != 0){
       if (motorBspeed > 0) motorBspeed=changeSpeed(motorBspeed, -1, MAX_SPEED);
         else motorBspeed=changeSpeed(motorBspeed, 1, MAX_SPEED);
       Moto.setSpeed('B',motorBspeed);
     };   
      #ifdef DEBUG_MOTORS
        Serial.print("Setting MotorA: ");
        Serial.println(motorAspeed);
        Serial.print("Setting MotorB: ");
        Serial.println(motorBspeed);
      #endif
    } 
 }
