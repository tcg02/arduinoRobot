#include <Servo.h>

Servo US_Eyes;//Ultrasound Eyes

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
