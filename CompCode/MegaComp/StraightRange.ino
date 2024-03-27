void StraightRange(void) {
  // Update varaibles
  counts1 = encoder1.read();
  counts2 = encoder2.read();
  theta1 = float(counts1 * 2 * PI) / (gear_ratio * counts_per_rev);
  theta2 = float(counts2 * 2 * PI) / (gear_ratio * counts_per_rev);

  // Control speed and
  m1s = line_speed - straight_kp*(theta1-theta2);
  m2s = line_speed - straight_kp*(theta2-theta1);
  m1s = constrain(m1s, -400, 400);
  m2s = constrain(m2s, -400, 400);
  md.setSpeeds(m1s, m2s);
}