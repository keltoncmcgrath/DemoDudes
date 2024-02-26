#include <QTRSensors.h>
QTRSensors qtr;
const uint8_t ir_pins[] = {24, 25, 26, 27, 28, 29, 30, 31};
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count];
int ir_bias[ir_sensor_count] = {};

void setup() {
  // put your setup code here, to run once:
  qtr.setTypeRC();
  qtr.setSensorPins(ir_pins, ir_sensor_count);
}

void loop() {
  // put your main code here, to run repeatedly:
  qtr.read(ir_values);
  for(int i=0; i<ir_sensor_count; i++){
    Serial.print(ir_values[i]);
    Serial.print("\t");
  }
  Serial.println();
}
