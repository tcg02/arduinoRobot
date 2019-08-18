#include <Servo.h>  

Servo servo;//Ultrasound Eyes

int pos = 73;//
// Constants for Interrupt Pins
// Change values if not using Arduino Uno
int echoPin = 13;
int trigPin = 12;
 
volatile int findRouteReqCount = 0;
 

const byte MOTOR_A = 3;  // Motor 2 Interrupt Pin - INT 1 - Right Motor
const byte MOTOR_B = 2;  // Motor 1 Interrupt Pin - INT 0 - Left Motor
 
// Constant for steps in disk
const float stepcount = 20.00;  // 20 Slots in disk, change if different

// Constant for wheel diameter
const float wheeldiameter = 66.10; // Wheel diameter in millimeters, change if different

// Integers for pulse counters
volatile int counter_A = 0;
volatile int counter_B = 0;
volatile int counter_A_calib= 0; 
volatile int counter_B_calib = 0;
volatile int speed_A=0;
volatile int speed_B=0;


// Motor A
int enA = 11;
int in1 = 10;
int in2 = 9;
 
// Motor B
int enB = 6;
int in3 = 7;
int in4 = 8;


float distF=10000.00;
float distL=10000.00;
float distR=10000.00;
// Interrupt Service Routines

 
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
  distance = duration / 58.2; //distance in cm
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

void driveStright(int mspeed){

   //Serial.print("Counter A = ");Serial.print(counter_A); Serial.print("   Counter B = ");Serial.print(counter_B); Serial.print ("   Motor A Speed = "); Serial.print(speed_A);Serial.print ("   Motor B Speed = "); Serial.println(speed_B);
    //Uncomment this to use for csv file//Serial.print(counter_A); Serial.print(",");Serial.print(counter_B); Serial.print (","); Serial.print(speed_A);Serial.print (","); Serial.println(speed_B);
  if(counter_A_calib != counter_B_calib){ 
      if(counter_A_calib > counter_B_calib){
        if(speed_B<230){
          speed_B+=15;
        }else{speed_B = 255;}
  
        analogWrite(enB, speed_B); 
      }
      else if(counter_A_calib < counter_B_calib){
          speed_B-=15; 
          analogWrite(enB, speed_B);             
      }
      else{
          speed_B = mspeed;
          analogWrite(enB, speed_B); 
      }
  }
  counter_A_calib = 0;  //  reset counter A to zero
  counter_B_calib = 0;  //  reset counter B to zero   
    
}


// Function to Move in Forward
void MoveForward(int steps, int mspeed) 
{ 
  findRouteReqCount = 0;
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
  
   int currentStep = 0;
   // Go in Forward until step value is reached
   while (steps > counter_A && steps > counter_B) { 
     if(currentStep%10==0){
        driveStright(mspeed);
      }  
      if (steps < counter_A) {      
        analogWrite(enA, 0);
      }
      if (steps < counter_B) {
        analogWrite(enB, 0);
      } 
      currentStep++;
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
  Serial.print("Forward Distance"); Serial.println(distF);
}

void lookRight(){
  for (pos=pos; pos >= 20; pos--) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  } 
  checkDistance('R');
  Serial.print("Right Distance"); Serial.println(distR);
}
void lookLeft(){
  for (pos=pos; pos <= 135; pos++) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }    
  checkDistance('L');
  Serial.print("Left Distance"); Serial.println(distL);
}

void performScan(){
  lookRight();
  delay(10);
  lookForward(); 
  delay(10);
  lookLeft();  
  delay(10);  
  lookForward(); 
  delay(10);
}

void findRoute(){
  findRouteReqCount++;
    performScan();
    delay(200);
 
   if(distF > 2000 || distF < 20){    
        if(distR > distL){
          SpinRight(CMtoSteps(50), 150);      
        }
        else{
          SpinLeft(CMtoSteps(50), 150);        
        } 

        if(findRouteReqCount <= 3 ){
          findRoute();
        }else{
          for(int i=0;i <= 20 ;i++){
            digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
            delay(500);                       // wait for a second
            digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
            delay(500); 
          }
          findRoute();
        }
             
       
    }else{  
      float togo = distF * 0.9; //Lets go 2/3rd distance as seen by US sensor
      Serial.print("Moving Forward Distance "); Serial.println(togo);
      MoveForward(CMtoSteps(togo), 200); 
    }   
}

// Motor A pulse count ISR
void ISR_countA() {
  counter_A++;  // increment Motor A counter value
  counter_A_calib++;
} 

// Motor B pulse count ISR
void ISR_countB() {
  counter_B++;  // increment Motor B counter value
  counter_B_calib++;
}


void setup() 
{
  Serial.begin (9600);
  servo.attach(5);  // attaches the servo on pin 5 to the servo object
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  
  attachInterrupt(digitalPinToInterrupt (MOTOR_A), ISR_countA, RISING);  // Increase counter A when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR_B), ISR_countB, RISING);  // Increase counter B when speed sensor pin goes High

  findRoute();
  
}

void loop()
{
 
}
