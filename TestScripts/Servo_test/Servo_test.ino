#include <Servo.h>

Servo arm_servo;
Servo shovel_servo;

int arm_pin = 22;
int shovel_pin = 23;
int servo_home = 93;

float t, t_start, shov_t_final, arm_t_final;
int shov_angle_start, shov_angle_final, shov_angle_des;
int arm_angle_start, arm_angle_final, arm_angle_des;
int shov_collect_angle = 120;
int shov_low_dump_angle = 80;
int arm_collect_ange = 80;
int arm_low_dump_angle = 80;

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

void ArmServo(void) {
  t = (millis() - t_start) / 1000;
  if (arm_angle_start > arm_angle_final) {
    arm_angle_des = ((arm_angle_start - arm_angle_final) / (1 + exp((10 / arm_t_final) * t - 5))) + arm_angle_final;
  }
  if (arm_angle_start < arm_angle_final) {
    arm_angle_des = (arm_angle_final - arm_angle_start) / (1 + exp(-(10 / arm_t_final * t) + 5)) + arm_angle_start + 1;
  }
  arm_servo.write(arm_angle_des);
}

void setup() {
  Serial.begin(9600);
  t_start = millis();

  arm_servo.attach(arm_pin);
  shovel_servo.attach(shovel_pin);

  shovel_servo.write(servo_home);
  shov_angle_start = shovel_servo.read();
  shov_t_final = 0.01;
  shov_angle_final = 120;

  arm_servo.write(servo_home);
  arm_angle_start = arm_servo.read();
  arm_t_final = 1;
  arm_angle_final = 20;

  arm_servo.write(25);
  shovel_servo.write(120);
}

void loop() {
  // Serial.print(arm_angle_des);
  // Serial.print('\t');
  // Serial.println(arm_angle_final);

  // ShovelServo();
  // if (shovel_servo.read() >= shov_angle_final) {
  //   shov_angle_start = shovel_servo.read();
  //   shov_angle_final = servo_home;
  // }
  
  // ArmServo();
  // if (arm_servo.read() >= arm_angle_final) {
  //   arm_angle_start = arm_servo.read();
  //   arm_angle_final = servo_home;
  //   t = 0;
  //   t_start = millis();
  // }
}