void TimedDrive(void) {
  // Update varaibles
  t = (millis() - t_start) / 1000;
  delta_T = t - t_old;
  counts1 = encoder1.read();
  counts2 = encoder2.read();
  theta1 = float(counts1 * 2 * PI) / (gear_ratio * counts_per_rev);
  theta2 = float(counts2 * 2 * PI) / (gear_ratio * counts_per_rev);

  // Calculate new desired thetas
  theta1_des += omega1_des * delta_T;
  theta2_des += omega2_des * delta_T;
  if (abs(theta1_des) >= abs(theta1_final)) {
    theta1_des = theta1_final;
  }
  if (abs(theta2_des) >= abs(theta2_final)) {
    theta2_des = theta2_final;
  }

  // Calculate omega
  omega1 = (theta1 - theta1_old) / delta_T;
  omega2 = (theta2 - theta2_old) / delta_T;

  // Compute errors
  error1 = theta1_des - theta1;
  d_error1 = omega1_des - omega1;
  integral_error1 += error1 * delta_T;
  integral_error1 = constrain(integral_error1, -10. / KI, 10. / KI);
  error2 = theta2_des - theta2;
  d_error2 = omega2_des - omega2;
  integral_error2 += error2 * delta_T;
  integral_error2 = constrain(integral_error2, -10. / KI, 10. / KI);

  // Control laws
  V1 = KP * error1 + KD * d_error1 + KI * integral_error1;
  V2 = KP * error2 + KD * d_error2 + KI * integral_error2;
  m1s = 400 * V1 / 10;
  m2s = 400 * V2 / 10;

  // Set Speed
  m1s = constrain(m1s, -400, 400);
  m2s = constrain(m2s, -400, 400);
  md.setSpeeds(m1s, m2s);

  // Reset vars
  t_old = t;
  theta1_old = theta1;
  theta2_old = theta2;
}