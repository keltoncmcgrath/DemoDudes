// #include <QTRSensors.h>
// QTRSensors qtr;
// const uint8_t ir_pins[] = {24, 25, 26, 27, 28, 29, 30, 31};
// const uint8_t ir_sensor_count = 8;
// uint16_t ir_values[ir_sensor_count];
// int ir_bias[ir_sensor_count] = {};

int dist_pin = A8;
float val = 0;
float d;
float a = exp(7.453976699);
float b = -0.907499336;
// float a = exp(7.449);
// float b = -0.9076;
float avgs[27];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Automated Calibration of Range Finder
  // Serial.println("Ready...");
  // delay(1000);
  // Serial.println("Set...");
  // delay(1000);
  // Serial.println("GO!!!!!");
  // for(int i=0; i<27; i++){
  //   for(int j=0; j<10; j++){
  //     val = val + analogRead(dist_pin);
  //   }
  //   avgs[i] = val / 10;
  //   val = 0;
  //   Serial.println(avgs[i]);
  //   delay(5000);
  // }
  // Serial.println("Finish!!");

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
