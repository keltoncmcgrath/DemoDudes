void ArmServo(void){
  t = (millis()-t_start) / 1000;
  if(arm_angle_start > arm_angle_final){
    arm_angle_des = ((arm_angle_start-arm_angle_final) / (1+exp((10/arm_t_final)*t-5))) + arm_angle_final;
  }
  if(arm_angle_start < arm_angle_final){
    arm_angle_des = (arm_angle_final-arm_angle_start) / (1+exp(-(10/arm_t_final*t)+5)) + arm_angle_start + 1;
  }
  arm_servo.write(arm_angle_des);
}