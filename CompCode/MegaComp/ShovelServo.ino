void ShovelServo(void){
  t = (millis()-t_start) / 1000;
  if(shov_angle_start > shov_angle_final){
    shov_angle_des = ((shov_angle_start-shov_angle_final) / (1+exp(t-shov_t_final/2))) + shov_angle_final;
  }
  if(shov_angle_start < shov_angle_final){
    shov_angle_des = (shov_angle_final-shov_angle_start) / (1+exp(-t+shov_t_final/2)) + shov_angle_start + 1;
  }
  shovel_servo.write(shov_angle_des);
}