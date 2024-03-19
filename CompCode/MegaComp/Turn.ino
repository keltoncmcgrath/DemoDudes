// REWRITE TO NOT USE WHILE
void Turn(){
  // Reset Variables
  encoder1.write(0);
  encoder2.write(0);
  counts1 = 0;
  counts2 = 0;
  theta1_des = 0;
  theta2_des = 0;
  t_old = 0;

  // Set theta final and omega besed on direction of travel
  if(turn_dir == 'r'){
    theta1_final = turn_angle_final * (wheel_dist/2) / wheel_radius;
    theta2_final = -turn_angle_final * (wheel_dist/2) / wheel_radius;
  }
  else{
    theta1_final = -turn_angle_final * (wheel_dist/2) / wheel_radius;
    theta2_final = turn_angle_final * (wheel_dist/2) / wheel_radius;
  }
  omega1_des = theta1_final/time_final;
  omega2_des = theta2_final/time_final;

  // Trajectory and control loop
  while(abs(theta1_des) < abs(theta1_final) && abs(theta2_des) < abs(theta2_final)){
    // Update varaibles
    t = (millis()-t_start)/1000;
    delta_T = t - t_old;
    counts1 = encoder1.read();
    counts2 = encoder2.read();
    theta1 = float(counts1*2*pi) / (gear_ratio * counts_per_rev);
    theta2 = float(counts2*2*pi) / (gear_ratio * counts_per_rev);

    // Calculate new desired thetas
    theta1_des += omega1_des*delta_T;
    theta2_des += omega2_des*delta_T;

    if(abs(theta1_des)>=abs(theta1_final)){
      theta1_des = theta1_final;
    }
    if(abs(theta2_des)>=abs(theta2_final)){
      theta2_des = theta2_final;
    }

    // Control and constrain speed
    m1s = KP * (theta1_des-theta1);
    m2s = KP * (theta2_des-theta2); 
    m1s = constrain(m1s, -400, 400);
    m2s = constrain(m2s, -400, 400);

    // Set motor speeds and record old time
    md.setSpeeds(m1s,m2s); 
    t_old = t;
  }
  md.setSpeeds(0, 0);
}