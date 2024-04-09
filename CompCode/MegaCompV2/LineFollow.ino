void LineFollow(void) {
  // Read sensor values and take bias off
  t = (millis()-t_start) / 1000;
  delta_T = t - t_start;
  qtr.read(ir_values);
  for (int i = 0; i < ir_sensor_count; i++) {
    ir_unbiased[i] = ir_values[i] - ir_bias[i];
  }

  // Calculate distance of line from deisred distance
  for (int i = 0; i < ir_sensor_count; i++) {
    num = num + (ir_unbiased[i] * ir_sensor_spacing[i]);
    den = den + ir_unbiased[i];
  }
  ir_dist_actual = num / den;
  ir_dist_actualf = ir_alpha*ir_dist_actual + (1-ir_alpha)*ir_dist_actualf;
  num = 0;
  den = 0;

  // Calculate error and control speed
  ir_error = ir_dist_desired - ir_dist_actualf;
  ir_d_error = (ir_error - ir_error_last) / delta_T;
  // ir_integral_error += ir_integral_error * delta_T;
  // ir_integral_error = constrain(ir_integral_error, -400/ki, 400/ki);
  // Check how many sensors read black
  for (int i=0; i<ir_sensor_count; i++) {
    if(ir_values[i] > 500) {
      black_count += 1;
    }
  }
  // If half the sensros read black, at junction
  if (black_count < ir_sensor_count/2) {
    m1s = line_follow_speed - kp*ir_error - kd*ir_d_error;// - ki*ir_integral_error;
    m2s = line_follow_speed + kp*ir_error + kd*ir_d_error;// + ki*ir_integral_error;
  }
  md.setSpeeds(m1s, m2s);
  ir_error_last = ir_error;
  black_count = 0;
}