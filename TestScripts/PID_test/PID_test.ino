#include <DualTB9051FTGMotorShieldUnoMega.h>
#include <Encoder.h>
DualTB9051FTGMotorShieldUnoMega md;

int encoder1_pinA = 19;
int encoder1_pinB = 18;
int encoder2_pinA = 20;
int encoder2_pinB = 21;

float t;
float t_old;
float t_start;
float delta_T;

// Control Vars
float KP = 95.7;
// float KP = 80;
float KI = 900;
float KD = 1.78;
// float KD = 3;
float error1, d_error1, integral_error1;
float error2, d_error2, integral_error2;
// int straight_kp = 125;
int m1s, m2s;
float V1, V2;

// Travel Vars
float dist_traveled;
float dist_final;
float turn_angle_final;        // rad
float arc_radius;              // cm
float arc_angle_final;         // rad
float time_final;
float theta1, theta1_des, theta1_desf, theta1_final, theta1_old;
float theta2, theta2_des, theta2_desf, theta2_final, theta2_old;
float omega1, omega1_des;
float omega2, omega2_des;
long counts1;
long counts2;
float alpha = 0.1;

// Travel Constants
int counts_per_rev = 64;
int gear_ratio = 131;
float wheel_radius = 4.2;      // cm
float wheel_dist = 18.7;       // cm
float turn_time = 1.5;         // s

// Encoder Vars
int encoder1_count;
int encoder2_count;
Encoder encoder1(encoder1_pinA, encoder1_pinB);
Encoder encoder2(encoder2_pinA, encoder2_pinB);

bool drive = true;

void TimedDrive(void) {
  // Update varaibles
  t = (millis() - t_start) / 1000;
  delta_T = t - t_old;
  counts1 = encoder1.read();
  counts2 = encoder2.read();
  theta1 = float(counts1 * 2 * PI) / (gear_ratio * counts_per_rev);
  theta2 = float(counts2 * 2 * PI) / (gear_ratio * counts_per_rev);

  // Calculate new desired thetas
  theta1_des += omega1_des * delta_T;
  theta2_des += omega2_des * delta_T;
  if (abs(theta1_des) >= abs(theta1_final)) {
    theta1_des = theta1_final;
  }
  if (abs(theta2_des) >= abs(theta2_final)) {
    theta2_des = theta2_final;
  }
  theta1_desf = alpha*theta1_des + (1-alpha)*theta1_desf;
  theta2_desf = alpha*theta2_des + (1-alpha)*theta2_desf;

  // Calculate omega
  omega1 = (theta1 - theta1_old) / delta_T;
  omega2 = (theta2 - theta2_old) / delta_T;

  // Compute errors
  error1 = theta1_desf - theta1;
  d_error1 = omega1_des - omega1;
  integral_error1 += error1 * delta_T;
  integral_error1 = constrain(integral_error1, -10. / KI, 10. / KI);
  error2 = theta2_desf - theta2;
  d_error2 = omega2_des - omega2;
  integral_error2 += error2 * delta_T;
  integral_error2 = constrain(integral_error2, -10. / KI, 10. / KI);
  

  // Control laws
  V1 = KP * error1 + KD * d_error1 + KI * integral_error1;
  V2 = KP * error2 + KD * d_error2 + KI * integral_error2;
  m1s = 400 * V1 / 10;
  m2s = 400 * V2 / 10;

  // Set Speed
  m1s = constrain(m1s, -400, 400);
  m2s = constrain(m2s, -400, 400);
  md.setSpeeds(m1s, m2s);

  // Reset vars
  t_old = t;
  theta1_old = theta1;
  theta2_old = theta2;
}

void setup() {
  Serial.begin(9600);
  md.init();
  md.enableDrivers();
  // // Turn
  turn_angle_final = -PI;
  time_final = turn_time;
  theta1_final = turn_angle_final * (wheel_dist / 2) / wheel_radius;
  theta2_final = -turn_angle_final * (wheel_dist / 2) / wheel_radius;

  //Striaght
  // dist_final = 15;
  // time_final = 3;
  // theta1_final = dist_final / wheel_radius;
  // theta2_final = dist_final / wheel_radius;

  t_start = millis();
  t_old = 0;
  counts1 = 0;
  counts2 = 0;
  theta1_des = 0;
  theta2_des = 0;
  encoder1.write(0);
  encoder2.write(0);
  omega1_des = theta1_final / time_final;
  omega2_des = theta2_final / time_final;
}

void loop() {
  if(drive){
      TimedDrive();
      Serial.print(theta1);
      Serial.print('\t');
      Serial.print(theta1_des);
      Serial.print('\t');
      Serial.print(theta2);
      Serial.print('\t');
      Serial.println(theta2_des);
  }
  if(abs(theta1)>=abs(theta1_final) && abs(theta2) >= abs(theta2_final) && md.getM1CurrentMilliamps() == 0 && md.getM2CurrentMilliamps() == 0){
    md.setSpeeds(0, 0);
    drive = false;
  }

}
