void ResetTravelVars(void){
  t_start = millis();
  t_old = 0;
  counts1 = 0;
  counts2 = 0;
  theta1_des = 0;
  theta2_des = 0;
  encoder1.write(0);
  encoder2.write(0);
  // Initialize Straight Line Variables
  omega1_des = theta1_final/time_final;
  omega2_des = theta2_final/time_final;
}