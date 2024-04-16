#include <Servo.h>

Servo arm_servo;
Servo shovel_servo;

int arm_pin = 22;
int shovel_pin = 23;
int home = 93;

float t, t_start, shov_t_final;
int shov_angle_start, shov_angle_final, shov_angle_des;
int shov_collect_angle = 120;
int shov_low_dump_angle = 80;

float t_begin, t_done;
bool first_time = true;

void ShovelServo(void) {
  t = (millis() - t_start) / 1000;
  if (shov_angle_start > shov_angle_final) {
    shov_angle_des = ((shov_angle_start - shov_angle_final) / (1 + exp((10 / shov_t_final) * t - 5))) + shov_angle_final;
  }
  if (shov_angle_start < shov_angle_final) {
    shov_angle_des = (shov_angle_final - shov_angle_start) / (1 + exp(-(10 / shov_t_final * t) + 5)) + shov_angle_start + 1;
  }
  shovel_servo.write(shov_angle_des);
}

void setup() {
  Serial.begin(9600);
  t_start = millis();

  arm_servo.attach(arm_pin);
  shovel_servo.attach(shovel_pin);

  shovel_servo.write(shov_low_dump_angle);
  delay(1000);
  shov_angle_start = shovel_servo.read();
  shov_t_final = 0.01;
  shov_angle_final = shov_collect_angle;
  delay(1000);
  Serial.println("Start...");
  t_begin = millis();
}

void loop() {
  ShovelServo();
  if(shovel_servo.read() >= shov_angle_final && first_time) {
    t_done = millis();
    Serial.print("Done! Finished in: ");
    Serial.print('\t');
    Serial.println((t_done-t_begin)/1000);
    first_time = false;
  }
  // arm_servo.write(93);
}
