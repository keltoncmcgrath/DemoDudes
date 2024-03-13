#include <Servo.h>
int pin = 23;
Servo arm_servo;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
arm_servo.attach(pin);

}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(arm_servo.read());
arm_servo.write(93);
}
