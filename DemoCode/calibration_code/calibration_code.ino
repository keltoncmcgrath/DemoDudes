// #include <QTRSensors.h>
// QTRSensors qtr;
// const uint8_t ir_pins[] = {24, 25, 26, 27, 28, 29, 30, 31};
// const uint8_t ir_sensor_count = 8;
// uint16_t ir_values[ir_sensor_count];
// int ir_bias[ir_sensor_count] = {};

int dist_pin = A8;
float val;
float d;
float a = exp(7.502420862);
float b = -0.9238074889;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // qtr.setTypeRC();
  // qtr.setSensorPins(ir_pins, ir_sensor_count);
}

void loop() {
  // // IR Array
  // qtr.read(ir_values);
  // for(int i=0; i<ir_sensor_count; i++){
  //   Serial.print(ir_values[i]);
  //   Serial.print("\t");
  // }
  // Serial.println();

  // Range Finder
  val = analogRead(dist_pin);
  d = pow(val/a, 1/b);
  Serial.println(d);
}
