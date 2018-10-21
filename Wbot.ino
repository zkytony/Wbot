#include <Servo.h>

int pinMotor = 9;
int pinServo = 10;
int pinDistTrig = 7;
int pinDistEcho = 8;

Servo servo;
long val;
bool positive;
int angle;
int angle_increment;
int total_seconds;
int current_period_start;
int current_time;
int time_accumulated;
float elapsed_seconds;
int sensitive_range;
int stop_range;
int motor_speed;

void setup() {
	Serial.begin(9600);

	pinMode(pinMotor, OUTPUT);
	pinMode(pinDistTrig, OUTPUT);
	pinMode(pinDistEcho, INPUT);
	servo.attach(pinServo);

	total_seconds = 10;  // how long does it take to finish laundry.
  angle_increment = 180 / total_seconds;
	angle = 0;

  current_period_start = 0;
  time_accumulated = 0;
  elapsed_seconds = 0.0;
  current_time = 0;
  sensitive_range = 2800;
  stop_range = 200;
  motor_speed = 0;
  
	servo.write(angle);
	delay(15);
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
  Serial.println("distance sensor");
  Serial.println(val);
  //val = val/148*2.54;
  
  current_time = millis();
	
  //Serial.println("distance sensor");
  //Serial.println(val);
	if (elapsed_seconds < total_seconds) {
		
		if (val >= stop_range) {
      // 255 is actually the lowest speed. 0 is highest.
      motor_speed = 255 - min(255, (int)(((float)min(sensitive_range, val) / (float)(sensitive_range - stop_range)) * 255));
      Serial.println("Motor speed:");
      Serial.println(motor_speed);
			analogWrite(pinMotor, motor_speed);
     
      if (current_period_start == 0) {
        current_period_start = current_time;
      }
      elapsed_seconds = (time_accumulated + (current_time - current_period_start)) / 1000.0;
      Serial.println("elapsed seconds");
      Serial.println(elapsed_seconds);

			angle = (int)(elapsed_seconds * angle_increment);
			servo.write(angle);
      Serial.println("angle:");
      Serial.println(angle);
			delay(15);
		} else {
      if (current_period_start != 0) {
        time_accumulated += current_time - current_period_start;
      }
      current_period_start = 0;
			Serial.println("Stop");
			servo.write(angle);  
			analogWrite(pinMotor, 255);
			delay(15);
		}
    Serial.println();
	} else {
      Serial.println("Finished");
      angle = 0;
      servo.write(angle);
      delay(15);
      analogWrite(pinMotor, 255);
	}
  delay(100);
}
