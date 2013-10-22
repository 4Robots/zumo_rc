// 20.09.2013 kkostyuk
// Initial version with direct motor control
// External lib IRremote - https://github.com/shirriff/Arduino-IRremote
#include <IRremote.h>
#include <IRremoteInt.h>

//Pin config
const int buttonPin = 2; //button 
//TSOP
const int TSOP_Pin = 8; //button 
IRrecv irrecv(TSOP_Pin);
decode_results results;

//Motor
const int M1_Dir_Pin =  12; 
const int M1_PWM_Pin =  3; 
const int M2_Dir_Pin =  13; 
const int M2_PWM_Pin =  11;



void rigth_f(){
    digitalWrite(M1_Dir_Pin, HIGH);   
    digitalWrite(M1_PWM_Pin, HIGH);    
    
    digitalWrite(M2_Dir_Pin, LOW);   
    digitalWrite(M2_PWM_Pin, LOW);   
}
void left_f(){
    digitalWrite(M1_Dir_Pin, LOW);   
    digitalWrite(M1_PWM_Pin, LOW);   
    
    digitalWrite(M2_Dir_Pin, HIGH);   
    digitalWrite(M2_PWM_Pin, HIGH);    
}


void rigth(){
    digitalWrite(M1_Dir_Pin, HIGH);   
    digitalWrite(M1_PWM_Pin, HIGH);   
    
    digitalWrite(M2_Dir_Pin, LOW);   
    digitalWrite(M2_PWM_Pin, HIGH);   
}
void left(){
    digitalWrite(M1_Dir_Pin, LOW);   
    digitalWrite(M1_PWM_Pin, HIGH);   
    
    digitalWrite(M2_Dir_Pin, HIGH);   
    digitalWrite(M2_PWM_Pin, HIGH);
}

void forward(){
    digitalWrite(M1_Dir_Pin, HIGH);   
    digitalWrite(M1_PWM_Pin, HIGH);   
    
    digitalWrite(M2_Dir_Pin, HIGH);   
    digitalWrite(M2_PWM_Pin, HIGH);
}

void back(){
    digitalWrite(M1_Dir_Pin, LOW);   
    digitalWrite(M1_PWM_Pin, HIGH);   
    
    digitalWrite(M2_Dir_Pin, LOW);   
    digitalWrite(M2_PWM_Pin, HIGH);
}
void stopBot(){
    digitalWrite(M1_Dir_Pin, LOW);   
    digitalWrite(M1_PWM_Pin, LOW);   
    
    digitalWrite(M2_Dir_Pin, LOW);   
    digitalWrite(M2_PWM_Pin, LOW);
}


void setup() {                
  Serial.begin(115200);
//---Motors init
  pinMode(M1_Dir_Pin, OUTPUT);  //M2   
  pinMode(M1_PWM_Pin, OUTPUT);     

  pinMode(M2_Dir_Pin, OUTPUT); //M1    
  pinMode(M2_PWM_Pin, OUTPUT);     
  stopBot();
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
      forward();
    Serial.println("Forward");
    break;
    //Go rotate left
    case B00000001:
    case B00010000:
      left_f();
      Serial.println("Left");
    break;
    //Go rotate right
    case B00000010:
    case B00010001:
      rigth_f();
      Serial.println("Right");
    break;
    //Go to back
    case B00100001:
      back();
      Serial.println("Back");
    break;
    //Stop    
    case B00000000:
    case B00111011:
      stopBot();
      Serial.println("Stop");
    default:
      Serial.print("Incorrect comand: ");
      Serial.println(results.value, BIN);
    };
    irrecv.resume(); // Receive the next value
  }  
}
