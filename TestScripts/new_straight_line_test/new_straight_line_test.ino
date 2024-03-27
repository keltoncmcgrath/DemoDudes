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
float KP = 125;
int counts_per_rev = 64;
int gear_ratio = 131;

const uint8_t ir_pins[] = {24, 25, 26, 27, 28, 29, 30, 31};
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count] = {0, 0, 0, 0, 0, 0, 0, 0};

bool go = true;

void StraightRange(void) {
  // Update varaibles
  counts1 = encoder1.read();
  counts2 = encoder2.read();
  theta1 = float(counts1 * 2 * PI) / (gear_ratio * counts_per_rev);
  theta2 = float(counts2 * 2 * PI) / (gear_ratio * counts_per_rev);

  // Control speed and
  m1s = line_speed - KP*(theta1-theta2);
  m2s = line_speed - KP*(theta2-theta1);
  m1s = constrain(m1s, -400, 400);
  m2s = constrain(m2s, -400, 400);
  md.setSpeeds(m1s, m2s);
}

void setup() {
  Serial.begin(9600);

  md.init();
  md.enableDrivers();

  qtr.setTypeRC();
  qtr.setSensorPins(ir_pins, ir_sensor_count);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(go){
    StraightRange();
  }
  qtr.read(ir_values);
  for (int i = 0; i < ir_sensor_count; i++) {
    if(ir_values[i] < 2000){
      go = true;
      goto end_of_case;
    }
  }
  Serial.println("Black Line");
  md.setSpeeds(0, 0);
  go = false;
  return;
  end_of_case:
    Serial.println("No Line");
}
