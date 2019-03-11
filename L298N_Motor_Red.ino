//References used during development
//https://tronixlabs.com.au/news/tutorial-l298n-dual-motor-controller-module-2a-and-arduino/
//https://electronicshobbyists.com/controlling-dc-motors-arduino-arduino-l298n-tutorial
//https://www.instructables.com/id/How-to-use-a-Buzzer-Arduino-Tutorial/
//https://www.ardumotive.com/how-to-use-a-buzzer-en.html
//https://drive.google.com/drive/folders/1g8IpnBzTWHh9URlSGvrl_ZXt8bjpqOH6

//https://docs.google.com/forms/d/e/1FAIpQLScgTbXMsu3gyGOLAdaBz9trJNfgeqCEC447UC1mxxNpGpEqcQ/viewform

/*UQ Robotics Hackathon March 2019 The University of Queensland
 * Theme: Transport
 * 
 * Team: A-Team
 * 
 * Lynn Shim
 * Pranshu Kulkarni
 * Duke Wu
 * Chanon Kachorn
 * Juichi Nishimura
 * 
 * Project idea: 
 *    Robotic guide dog for people with partial vision impairment.
 *    Using onboard ultrasonic sensor it would detect distance to obstacle in front and
 *    produce warning sounds and LED lights (Red - stop, Yellow - warning, Green - safe)
 *  
 *  Hardware:
 *    Arduino
 *    Ultrasonic sensor
 *    Buzzer
 *    L298n motor controller (Red)
 *    x2 motor (wheel)
 *    x3 LEDs
 *    Servo (tail)
 *    
 *  Pins Setup:
 *    2 - LED Green       9 - Motor 2
 *    3 - LED Yellow      0 - Motor 2 enabler
 *    4 - LED Red         11 - Buzzer
 *    5 - Motor enabler   12 - Servo
 *    6 - Motor           13 - Motor controller (Red)
 *    7 - Motor           A0 - Sensor Trigger
 *    8 - Motor 2         A1 - Sensor Echo
 */

#include <Servo.h>

// motor one pins
int enA = 0; ///turn motor on/off
int in1 = 9;
int in2 = 8;
// motor two pins
int enB = 5;
int in3 = 7;
int in4 = 6;

//buzzer
const int buzzer = 11;

//Ultrasonic Sensor using analog input pins as IO
#define trigPin A0
#define echoPin A1
long duration; // time difference between emitting an ultrasonic pulse and receiving its echo
int distance;  // distance between sensor and the object

//LED pins
const int red = 4;
const int yellow = 3;
const int green = 2;

//Tail setup and position
Servo myservo;
int pos = 0;

int speed = 255;
/*
 * Set up all pins for IO. Run once.
 */
void setup()
{
  // set all the motor control pins to Outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // sets the buzzer pin as Output
  pinMode(buzzer, OUTPUT);

  // sets the trigPin as an Output
  pinMode(trigPin, OUTPUT);

  // sets the echoPin as an Input
  pinMode(echoPin, INPUT);

  // set all the IED pins to Outputs
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  // sets servo pin
  myservo.attach(12);

  // starts the serial communication for sensor
  Serial.begin(9600);
}


/*
 * Move the robot forward (or backwards) at a constant speed.
 * If HIGH and LOW were swap, the robot would go the other way.
 * Speed is in range 0 - 255.
 */
void move()
{
  // turn on motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, speed);
  
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, speed);
}

/*
 * Wiggle the tail to mimic a dog behaviour. Moving the servo between 0 - 180
 * degrees.
 */
void tail_wag() {
  for (pos = 0; pos <= 60; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  /*for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }*/
}

/*
 * Check the distance between the robot and the obstacle in front. Gets called every 3
 * seconds.
 */
void check_distance() {

  //For sensor
  // clears the trigger pin for 2us by setting it to LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // sets the trigger pin on HIGH state for 10us to send out an ultrasonic burst
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  // sets the trigger pin back to LOW
  digitalWrite(trigPin, LOW);

  // reads the echo pin, returns the sound wave travel time in us
  duration = pulseIn(echoPin, HIGH);

  // calculating the distance
  distance = duration * 0.034 / 2;

  // Prints the distance in cm on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  clear_lights(); //Clear all current lightings
  noTone(buzzer); //Clear all current noise
  set_lights();
  
}

/*
 * Light up the appropriate LED depending on the distance of obstacle.
 * Red: danger close, sound a high frequency noise
 * Yellow: warning, sound a low frequency noise
 * Green: safe
 */
void set_lights() {
  //Display warning lights according to distance to obstacle
  if (distance <= 70) {
    digitalWrite(red, HIGH);
    tone(buzzer, 500);
    speed = 0; //Stop the robot
  } else if (distance > 70 && distance <= 250) {
    digitalWrite(yellow, HIGH);
    //speed = 50;
    tone(buzzer, 1000);
  } else {
    digitalWrite(green, HIGH);
    speed = 255; //Move forward again
  }
}

/*
 * clear all lightinings for the next distance read.
 */
void clear_lights() {
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
}

/*
 * Main program/control of the robot.
 */
void loop()
{
  move();
  tail_wag();
  check_distance();
  delay(500); //Delay 2 seconds before reading the distance again
}
