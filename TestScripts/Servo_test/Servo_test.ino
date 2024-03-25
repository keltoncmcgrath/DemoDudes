#include <Servo.h>
Servo arm_servo;
Servo shovel_servo;
int arm_pin = 22;
int shovel_pin = 23;
int home = 93;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  arm_servo.attach(arm_pin);
  shovel_servo.attach(shovel_pin);
}

void loop() {
  // put your main code here, to run repeatedly:
  shovel_servo.write(75);

}
