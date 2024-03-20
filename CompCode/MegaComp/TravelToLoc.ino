void TravelToLoc(void){
  // Switch on each face
  switch(current_block.face){
    // North Face Case
    case 'n':
      if(turn_bool){
        Turn();
        if(abs(theta1_des)>=abs(theta1_final) && abs(theta2_des)>=abs(theta2_final)){
          md.setSpeeds(0, 0);
          if(!final_stage){
            // Set straight travel variables according to pos
            if(current_block.pos == '1'){
              dist_final = guide1-2;
              time_final = 4;
            }
            else if(current_block.pos == '2'){
              dist_final = guide2;
              time_final = 4.5;
            }
            else if(current_block.pos == '3'){
              dist_final = guide3;
              time_final = 5;
            }
            // Travel Vars
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            final_stage = true;
            ResetTravelVars();
            // Arm Servo Vars
            arm_angle_final = arm_max_angle;
            arm_angle_start = arm_servo.read();
            arm_t_final = 2;
            // Change State
            straight_bool = true;
            turn_bool = false;
            // Add turn command to linked list
          }
          else if(final_stage){
            state = 'e';
          }
        }
      }

      if(straight_bool){

        StraightLine();
        if(abs(theta1_des)>=abs(theta1_final) && abs(theta2_des)>=abs(theta2_final)){
          turn_angle_final = pi/2;
          time_final = 2;
          theta1_final = turn_angle_final * (wheel_dist/2) / wheel_radius;
          theta2_final = -turn_angle_final * (wheel_dist/2) / wheel_radius;
          ResetTravelVars();
          turn_bool = true;
          straight_bool = false;
        }
      }
      break;

    case 'e':
      break;

    case 's':
      break;
    
    case 'w':
      break;
  }
}