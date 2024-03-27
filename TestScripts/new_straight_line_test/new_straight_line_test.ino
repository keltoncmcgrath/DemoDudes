#include <DualTB9051FTGMotorShieldUnoMega.h>
#include <Encoder.h>

DualTB9051FTGMotorShieldUnoMega md;

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

void StraightLineDist(void) {
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
}

void loop() {
  // put your main code here, to run repeatedly:
  StraightLineDist();
}
