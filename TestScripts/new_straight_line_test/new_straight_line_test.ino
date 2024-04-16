#include <DualTB9051FTGMotorShieldUnoMega.h>
#include <Encoder.h>
#include <QTRSensors.h>

DualTB9051FTGMotorShieldUnoMega md;
QTRSensors qtr;

int encoder1_pinA = 19;
int encoder1_pinB = 18;
int encoder2_pinA = 20;
int encoder2_pinB = 21;
Encoder encoder1(encoder1_pinA, encoder1_pinB);
Encoder encoder2(encoder2_pinA, encoder2_pinB);
int counts1, counts2;
int theta1, theta2;
int m1s, m2s;
int line_speed = 300;
float KP = 95.7;
float KD = 1.78;
float KI = 1.78;
int counts_per_rev = 64;
int gear_ratio = 131;
float t, t_start, delta_T, t_old;
int error1, d_error1, integral_error1;

const uint8_t ir_pins[] = { 24, 25, 26, 28, 27, 29, 30, 31 };
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count] = { 0, 0, 0, 0, 0, 0, 0, 0 };

bool go = true;
bool second_line;
bool last_line_state;
bool new_action = false;
char face = 'e';

void StraightRange(void) {
  // Update varaibles
  t = (millis() - t_start) / 1000;
  delta_T = t - t_old;
  counts1 = encoder1.read();
  counts2 = encoder2.read();

  // Read Encoders
  theta1 = float(counts1 * 2 * PI) / (gear_ratio * counts_per_rev);
  theta2 = float(counts2 * 2 * PI) / (gear_ratio * counts_per_rev);

  // Compute Errors
  error1 = theta1 - theta2;
  d_error1 = error1 / delta_T;
  integral_error1 = error1 * delta_T;
  integral_error1 = constrain (integral_error1, -400/KI, 400/KI);

  // Control speed and
  m1s = line_speed - KP*error1 - KD*d_error1 - KI*integral_error1;
  m2s = line_speed + KP*error1 + KD*d_error1 + KI*integral_error1;
  m1s = constrain(m1s, -400, 400);
  m2s = constrain(m2s, -400, 400);
  md.setSpeeds(m1s, m2s);

  // Reset Vars
  t_old = t;
}

void setup() {
  Serial.begin(9600);

  md.init();
  md.enableDrivers();

  qtr.setTypeRC();
  qtr.setSensorPins(ir_pins, ir_sensor_count);

  t_start = millis();
}

void loop() {
//   if(new_action){
//     last_line_state = true;
//     new_action = false;
//   }
//   Serial.println(last_line_state);
//   if (go) {
//     StraightRange();
//   }

//   qtr.read(ir_values);
//   for (int i = 0; i < ir_sensor_count; i++) {
//     if (ir_values[i] < 500) {
//       if(!last_line_state){
//         second_line = true;
//       }
//       last_line_state = true;
//       go = true;
//       goto end_of_case;
//     }
//   }

//   last_line_state = false;
//   if (face == 'e' && !second_line) {
//     goto end_of_case;
//   }
//   Serial.println("Black Line");
//   md.setSpeeds(0, 0);
//   go = false;
//   return;

// end_of_case:

//   Serial.println("No Line");

  StraightRange();
}
