#include <DualTB9051FTGMotorShieldUnoMega.h>
DualTB9051FTGMotorShieldUnoMega md;
int speed = 200;
void setup() {
  // put your setup code here, to run once:
  md.init();
  md.enableDrivers();
}

void loop() {
  // put your main code here, to run repeatedly:
  md.setSpeeds(speed, speed);
}
