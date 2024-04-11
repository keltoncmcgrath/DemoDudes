void ResetTravelVars(void) {
  t_start = millis();
  t_old = 0;
  counts1 = 0;
  counts2 = 0;
  theta1_des = 0;
  theta2_des = 0;
  theta1_desf = 0;
  theta2_desf = 0;
  encoder1.write(0);
  encoder2.write(0);
  omega1_des = theta1_final / time_final;
  omega2_des = theta2_final / time_final;
  integral_error1 = 0;
  integral_error2 = 0;
  ir_integral_error = 0;
  ir_dist_actualf = 0;
  dist_actualf = 0;
}