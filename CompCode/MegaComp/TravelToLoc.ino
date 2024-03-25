void TravelToLoc(void) {
  // Switch on each face
  switch (current_block.face) {
    // North Face Case
    case 'n':
      // Turn to align with face
      if (turn_bool) {
        Turn();
        if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
          md.setSpeeds(0, 0);
          // Prepare for travel
          if (!final_stage) {
            // Set straight travel variables according to pos
            if (current_block.pos == '1') {
              dist_final = guide1 - arc_radius - (wheel_dist_arc / 2);
              time_final = 4;
            } else if (current_block.pos == '2') {
              dist_final = guide2 - arc_radius - (wheel_dist_arc / 2);
              time_final = 4.5;
            } else if (current_block.pos == '3') {
              dist_final = guide3 - arc_radius - (wheel_dist_arc / 2);
              time_final = 5;
            }
            // Travel Vars
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            ResetTravelVars();
            // Arm Servo Vars
            arm_angle_final = arm_max_angle;
            arm_angle_start = arm_servo.read();
            arm_t_final = 1.5;
            // Change State
            straight_bool = true;
            turn_bool = false;
          }
          // Prepare to back up before dump
          else if (final_stage) {
            if (current_block.elev == 'l') {
              arm_angle_final = arm_low_dump_angle;
              arm_angle_start = arm_servo.read();
              arm_t_final = 2;
              dist_actual = 1000;
              dist_final = -6;
              time_final = 2;
            } else if (current_block.elev == 'u') {
              dist_final = -6;
              time_final = 2;
            }
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            straight_bool = true;
            turn_bool = false;
            ResetTravelVars();
          }
        }
      }
      // Travel to block pos
      else if (straight_bool) {
        // Actuate arm servo
        if (arm_servo.read() != arm_angle_final) {
          ArmServo();
        }
        StraightLine();
        if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
          // Prepare to turn to face
          if (!final_stage) {
            turn_angle_final = pi / 2;
            time_final = turn_time;
            theta1_final = turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
            theta2_final = -turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
            final_stage = true;
            ResetTravelVars();
            turn_bool = true;
            straight_bool = false;
          }
          // Prepare to dump block
          else if (final_stage) {
            if (current_block.elev == 'l') {
              dist_final = 6;
              time_final = 2;
            } else if (current_block.elev == 'u') {
              dist_final = 2;
              time_final = 2;
              straight_bool = true;
              servo_bool = false;
            }
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            ResetTravelVars();
            state = 'e';
          }
        }
      }
      break;

    case 'e':
      if (turn_bool) {
        Turn();
        if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
          md.setSpeeds(0, 0);
          // Prepare for travel
          if (!final_stage && !final_final_stage) {
            // Travel vars
            dist_final = east_guide - arc_radius - (wheel_dist_arc / 2);
            time_final = 6;
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            final_stage = true;
            ResetTravelVars();
            // Arm Servo Vars
            if (current_block.elev == 'l') {
              arm_angle_final = arm_low_dump_angle;
            } else if (current_block.elev == 'u') {
              arm_angle_final = arm_max_angle;
            }
            arm_angle_start = arm_servo.read();
            arm_t_final = 1.5;
            // Change State
            straight_bool = true;
            turn_bool = false;
          }
          // Prepare for line following
          else if (final_stage && !final_final_stage) {
            // Set line follow distance and vars
            if (current_block.pos == '4') {
              dist_final = guide4 - arc_radius - (wheel_dist_arc / 2);
            } else if (current_block.pos = '5') {
              dist_final = guide5 - arc_radius - (wheel_dist_arc / 2);
            } else if (current_block.pos = '6') {
              dist_final = guide6 - arc_radius - (wheel_dist_arc / 2);
            }
            ResetTravelVars();
            turn_bool = false;
            line_follow_bool = true;
          }
          // Prepare for block dump
          else if (final_final_stage) {
            if (current_block.elev == 'l') {
              dist_final = 5;
              time_final = 2;
              line_follow_speed = 50;
            } else if (current_block.elev == 'u') {
              dist_final = 10;
              time_final = 5;
              line_follow_speed = 50;
              straight_bool = true;
              servo_bool = false;
            }
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            final_stage = false;
            ResetTravelVars();
            state = 'e';
          }
        }
      }
      // Travel to east guide
      else if (straight_bool) {
        // Raise arms servo it not already raised
        if (arm_servo.read() != arm_angle_final) {
          ArmServo();
        }
        StraightLine();
        // Prepare for turning
        if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
          md.setSpeeds(0, 0);
          turn_angle_final = pi / 2;
          time_final = turn_time;
          theta1_final = turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
          theta2_final = -turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
          ResetTravelVars();
          turn_bool = true;
          straight_bool = false;
        }
      }
      // Follow line to block pos
      else if (line_follow_bool) {
        LineFollow();
        ReadEncoderDist();
        if (dist_traveled >= dist_final) {
          md.setSpeeds(0, 0);
          turn_angle_final = pi / 2;
          time_final = turn_time;
          theta1_final = turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
          theta2_final = -turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
          final_stage = false;
          final_final_stage = true;
          ResetTravelVars();
          line_follow_bool = false;
          turn_bool = true;
        }
      }
      break;

    case 's':
      // Turning
      if (turn_bool) {
        if (arm_servo.read() != arm_angle_final) {
          ArmServo();
        }
        Turn();
        if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
          md.setSpeeds(0, 0);
          // Prepare for straight travel to face
          if (!final_stage && !final_final_stage) {
            // Arm servo vals
            arm_angle_final = arm_max_angle;
            arm_angle_start = arm_servo.read();
            arm_t_final = 3;
            // Travel vals
            dist_final = south_guide;
            time_final = 3;
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            ResetTravelVars();
            // Change stage
            turn_bool = false;
            straight_bool = true;
          }
          // Prepare to travel to block pos
          else if (final_stage && !final_final_stage) {
            // Set travel vals depending on pos
            if (current_block.pos == '1') {
              dist_final = guide1;
              time_final = 4;
            } else if (current_block.pos == '2') {
              dist_final = guide2;
              time_final = 4.5;
            } else if (current_block.pos == '3') {
              dist_final = guide3;
              time_final = 5;
            }
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            ResetTravelVars();
            // Change stage
            turn_bool = false;
            straight_bool = true;
          }
          // Prepare to dump
          else if (final_final_stage) {
            if (current_block.elev == 'l') {
              dist_final = 6;
              time_final = 2;
            } else if (current_block.elev == 'u') {
              dist_final = 2;
              time_final = 2;
              straight_bool = true;
              servo_bool = false;
            }
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            ResetTravelVars();
            state = 'e';
          }
        }
      } 
      // Striaght
      else if (straight_bool) {
        if (arm_servo.read() != arm_angle_final) {
          ArmServo();
        }
        StraightLine();
        if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
          md.setSpeeds(0, 0);
          // Prepare to turn to travel to pos
          if (!final_stage && !final_final_stage) {
            turn_angle_final = -pi / 2;
            time_final = 2;
            theta1_final = turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
            theta2_final = -turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
            final_stage = true;
            ResetTravelVars();
            turn_bool = true;
            straight_bool = false;
          }
          // Prepare to turn to face chassis
          else if (final_stage && !final_final_stage) {
            // Change arm servo if dumping in lower position
            if (current_block.elev == 'l') {
              arm_angle_final = arm_low_dump_angle;
              arm_angle_start = arm_servo.read();
              arm_t_final = 2;
            }
            // Travel vals
            turn_angle_final = -pi / 2;
            time_final = 2;
            theta1_final = turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
            theta2_final = -turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
            final_final_stage = true;
            ResetTravelVars();
            turn_bool = true;
            straight_bool = false;
          }
        }
      }
      break;

    case 'w':
      // Turn
      if (turn_bool) {
        Turn();
        if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
          md.setSpeeds(0, 0);
          // Prepare to travel to block pos
          if (!final_stage) {
            if (current_block.pos == '4') {
              dist_final = guide4;
              time_final = 3;
            } else if (current_block.pos == '5') {
              dist_final = guide5;
              time_final = 3.5;
            } else if (current_block.pos == '6') {
              dist_final = guide6;
              time_final = 4;
            }
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            ResetTravelVars();
            // Arm Servo Vars
            if (current_block.elev = 'l') {
              arm_angle_final = arm_low_dump_angle;
            } else if (current_block.elev = 'u') {
              arm_angle_final = arm_max_angle;
            }
            arm_angle_start = arm_servo.read();
            arm_t_final = 2;
            // Change State
            straight_bool = true;
            turn_bool = false;
          }
          // Prepare for dump
          else if (final_stage) {
            if (current_block.elev == 'l') {
              dist_final = 5;
              time_final = 2;
            } else if (current_block.elev == 'u') {
              dist_final = 10;
              time_final = 5;
              line_follow_speed = 50;
              straight_bool = true;
              servo_bool = false;
            }
            theta1_final = dist_final / wheel_radius;
            theta2_final = dist_final / wheel_radius;
            final_stage = false;
            ResetTravelVars();
            state = 'e';
          }
        }
      } 
      // Travel to block pos
      else if (straight_bool) {
        if (arm_servo.read() != arm_angle_final) {
          ArmServo();
        }
        StraightLine();
        if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
          md.setSpeeds(0, 0);
          turn_angle_final = -pi / 2;
          time_final = turn_time;
          theta1_final = turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
          theta2_final = -turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
          final_stage = true;
          ResetTravelVars();
          turn_bool = true;
          straight_bool = false;
        }
      }
      break;
  }
}