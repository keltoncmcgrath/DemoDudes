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
              dist_final = guide1-arc_radius-(wheel_dist_arc/2);
              time_final = 4;
            }
            else if(current_block.pos == '2'){
              dist_final = guide2-arc_radius-(wheel_dist_arc/2);
              time_final = 4.5;
            }
            else if(current_block.pos == '3'){
              dist_final = guide3-arc_radius-(wheel_dist_arc/2);
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
            arm_t_final = 1.5;
            // Change State
            straight_bool = true;
            turn_bool = false;
            // Add turn command to linked list
          }
          // Prepare for block dump
          else if(final_stage){
            if(current_block.elev == 'l'){
              arm_angle_final = arm_low_dump_angle;
              arm_angle_start = arm_servo.read();
              arm_t_final = 1;
            }
            dist_actual = 50;
            dist_final = -5;
            time_final = 2;
            arm_t_final = 2;
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            final_stage = false;
            ResetTravelVars();
            state = 'e';
          }
        }
      }

      if(straight_bool){
        ArmServo();
        StraightLine();
        if(abs(theta1_des)>=abs(theta1_final) && abs(theta2_des)>=abs(theta2_final)){
          turn_angle_final = pi/2;
          time_final = 2;
          theta1_final = turn_angle_final * (wheel_dist_turn/2) / wheel_radius;
          theta2_final = -turn_angle_final * (wheel_dist_turn/2) / wheel_radius;
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