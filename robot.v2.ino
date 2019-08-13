#include <Servo.h>

Servo servo;//Ultrasound Eyes

int pos = 73;//
// Constants for Interrupt Pins
// Change values if not using Arduino Uno
int echoPin = 13;
int trigPin = 12;

const byte MOTOR_A = 3;  // Motor 2 Interrupt Pin - INT 1 - Right Motor
const byte MOTOR_B = 2;  // Motor 1 Interrupt Pin - INT 0 - Left Motor
 
// Constant for steps in disk
const float stepcount = 20.00;  // 20 Slots in disk, change if different

// Constant for wheel diameter
const float wheeldiameter = 66.10; // Wheel diameter in millimeters, change if different

// Integers for pulse counters
volatile int counter_A = 0;
volatile int counter_B = 0;


// Motor A
int enA = 10;
int in1 = 9;
int in2 = 8;

// Motor B
int enB = 5;
int in3 = 6;
int in4 = 7;


float distF=10000.00;
float distL=10000.00;
float distR=10000.00;
// Interrupt Service Routines

//function decleration
int CMtoSteps(float);
void checkDistance(char);
void MoveReverse(int, int);
void MoveForward(int, int);
void SpinRight(int, int);
void SpinLeft(int, int);
void lookForward();
void lookRight();
void lookLeft();
void findRoute();


// Function to convert from centimeters to steps
int CMtoSteps(float cm) {

  int result;  // Final calculation result
  float circumference = (wheeldiameter * 3.14) / 10; // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;  // CM per Step
  
  float f_result = cm / cm_step;  // Calculate result as a float
  result = (int) f_result; // Convert to an integer (note this is NOT rounded)
  
  return result;  // End and return result

}

void checkDistance(char D){
  float duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  if(D =='L'){
    distL = distance;
  }
  else if(D =='R'){
    distR = distance;
  }
  else{
    distF = distance;
  }
}

// Function to Move Reverse
void MoveReverse(int steps, int mspeed) 
{
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   
   // Set Motor A Reverse
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);

   // Set Motor B Reverse
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   
   // Go forward until step value is reached
   while (steps > counter_A && steps > counter_B) {
   
    if (steps > counter_A) {
    analogWrite(enA, mspeed);
    } else {
    analogWrite(enA, 0);
    }
    if (steps > counter_B) {
    analogWrite(enB, mspeed);
    } else {
    analogWrite(enB, 0);
    }
   }
    
  // Stop when done
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 

}

// Function to Move in Forward
void MoveForward(int steps, int mspeed) 
{ 
  Serial.println("Moving Forward");
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   
   // Set Motor A Forward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  


  // Set Motor B Forward
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, mspeed);
  analogWrite(enB, mspeed);
  
   
   // Go in Forward until step value is reached
   while (steps > counter_A && steps > counter_B) {   
      if (steps < counter_A) {      
        analogWrite(enA, 0);
      }
      if (steps < counter_B) {
        analogWrite(enB, 0);
      } 
    }
    
  // Stop when done
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 

  findRoute();

}

// Function to Spin Right
void SpinRight(int steps, int mspeed) 
{
  Serial.println("Spin Right");
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   
   // Set Motor A reverse
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Set Motor B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
   
   // Go until step value is reached
   while (steps > counter_A && steps > counter_B) {
   
    if (steps > counter_A) {
    analogWrite(enA, mspeed);
    } else {
    analogWrite(enA, 0);
    }
    if (steps > counter_B) {
    analogWrite(enB, mspeed);
    } else {
    analogWrite(enB, 0);
    }
   }
    
  // Stop when done
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 

}

// Function to Spin Left
void SpinLeft(int steps, int mspeed) 
{
  Serial.println("Spin Left");
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   
   // Set Motor A forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
   
   // Go until step value is reached
   while (steps > counter_A && steps > counter_B) {
   
    if (steps > counter_A) {
    analogWrite(enA, mspeed);
    } else {
    analogWrite(enA, 0);
    }
    if (steps > counter_B) {
    analogWrite(enB, mspeed);
    } else {
    analogWrite(enB, 0);
    }
  }
    
  // Stop when done
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 

}

void lookForward(){
  
  if(pos > 75){
    for (pos=pos; pos >= 73; pos--) {
      // in steps of 1 degree
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    } 
  }
  else if(pos < 70){
    for (pos=pos; pos <= 73; pos++) { 
      // in steps of 1 degree
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    } 
  } 
  else{
    pos = 73;
    servo.write(pos);  
    delay(300);                // tell servo to go to position in variable 'pos'   
  } 

  checkDistance('F');

}

void lookRight(){
  for (pos=pos; pos >= 20; pos--) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  } 
  checkDistance('R');
  
}
void lookLeft(){
  for (pos=pos; pos <= 135; pos++) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }    
  checkDistance('L');

}

void findRoute(){
   lookForward();
   delay(100);
   Serial.print("Forward Distance ");
   Serial.println(distF);
   if(distF > 30.00 && distF < 1000.00){
      MoveForward(CMtoSteps(100), 200);   
    }
    else{
       lookRight();
       delay(100);
  
       lookLeft();
       delay(100);
  
       if(distR > distL){
         SpinRight(CMtoSteps(50), 150);
        
       }
       else{
         SpinLeft(CMtoSteps(50), 150);
        
       }
       delay(100); 
      findRoute();
     }
}


void setup() 
{
  Serial.begin (9600);
  servo.attach(11);  // attaches the servo on pin 9 to the servo object
  
  Serial.println("Started");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  // Attach the Interrupts to their ISR's
 
  attachInterrupt(digitalPinToInterrupt (MOTOR_A), ISR_countA, RISING);  // Increase counter A when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR_B), ISR_countB, RISING);  // Increase counter B when speed sensor pin goes High
 

  findRoute(); 

//  digitalWrite(in1, LOW);
//  digitalWrite(in2, HIGH);
//  analogWrite(enA, 200);
//  Serial.println("moving wheel A");
//
//  // Set Motor B Forward
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, HIGH);
//  analogWrite(enB, 200);
//  Serial.println("moving wheel B");
  
}

void loop()
{
  
}

