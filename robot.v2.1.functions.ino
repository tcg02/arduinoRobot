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
    for (pos=pos; pos >= 73; pos--) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      US_Eyes.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    } 
  }
  else if(pos < 70){
    for (pos=pos; pos <= 73; pos++) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      US_Eyes.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    } 
  } 
  else{
    pos = 73;
    US_Eyes.write(pos);              // tell servo to go to position in variable 'pos'   
  } 

  checkDistance('F');

}

void lookRight(){
  for (pos=pos; pos >= 20; pos--) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    US_Eyes.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  } 
  checkDistance('R');
  
}
void lookLeft(){
  for (pos=pos; pos <= 135; pos++) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    US_Eyes.write(pos);              // tell servo to go to position in variable 'pos'
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
