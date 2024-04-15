#include <Encoder.h>
#include <DualTB9051FTGMotorShieldUnoMega.h>
DualTB9051FTGMotorShieldUnoMega md;

int encoder1_pinA = 19;
int encoder1_pinB = 18;
int encoder2_pinA = 20;
int encoder2_pinB = 21;
Encoder encoder1(encoder1_pinA, encoder1_pinB);
Encoder encoder2(encoder2_pinA, encoder2_pinB);

int counts1;
int counts2;
int current1, current2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  md.init();
  md.enableDrivers();
  md.setSpeeds(400, 400);
}

void loop() {
  // put your main code here, to run repeatedly:
  counts1 = encoder1.read();
  counts2 = encoder2.read();
  Serial.print(counts1);
  Serial.print('\t');
  Serial.println(counts2);
  current1 = 0.1*md.getM1CurrentMilliamps() + 0.9*current1;
  current2 = 0.1*md.getM2CurrentMilliamps() + 0.9*current2;
  // Serial.print(current1);
  // Serial.print('\t');
  // Serial.println(current2);
}
