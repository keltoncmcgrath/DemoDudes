void LineFollow(void){
  // Read sensor values and take bias off
  qtr.read(ir_values);
  for(int i=0; i<ir_sensor_count; i++){
    ir_unbiased[i] = ir_values[i] - ir_bias[i];
  }

  // Calculate distance of line from deisred distance
  for(int i=0; i<ir_sensor_count; i++){
    num = num + (ir_unbiased[i] * ir_sensor_spacing[i]);
    den = den + ir_unbiased[i];
  }
  ir_dist_actual = num/den;
  num = 0;
  den = 0;

  // Calculate error and control speed
  ir_error = ir_dist_desired - ir_dist_actual;
  m1s = speed - kp*ir_error;
  m2s = speed + kp*ir_error;
  md.setSpeeds(m1s, m2s);
}