#include<Servo.h>
#include<AFMotor.h>
#define LEFT A4
#define echopin A1 // echo pin
#define trigpin A2 // Trigger pin
#define RIGHT A5

AF_DCMotor Motor1(1,MOTOR12_1KHZ);   // front left wheel
AF_DCMotor Motor2(2,MOTOR12_1KHZ);   // rear left wheel
AF_DCMotor Motor3(3,MOTOR12_1KHZ);   // rear right wheel
AF_DCMotor Motor4(4,MOTOR12_1KHZ);   // front right wheel
Servo myservo;
 
int pos = 0;
long time;

void setup(){

Serial.begin(9600);
myservo.attach(10);

for(pos = 90; pos <= 180; pos += 1){
myservo.write(pos);
delay(15);
} 
 
for(pos = 180; pos >= 0; pos-= 1) {
myservo.write(pos);
delay(15);
}

for(pos = 0; pos<=90; pos += 1) {
myservo.write(pos);
delay(15);
}

pinMode(RIGHT, INPUT);
pinMode(LEFT, INPUT);

pinMode(trigpin, OUTPUT);
pinMode(echopin, INPUT);

}

void loop(){
unsigned int distance = read_cm();

int Right_Value = digitalRead(RIGHT);
int Left_Value  = digitalRead(LEFT);

Serial.print("R= ");
Serial.print(Right_Value);
Serial.print(" L= ");
Serial.print(Left_Value);
Serial.print(" D= ");
Serial.println(distance);


if(distance<=30){
  if(checkRight(distance) && checkLeft(distance)){ // if objects are on both sides
    backward();
  } else if(checkRight(distance) && !checkLeft(distance)){ // if an object is on the right
    turnLeft(distance);
  } else if(!checkRight(distance) && checkLeft(distance)){ // if an object is on the left
    turnRight(distance);
  }
} 

delay(50);
  checkLeft(distance);
  checkRight(distance);
}

long read_cm(){
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  time = pulseIn (echopin, HIGH);
  return time / 29 / 2;
}

bool forward(unsigned int distance){
  if (distance >= 30) {
    Serial.println("forward");
    Motor1.setSpeed(200);
    Motor1.run(FORWARD);
    Motor2.setSpeed(200);
    Motor2.run(FORWARD);
    Motor3.setSpeed(200);
    Motor3.run(BACKWARD);
    Motor4.setSpeed(200);
    Motor4.run(FORWARD);
    return true;
  } 
  return false;
}

bool checkLeft(unsigned int distance){
  for(pos = 90; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
  if (distance >= 30){
    for(pos = 0; pos >= 90; pos += 1){
      myservo.write(pos);
      delay(15);
      }
    return false;
    }
  return true;
}


bool checkRight(unsigned int distance){
  for(pos = 90; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  if (distance >= 30){
    for(pos = 180; pos >= 90; pos -= 1){
      myservo.write(pos);
      delay(15);
      }
    return false;
    }
  return true;
}


bool turnLeft(unsigned int distance){
  Serial.println("left");
  Motor1.setSpeed(100);
  Motor1.run(BACKWARD);
  Motor3.setSpeed(200);
  Motor3.run(FORWARD);
  if (distance >= 30) {
    return false;  
  }
  return true;
}

bool turnRight(unsigned int distance){
  Serial.println("right");
  Motor2.setSpeed(200);
  Motor2.run(FORWARD);
  Motor4.setSpeed(100);
  Motor4.run(BACKWARD);
  if (distance >= 30) {
    return false;  
  }
  return true;
}

void backward(){
Motor1.setSpeed(120);
Motor1.run(BACKWARD); 
Motor2.setSpeed(120);
Motor2.run(BACKWARD);
Motor3.setSpeed(120);
Motor3.run(BACKWARD); 
Motor4.setSpeed(120);
Motor4.run(BACKWARD); 
}

void stop(){
Motor1.setSpeed(0);  
Motor1.run(RELEASE);
Motor2.setSpeed(0);
Motor2.run(RELEASE);
Motor3.setSpeed(0);
Motor3.run(RELEASE);
Motor4.setSpeed(0);
Motor4.run(RELEASE);  
}
