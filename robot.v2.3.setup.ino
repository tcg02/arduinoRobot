void setup() 
{
  Serial.begin (9600);
  US_Eyes.attach(11);  // attaches the servo on pin 9 to the servo object
  
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
