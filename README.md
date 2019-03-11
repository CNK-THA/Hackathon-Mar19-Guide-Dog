# Hackathon-Mar19-Guide-Dog

UQ Robotic Hackathon The University of Queensland

Robotic guide dog for people with partial visual impairment.
  Using onboard ultrasonic sensor to detect distance of obstacle in front and
  produce warning sounds and LED lights (Red - stop, Yellow - warning, Green - safe)

Hardware:
Arduino
Ultrasonic sensor
Buzzer
L298n motor controller (Red)
x2 motor (wheel)
x3 LEDs
Servo (tail)
    
Arduino Pins Setup:
2 - LED Green       9 - Motor 2
3 - LED Yellow      0 - Motor 2 enabler
4 - LED Red         11 - Buzzer
5 - Motor enabler   12 - Servo
6 - Motor           13 - Motor controller (Red)
7 - Motor           A0 - Sensor Trigger
8 - Motor 2         A1 - Sensor Echo
