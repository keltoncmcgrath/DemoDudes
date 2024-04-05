#include <DualTB9051FTGMotorShieldUnoMega.h>
#include <Encoder.h>
#include <QTRSensors.h>

DualTB9051FTGMotorShieldUnoMega md;
QTRSensors qtr;

int encoder1_pinA = 19;
int encoder1_pinB = 18;
int encoder2_pinA = 20;
int encoder2_pinB = 21;
int encoder1_count;
int encoder2_count;
Encoder encoder1(encoder1_pinA, encoder1_pinB);
Encoder encoder2(encoder2_pinA, encoder2_pinB);

float t;
float t_old;
float t_start;
float delta_T;
float pi = 3.1415927;
char state = 'a';
char last_state;

// Line Following Vars
int kp = 200;
int ki = 500;
int kd = 4;
float ir_error, ir_error_last, ir_d_error, ir_integral_error;
int ir_bias[] = { 140, 94, 128, 134, 140, 140, 140, 140 };
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count];
int ir_unbiased[ir_sensor_count];
float ir_sensor_spacing[] = { 0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6 };  // cm
float ir_dist_desired = 2.8;                                           // cm
float ir_dist_actual;
bool all_black;
float num = 0;
float den = 0;
int line_speed = 300;
int m1s, m2s;
const uint8_t ir_pins[] = { 24, 25, 26, 27, 28, 29, 30, 31 };

void LineFollow(void) {
  // Read sensor values and take bias off
  t = (millis()-t_start) / 1000;
  delta_T = t - t_start;
  qtr.read(ir_values);
  for (int i = 0; i < ir_sensor_count; i++) {
    ir_unbiased[i] = ir_values[i] - ir_bias[i];
  }

  // Calculate distance of line from deisred distance
  for (int i = 0; i < ir_sensor_count; i++) {
    num = num + (ir_unbiased[i] * ir_sensor_spacing[i]);
    den = den + ir_unbiased[i];
  }
  ir_dist_actual = num / den;
  num = 0;
  den = 0;

  // Calculate error and control speed
  ir_error = ir_dist_desired - ir_dist_actual;
  ir_d_error = (ir_error - ir_error_last) / delta_T;
  ir_integral_error += ir_integral_error * delta_T;
  ir_integral_error = constrain(ir_integral_error, -400/ki, 400/ki);
  // m1s = line_follow_speed - kp * ir_error;
  // m2s = line_follow_speed + kp * ir_error;
  m1s = line_speed - kp*ir_error - kd*ir_d_error - ki*ir_integral_error;
  m2s = line_speed + kp*ir_error + kd*ir_d_error + ki*ir_integral_error;
  md.setSpeeds(m1s, m2s);
  ir_error_last = ir_error;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  md.init();
  md.enableDrivers();
  qtr.setTypeRC();
  qtr.setSensorPins(ir_pins, ir_sensor_count);

  t_start = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(m1s);
  LineFollow();
}
