#include <Servo.h>
Servo arm_servo;
int pin = 23;
int home = 93;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  arm_servo.attach(pin);
  arm_servo.write(120);
  Serial.println("READY...");
  for(int i=1; i<5; i++){
    Serial.println(i);
    delay(1000);
  }
  Serial.println("GO");

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(arm_servo.read());
  arm_servo.write(home);

}
