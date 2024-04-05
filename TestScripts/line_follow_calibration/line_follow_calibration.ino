#include <QTRSensors.h>

QTRSensors qtr;

const uint8_t ir_pins[] = {24, 25, 26, 27, 28, 29, 30, 31};

const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count] = {0, 0, 0, 0, 0, 0, 0, 0};
uint16_t ir_values_new[ir_sensor_count];
int ir_bias[] = {166, 58, 58, 106, 58, 106, 106};
int ir_unbiased[ir_sensor_count];
float ir_sensor_spacing[] = {0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6};
float ir_dist_desired = 2.8;
float ir_dist_actual;
float ir_error;
int m1s;
int m2s;
int kp = 75;
float num = 0;
float den = 0;
int speed = 200;

int num_calcs = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  qtr.setTypeRC();
  qtr.setSensorPins(ir_pins, ir_sensor_count);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<ir_sensor_count; i++){
    ir_values[i] = 0;
  }

  for(int i=0; i<num_calcs; i++){
    qtr.read(ir_values_new);
    for(int j=0; j<ir_sensor_count; j++){
      ir_values[j] += ir_values_new[j];
    }

  }
  for(int i=0; i<ir_sensor_count; i++){
    ir_bias[i] = ir_values[i] / num_calcs;
    Serial.print(ir_bias[i]);
    Serial.print('\t');
  }
  Serial.println();
}
