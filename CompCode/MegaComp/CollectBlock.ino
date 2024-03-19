void CollectBlock(void){
  // Control Arm Servo
  if(arm_servo.read() != arm_angle_final){
    ArmServo();
  }

  // Control Shovel Servo
  if(shovel_servo.read() != shov_angle_final){
    ShovelServo();
  }

  // Drive forward / backward
  if(abs(theta1_des)<abs(theta1_final) && abs(theta2_des)<abs(theta2_final)){
    StraightLine();
  }

  // Switch forward stright line to backward straight line
  if(abs(theta1_des)>=abs(theta1_final) && abs(theta2_des)>=abs(theta2_final)){
    // Set Motors to Zero
    md.setSpeeds(0, 0);

    // Initialize Variables for Back Travel
    if(!final_stage){
      dist_final = -1.25;
      theta1_final = dist_final/wheel_radius;
      theta2_final = dist_final/wheel_radius;
      omega1_des = theta1_final/time_final;
      omega2_des = theta2_final/time_final;
      counts1 = 0;
      counts2 = 0;
      theta1_des = 0;
      theta2_des = 0;
      encoder1.write(0);
      encoder2.write(0);
      final_stage = true;
    }
    else if(final_stage){
      state = 'c';
      t_start = millis();
    }
  }
}