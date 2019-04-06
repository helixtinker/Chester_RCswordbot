/*
 * code for remote control cart with servos that move a 
 * couple of swords or whatever you hook up.
 * Drive around and smack things 
 * Using the Giumig Apps Arduino Bluetooth Controller app for
 * android with the controller mode.  It should work with 
 * most any similar bluetooth conrtoller app
 *
 * TODO: add slow mode - need library??
 * TODO: learn to add diode to prevent "backwash"
 */

#include <SoftwareSerial.h>
#include <Servo.h>

//inputs-------------------
Servo servo1; //left
Servo servo2;//right
bool attackOn;
bool reverseOn;
bool slowOn;
int driveSpeed;
SoftwareSerial Bluetooth(10, 9); // RX, TX
int btData;

#define MOTOR_A_EN_PIN 5
#define MOTOR_B_EN_PIN 9
#define MOTOR_A_IN_1_PIN 7
#define MOTOR_A_IN_2_PIN 8
#define MOTOR_B_IN_1_PIN 12
#define MOTOR_B_IN_2_PIN 4
#define MOTOR_A_OFFSET 5
#define MOTOR_B_OFFSET 0

//actions------------------
void DoAttack(){
  //servo1
  Serial.println("attacking!");
      for(int i=10;i<=170; i++){
 servo1.write(i);
 delay(2);
} 
   for(int i=170;i>=10;i--){
 servo1.write(i);
delay(4);
}  

  //servo2
   for(int i=170;i>=10;i--){
 servo2.write(i);
 delay(2);
 }  
   for(int i=10;i<=170; i++){
 servo2.write(i);
 delay(4);
 }
}

//Drive commands
void setSpeedMotorA(int speed) {
  if(speed > 10){
     if(reverseOn){
      digitalWrite(MOTOR_A_IN_1_PIN, HIGH);
  digitalWrite(MOTOR_A_IN_2_PIN, LOW);
  }
  else{
      digitalWrite(MOTOR_A_IN_1_PIN, LOW);
  digitalWrite(MOTOR_A_IN_2_PIN, HIGH);
  }
  // set speed out of possible range 0~255
  analogWrite(MOTOR_A_EN_PIN, speed + MOTOR_A_OFFSET);
  }
  else{
       digitalWrite(MOTOR_A_IN_1_PIN, LOW);
  digitalWrite(MOTOR_A_IN_2_PIN, LOW);

  }
}

void setSpeedMotorB(int speed) {
  if(speed >10){
    if(reverseOn){
        digitalWrite(MOTOR_B_IN_1_PIN, HIGH);
  digitalWrite(MOTOR_B_IN_2_PIN, LOW);
    }
    else{
        digitalWrite(MOTOR_B_IN_1_PIN, LOW);
  digitalWrite(MOTOR_B_IN_2_PIN, HIGH);
    }
      // set speed to 200 out of possible range 0~255
  analogWrite(MOTOR_B_EN_PIN, speed + MOTOR_B_OFFSET);
  }
  else{
  
  digitalWrite(MOTOR_B_IN_1_PIN, LOW);
  digitalWrite(MOTOR_B_IN_2_PIN, LOW);

  }
}

void GoForward(){
  Serial.println("fwd"); 
    reverseOn = false;
  setSpeedMotorA(driveSpeed);
  setSpeedMotorB(driveSpeed);

}

void GoBack(){
    Serial.println("back");
    //Reverse high and low pins for reverse
  reverseOn = true;
    setSpeedMotorA(driveSpeed);
  setSpeedMotorB(driveSpeed);
}

void GoLeft(){
    Serial.println("left");
      setSpeedMotorA(0);
  setSpeedMotorB(driveSpeed);
}

void GoRight(){
    Serial.println("right");
      setSpeedMotorA(driveSpeed);
  setSpeedMotorB(0);
}

void Stop(){
  Serial.println("stop");
  setSpeedMotorA(0);
  setSpeedMotorB(0);

 // digitalWrite(MOTOR_A_IN_1_PIN, LOW);
 // digitalWrite(MOTOR_A_IN_2_PIN, LOW);  
 // digitalWrite(MOTOR_B_IN_1_PIN, LOW);
 // digitalWrite(MOTOR_B_IN_2_PIN, LOW);
}
/////////////////////////SETUP//////////////////////////

void setup() {
  servo1.attach(5);//left
  servo2.attach(6);  //right
  Bluetooth.begin(9600);
  Serial.begin(9600); 
 attackOn = false;
 slowOn = false;
 driveSpeed = 240;//fast
}

void loop() {
  if(Bluetooth.available()){
    btData = Bluetooth.read();
    Serial.println(btData);
    
    switch(btData){
     case 49: //left 1
      GoLeft();
     break;
     case 50: //right 2
      GoRight();
     break;
     case 51: //forward 3
      GoForward();
     break;
     case 52: //back 4
      GoBack();
     break;
     case 53:  //stop 5 - square
      Stop();
     break;
     case 54: //attackOn 6 - triangle
      attackOn = true;
      Serial.println("attack ON");
     break;
     case 55: //attackOff 7 - "X"
      attackOn = false;
      Serial.println("attack OFF");
     break;
     case 56: //stop everything 8 - circle
      attackOn = false;
      Stop();
     break;
     case 57: //slow mode 9 - "select" button
      if(slowOn){
        slowOn = false;
        driveSpeed= 240;
        Serial.println("fast");
      }
      else{
        slowOn = true;
        driveSpeed = 60;
        Serial.println("slow");
      }
     break;
     default:
     Serial.println("wat?");
     servo1.write(5);
     servo2.write(175);
     Stop();
    }
  }
  else{
     //Serial.println("no command"); //for testing
  }

  if(attackOn){
    DoAttack();
  }
  else{
        servo1.write(5);
     servo2.write(175); 
  }

}
