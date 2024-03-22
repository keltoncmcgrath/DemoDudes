void DumpBlock(void){
  switch(current_block.pos){
    // Block placed on guidelines
    case '1':
    case '3':
    case '4':
    case '6':
      if(current_block.elev == 'l'){
        if(arm_servo.read() != arm_angle_final){
          ArmServo();
        }
        if(final_stage){
          DistSense();
        }
        StraightLine();
        if((abs(theta1_des)>=abs(theta1_final) && abs(theta2_des)>=abs(theta2_final)) || dist_actual <= dump_dist_lower){
          md.setSpeeds(0, 0);
          // Prepare for forward travel
          if(!final_stage){
            dist_final = 14;
            time_final = 5;
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            final_stage = true;
            ResetTravelVars();
          }
          // Prepare for return to dispenser
          else if(final_stage){
            // Store some commands in LL
            state = 'f';
          }
        }
      }
      else if(current_block.elev == 'u'){

      }
      break;
    
    // Block placed on black line
    case '2':
    case '5':

      break;
  }
}