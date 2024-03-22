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
            dist_final = 6;
            time_final = 2;
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
        // Travel straight until at distance
        if(straight_bool){
          StraightLine();
          DistSense();
          // Set shovel servo vars and set state
          if(dist_actual >= dump_dist_upper){
            md.setSpeeds(0, 0);
            arm_angle_final = arm_max_angle;
            arm_angle_start = arm_servo.read();
            arm_t_final = 1;
            shov_angle_start = shovel_servo.read();
            shov_angle_final = shov_dump_angle;
            shov_t_final = 2;
            final_stage = false;
            ResetTravelVars();
            straight_bool = false;
            servo_bool = true;
          }
        }
        // Dump block
        if(servo_bool){
          ShovelServo();
          if(shovel_servo.read() == shov_angle_final){
            // Reset vars to reset shovel to home
            if(!final_stage){
              shov_angle_start = shovel_servo.read();
              shov_angle_final = servo_home;
              shov_t_final = 2;
              final_stage = true;
              ResetTravelVars();
            }
            // Change state to return
            else if(final_stage){
              state = 'f';
            }
          }
        }
      }
      break;
    
    // Block placed on black line
    case '2':
    case '5':

      break;
  }
}