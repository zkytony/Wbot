#include <Servo.h>

int pinMotorGate = 9;
int pinServo = 10;
int pinDistTrig = 7;
int pinDistEcho = 8;

Servo servo;
int angle;
long val;
bool positive;

void setup() {
	Serial.begin(9600);

	pinMode(pinMotorGate, OUTPUT);
	pinMode(pinDistTrig, OUTPUT);
	pinMode(pinDistEcho, INPUT);
	servo.attach(pinServo);

  angle = 0;
  positive = true;
}

void loop() {
	// Spin the motor. Move the servo back and forth.
	// Stop everything when object is near the distance
	// sensor.

  //sends trigger pulse to trigger pin to start sensor into ranging mode
  digitalWrite(pinDistTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinDistTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinDistTrig, LOW);
	val = pulseIn(pinDistEcho, HIGH);
  val = val/148*2.54;

	Serial.println(val);
	if (val >= 100) {
		analogWrite(pinMotorGate, 10);

    //Serial.println(angle);
    servo.write(angle);
    delay(15);
    if (positive) {
      angle += 1;
    } else {
      angle -= 1;
    }

    if (angle == 180) {
      positive = false;
    } else if (angle == 0) {
      positive = true;
    } 
	} else {
    Serial.println("Stop");
    servo.write(angle);  
		analogWrite(pinMotorGate, 0);
    delay(15);
	}
  delay(100);
}
