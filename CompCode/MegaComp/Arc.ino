void Arc() {
  // Update varaibles
  t = (millis() - t_start) / 1000;
  delta_T = t - t_old;
  counts1 = encoder1.read();
  counts2 = encoder2.read();
  theta1 = float(counts1 * 2 * pi) / (gear_ratio * counts_per_rev);
  theta2 = float(counts2 * 2 * pi) / (gear_ratio * counts_per_rev);

  // Calculate new desired thetas
  theta1_des += omega1_des * delta_T;
  theta2_des += omega2_des * delta_T;

  if (abs(theta1_des) >= abs(theta1_final)) {
    theta1_des = theta1_final;
  }
  if (abs(theta2_des) >= abs(theta2_final)) {
    theta2_des = theta2_final;
  }

  // Control and constrain speed
  m1s = KP * (theta1_des - theta1);
  m2s = KP * (theta2_des - theta2);
  m1s = constrain(m1s, -400, 400);
  m2s = constrain(m2s, -400, 400);

  // Set motor speeds and record old time
  md.setSpeeds(m1s, m2s);
  t_old = t;
}
