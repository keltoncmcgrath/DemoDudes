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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  md.init();
  md.enableDrivers();
}

void loop() {
  // put your main code here, to run repeatedly:
  counts1 = encoder1.read();
  counts2 = encoder2.read();
  Serial.print(counts1);
  Serial.print('\t');
  Serial.println(counts2);
}
