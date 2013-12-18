// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myservo[4];  // create servo objects to control a servo 
 
int potpin = 6;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin 
 
void setup() 
{ 
  myservo[0].attach(2);  // attaches the servo on pin 2 to the servo object (Servo Output 1 of Servo Shield)
  myservo[1].attach(3);  // attaches the servo on pin 3 to the servo object (Servo Output 2 of Servo Shield)
  myservo[2].attach(4);  // attaches the servo on pin 4 to the servo object (Servo Output 3 of Servo Shield) 
  myservo[3].attach(5);  // attaches the servo on pin 5 to the servo object (Servo Output 4 of Servo Shield) 
} 
 
void loop() 
{ 
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo[0].write(val);               // sets the servo 1 position according to the scaled value 
  myservo[1].write(val);               // sets the servo 2 position according to the scaled value 
  myservo[2].write(val);               // sets the servo 3 position according to the scaled value 
  myservo[3].write(val);               // sets the servo 4 position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
} 
