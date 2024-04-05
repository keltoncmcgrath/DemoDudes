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
  num = 0;
  den = 0;

  // Calculate error and control speed
  ir_error = ir_dist_desired - ir_dist_actual;
  ir_d_error = (ir_error - ir_error_last) / delta_T;
  ir_integral_error += ir_integral_error * delta_T;
  ir_integral_error = constrain(ir_integral_error, -400/ki, 400/ki);
  // m1s = line_follow_speed - kp * ir_error;
  // m2s = line_follow_speed + kp * ir_error;
  m1s = line_follow_speed - kp*ir_error - kd*ir_d_error - ki*ir_integral_error;
  m2s = line_follow_speed + kp*ir_error + kd*ir_d_error + ki*ir_integral_error;
  md.setSpeeds(m1s, m2s);
  ir_error_last = ir_error;
}