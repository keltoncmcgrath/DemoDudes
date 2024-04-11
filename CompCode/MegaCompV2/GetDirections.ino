void GetDirections(void) {
  /* Set instructions to travel to preffered block dump location */
  // Get directions from home dispenser
  if(home_dispense){
    switch (current_block.face) {
      // North Face
      case 'n':
        arc_radius = 0.8;
        directions.AddTailNode('o', PI / 2, arc_time_little, arc_radius, 'a', arm_max_angle, 2);
        if (current_block.pos == '1') {
          dist_right = false;
          if (current_block.elev == 'l') {
            directions.AddTailNode('k', line_follow_speed, 0, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('k', line_follow_speed);
          }
          directions.AddTailNode('d', -next_pos_dist + ir_to_wheel, ir_to_wheel_time-0.2);
        } else if (current_block.pos == '2') {
          dist_right = true;
          dist_final = guide2 - arc_radius - (wheel_dist / 2);
          if (current_block.elev == 'l') {
            directions.AddTailNode('k', 350, 0, 0, 's', shov_low_dump_angle, 2);
            directions.AddTailNode('d', ir_to_wheel-0.5, ir_to_wheel_time);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('k', 350);
            directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
          }
        } else if (current_block.pos == '3') {
          dist_right = true;
          if (current_block.elev == 'l') {
            directions.AddTailNode('k', line_follow_speed, 0, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('k', line_follow_speed);
          }
          directions.AddTailNode('d', next_pos_dist + ir_to_wheel - 2.5, ir_to_wheel_time+0.2);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        if (current_block.pos == '2' && current_block.elev == 'l'){
          directions.AddTailNode('d', -13, 1, 0, 'a', arm_low_dump_angle, 1);
        }
        else if (current_block.elev == 'l') {
          directions.AddTailNode('d', -6, 1, 0, 'a', arm_low_dump_angle, 1);
        } else if (current_block.elev == 'u') {
          // directions.AddTailNode('d', -6, 0.5);
        }
        break;

      // East Face
      case 'e':
        arc_radius = 0.8;
        line_dist = false;
        directions.AddTailNode('o', PI / 2, arc_time_little, arc_radius);
        if (current_block.elev == 'l') {
          directions.AddTailNode('k', 350, 0, 0, 'a', arm_low_dump_angle, 3);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('k', 350, 0, 0, 'a', arm_max_angle, 3);
        }
        directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
        directions.AddTailNode('t', PI / 2, turn_time);
        if (current_block.pos == '4') {
          dist_right = false;
          dump_dist_lower = 9.5;
          directions.AddTailNode('j', 300);
          directions.AddTailNode('d', -next_pos_dist + ir_to_wheel, ir_to_wheel_time);
        } else if (current_block.pos == '5') {
          dist_right = true;
          directions.AddTailNode('j', line_follow_speed);
          directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
        } else if (current_block.pos == '6') {
          dist_right = true;
          directions.AddTailNode('j', line_follow_speed);
          directions.AddTailNode('d', next_pos_dist + ir_to_wheel, ir_to_wheel_time+0.2);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        break;

      // South Face
      case 's':
        line_speed = 300;
        directions.AddTailNode('t', PI, turn_time + 1);
        directions.AddTailNode('r', dist_to_wall, 0, 0, 'a', arm_max_angle, 2);
        directions.AddTailNode('t', -PI / 2, turn_time);
        if (current_block.pos == '1') {
          dump_dist_upper = 14;
          dist_right = true;
          directions.AddTailNode('k', line_follow_speed);
          directions.AddTailNode('d', -next_pos_dist + ir_to_wheel, ir_to_wheel_time+0.2);
        } else if (current_block.pos == '2') {
          dist_right = true;
          directions.AddTailNode('k', 350);
          directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
        } else if (current_block.pos == '3') {
          dist_right = false;
          directions.AddTailNode('k', 350);
          directions.AddTailNode('d', ir_to_wheel + next_pos_dist - 0.5, ir_to_wheel_time+0.2);
        }
        directions.AddTailNode('t', -PI / 2, turn_time);
        if(current_block.elev == 'l'){
          directions.AddTailNode('d', -6, 1, 0, 'a', arm_low_dump_angle, 1);
        } else if (current_block.elev == 'u'){
          directions.AddTailNode('d', -1, 1);
        }
        break;

      // West Face
      case 'w':
        line_dist = false;
        // if(current_block.elev == 'l'){
        //   directions.AddTailNode('t', PI, turn_time+1, 0, 'a', arm_low_dump_angle, 1);
        // } else if (current_block.elev == 'u'){
        //   directions.AddTailNode('t', PI, turn_time+1, 0, 'a', arm_max_angle, 1);
        // }
        if (current_block.pos == '4') {
          dist_right = true;
          if(current_block.elev == 'l'){
            directions.AddTailNode('j', -300, 0, 0, 'a', arm_low_dump_angle, 1);
            directions.AddTailNode('d', next_pos_dist + ir_to_wheel+1, ir_to_wheel_time);
          } else {
            directions.AddTailNode('j', -300, 0, 0, 'a', arm_max_angle, 1);
            directions.AddTailNode('d', next_pos_dist + ir_to_wheel+1, ir_to_wheel_time);
          }
        } else if (current_block.pos == '5') {
          dist_right = true;
          if (current_block.pos == 'l') {
            directions.AddTailNode('j', -line_follow_speed, 0, 0, 'a', arm_low_dump_angle, 1);
            directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
          } else {
            directions.AddTailNode('j', -line_follow_speed, 0, 0, 'a', arm_max_angle, 1);
            directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
          }
        } else if (current_block.pos == '6') {
          dump_dist_upper = 10;
          dump_dist_lower = 9;
          dist_right = false;
          if (current_block.elev == 'l') {
            directions.AddTailNode('j', -line_follow_speed, 0, 0, 'a', arm_low_dump_angle, 1);
            directions.AddTailNode('d', -next_pos_dist + ir_to_wheel, ir_to_wheel_time+0.2);
          } else {
            directions.AddTailNode('j', -line_follow_speed, 0, 0, 'a', arm_max_angle, 1);
            directions.AddTailNode('d', -next_pos_dist + ir_to_wheel, ir_to_wheel_time+0.2);
          }
        }
        if (current_block.elev == 'l'){
          directions.AddTailNode('t', PI / 2, turn_time, 0, 's', shov_low_dump_angle, turn_time);
        } else {
          directions.AddTailNode('t', PI / 2, turn_time);
        }
        break;
    } // end switch
  } // end if

  // Get directions from opponent dispenser
  else{
    switch (current_block.face) {
      // North Face
      case 'n':
        arc_radius = 0.8;
        directions.AddTailNode('o', -PI / 2, arc_time_little+0.5, arc_radius, 'a', arm_max_angle, 2);
        if (current_block.pos == '1') {
          dist_right = false;
          directions.AddTailNode('k', 350);
          directions.AddTailNode('d', ir_to_wheel + next_pos_dist, ir_to_wheel_time+0.2);
        } else if (current_block.pos == '2') {
          dist_right = true;
          directions.AddTailNode('k', 350, 0, 0, 'a', arm_max_angle, 2);
          directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
        } else if (current_block.pos == '3') {
          dist_right = true;
          directions.AddTailNode('k', line_follow_speed);
          directions.AddTailNode('d', -next_pos_dist + ir_to_wheel, ir_to_wheel_time+0.2);
        }
        if (current_block.elev == 'l') {
          directions.AddTailNode('t', -PI / 2, turn_time, 0, 's', shov_low_dump_angle);
          directions.AddTailNode('d', -4, 1, 0, 'a', arm_low_dump_angle, 1);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('t', -PI / 2, turn_time);
          directions.AddTailNode('d', -6, 0.5);
        }
        break;

      // East Face
      case 'e':
        line_dist = false;
        // if(current_block.elev == 'l'){
        //   directions.AddTailNode('t', -PI, turn_time+1, 0, 'a', arm_low_dump_angle, 1);
        // } else if (current_block.elev == 'u'){
        //   directions.AddTailNode('t', -PI, turn_time+1, 0, 'a', arm_max_angle, 1);
        // }
        if (current_block.pos == '4') {
          dump_dist_upper = 10;
          dump_dist_lower = 8;
          dist_right = false;
          if(current_block.elev == 'l'){
            directions.AddTailNode('j', -300, 0, 0, 'a', arm_low_dump_angle, 1);
            directions.AddTailNode('d', next_pos_dist + ir_to_wheel, ir_to_wheel_time);
          } else {
            directions.AddTailNode('j', -300, 0, 0, 'a', arm_max_angle, 1);
            directions.AddTailNode('d', next_pos_dist + ir_to_wheel, ir_to_wheel_time);
          }
        } else if (current_block.pos == '5') {
          dist_right = true;
          if (current_block.pos == 'l') {
            directions.AddTailNode('j', -line_follow_speed, 0, 0, 'a', arm_low_dump_angle, 1);
            directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
          } else {
            directions.AddTailNode('j', -line_follow_speed, 0, 0, 'a', arm_max_angle, 1);
            directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
          }
        } else if (current_block.pos == '6') {
          dist_right = true;
          if (current_block.elev == 'l') {
            directions.AddTailNode('j', -line_follow_speed, 0, 0, 'a', arm_low_dump_angle, 1);
            directions.AddTailNode('d', -next_pos_dist + ir_to_wheel+2, ir_to_wheel_time+0.2);
          } else {
            directions.AddTailNode('j', -line_follow_speed, 0, 0, 'a', arm_max_angle, 1);
            directions.AddTailNode('d', -next_pos_dist + ir_to_wheel+2, ir_to_wheel_time+0.2);
          }
        }
        if(current_block.elev == 'l') {
          directions.AddTailNode('t', -PI / 2, turn_time, 0, 's', shov_low_dump_angle, turn_time);
        } else {
          directions.AddTailNode('t', -PI / 2, turn_time);
        }
        break;

      // South Face
      case 's':
        line_dist = true;
        line_speed = 300;
        directions.AddTailNode('t', -PI, turn_time + 1);
        directions.AddTailNode('l', dist_to_wall, 0, 0, 'a', arm_max_angle, 2);
        directions.AddTailNode('t', PI / 2, turn_time);
        if (current_block.pos == '1') {
          dist_right = true;
          directions.AddTailNode('k', 350);
          directions.AddTailNode('d', ir_to_wheel + next_pos_dist - 2.5, ir_to_wheel_time+0.2);
        } else if (current_block.pos == '2') {
          dist_right = true;
          directions.AddTailNode('k', 350);
          directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
        } else if (current_block.pos == '3') {
          dist_right = false;
          directions.AddTailNode('k', line_follow_speed);
          directions.AddTailNode('d', -next_pos_dist + ir_to_wheel, ir_to_wheel_time+0.2);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        if (current_block.pos == 2){
          directions.AddTailNode('d', -15, 1.5);
        } else if (current_block.elev == 'l'){
          directions.AddTailNode('d', -6, 1, 0, 'a', arm_low_dump_angle, 1);
        } else if (current_block.elev == 'u'){
          directions.AddTailNode('d', -1, 0.5);
        }
        break;

      // West Face
      case 'w':
        arc_radius = 0.8;
        line_dist = false;
        directions.AddTailNode('o', -PI / 2, arc_time_little+0.5, arc_radius);
        if (current_block.elev == 'l') {
          directions.AddTailNode('k', 350, 0, 0, 'a', arm_low_dump_angle, 3);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('k', 350, 0, 0, 'a', arm_max_angle, 3);
        }
        directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
        directions.AddTailNode('t', -PI / 2, turn_time);
        if (current_block.pos == '4') {
          dist_right = true;
          directions.AddTailNode('j', 300);
          directions.AddTailNode('d', -next_pos_dist + ir_to_wheel, ir_to_wheel_time);
        } else if (current_block.pos == '5') {
          dist_right = true;
          directions.AddTailNode('j', line_follow_speed);
          directions.AddTailNode('d', ir_to_wheel, ir_to_wheel_time);
        } else if (current_block.pos == '6') {
          dist_right = false;
          directions.AddTailNode('j', line_follow_speed);
          directions.AddTailNode('d', next_pos_dist + ir_to_wheel, ir_to_wheel_time+0.2);
        }
        directions.AddTailNode('t', -PI / 2, turn_time);
        break;
    } // end switch
  } // end else
}