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
          dist_final = guide1 - arc_radius - (wheel_dist / 2); //+ 1.5;
          if (current_block.elev == 'l') {
            directions.AddTailNode('d', dist_final, 2.5, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            dump_dist_upper = 10;
            directions.AddTailNode('d', dist_final, 2.5, 0);
          }
        } else if (current_block.pos == '2') {
          dist_final = guide2 - arc_radius - (wheel_dist / 2);
          if (current_block.elev == 'l') {
            directions.AddTailNode('k', 350, 0, 0, 's', shov_low_dump_angle, 2);
            directions.AddTailNode('d', ir_to_wheel-0.5, 0.5);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('k', 350);
            directions.AddTailNode('d', ir_to_wheel, 0.5);
          }
        } else if (current_block.pos == '3') {
          dist_final = guide3 - arc_radius - (wheel_dist / 2);
          if (current_block.elev == 'l') {
            directions.AddTailNode('d', dist_final, 3, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('d', dist_final, 3, 0);
          }
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
        directions.AddTailNode('d', ir_to_wheel, 0.5);
        directions.AddTailNode('t', PI / 2, turn_time);
        if (current_block.pos == '4') {
          dist_final = guide4 - north_guide - arc_radius;
          directions.AddTailNode('l', dist_final);
        } else if (current_block.pos == '5') {
          dist_final = guide5 - north_guide - arc_radius;
          directions.AddTailNode('l', dist_final);
        } else if (current_block.pos == '6') {
          dist_final = guide6 - north_guide + arc_radius;
          directions.AddTailNode('l', dist_final, 4);
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
          directions.AddTailNode('d', guide1, 2.5);
        } else if (current_block.pos == '2') {
          directions.AddTailNode('k', 350);
          directions.AddTailNode('d', ir_to_wheel, 0.5);
        } else if (current_block.pos == '3') {
          directions.AddTailNode('d', guide3, 4);
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
        if(current_block.elev == 'l'){
          directions.AddTailNode('t', PI, turn_time+1, 0, 'a', arm_low_dump_angle, 1);
        } else if (current_block.elev == 'u'){
          directions.AddTailNode('t', PI, turn_time+1, 0, 'a', arm_max_angle, 1);
        }
        if (current_block.pos == '4') {
          if (current_block.elev == 'u') {
            directions.AddTailNode('l', guide4-north_guide+1);
          } else {
            directions.AddTailNode('l', guide4-north_guide);
          }
        } else if (current_block.pos == '5') {
          directions.AddTailNode('l', guide5-north_guide-1);
        } else if (current_block.pos == '6') {
          if (current_block.elev == 'l'){
            dump_dist_lower = 8;
            directions.AddTailNode('l', guide6-north_guide);
          } else {
            dump_dist_upper = 10;
            directions.AddTailNode('l', guide6-north_guide+1);
          }
        }
        if (current_block.elev == 'l'){
          directions.AddTailNode('t', -PI / 2, turn_time, 0, 's', shov_low_dump_angle, turn_time);
        } else {
          directions.AddTailNode('t', -PI / 2, turn_time);
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
        directions.AddTailNode('o', -PI / 2, arc_time_little+0.5, arc_radius);
        if (current_block.pos == '1') {
          dist_final = guide3 - arc_radius - (wheel_dist / 2);
          directions.AddTailNode('d', dist_final, 2.5, 0, 'a', arm_max_angle, 3);
        } else if (current_block.pos == '2') {
          dist_final = guide2 - arc_radius - (wheel_dist / 2);
          directions.AddTailNode('k', 350, 0, 0, 'a', arm_max_angle, 2);
          directions.AddTailNode('d', ir_to_wheel, 0.5);
        } else if (current_block.pos == '3') {
          if(current_block.elev == 'l') {
            dist_final = guide1 - arc_radius - (wheel_dist / 2) + 1;
          } else {
            dist_final = guide1 - arc_radius - (wheel_dist / 2) + 3;
          }
          directions.AddTailNode('d', dist_final, 3, 0, 'a', arm_max_angle, 3);
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
        if(current_block.elev == 'l'){
          directions.AddTailNode('t', -PI, turn_time+1, 0, 'a', arm_low_dump_angle, 1);
        } else if (current_block.elev == 'u'){
          directions.AddTailNode('t', -PI, turn_time+1, 0, 'a', arm_max_angle, 1);
        }
        if (current_block.pos == '4') {
          if (current_block.elev == 'l'){
            directions.AddTailNode('l', guide4-north_guide-2.2);
          } else {
            dump_dist_upper = 11;
            directions.AddTailNode('l', guide4-north_guide+1);
          }
        } else if (current_block.pos == '5') {
          directions.AddTailNode('l', guide5-north_guide-2);
        } else if (current_block.pos == '6') {
          directions.AddTailNode('l', guide6-north_guide-2);
        }
        if(current_block.elev == 'l') {
          directions.AddTailNode('t', PI / 2, turn_time, 0, 's', shov_low_dump_angle, turn_time);
        } else {
          directions.AddTailNode('t', PI / 2, turn_time);
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
          directions.AddTailNode('d', guide3, 3.3);
        } else if (current_block.pos == '2') {
          directions.AddTailNode('k', 350);
          directions.AddTailNode('d', ir_to_wheel, 0.5);
        } else if (current_block.pos == '3') {
          directions.AddTailNode('d', guide1+1.5, 3.5);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        if(current_block.elev == 'l'){
          directions.AddTailNode('d', -6, 1, 0, 'a', arm_low_dump_angle, 1);
        } else if (current_block.elev == 'u'){
          directions.AddTailNode('d', -1, 0.5);
        }
        break;

      // West Face
      case 'w':
        arc_radius = 0.8;
        line_dist = false;
        directions.AddTailNode('o', -PI / 2, arc_time_little, arc_radius);
        if (current_block.elev == 'l') {
          directions.AddTailNode('k', 350, 0, 0, 'a', arm_low_dump_angle, 3);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('k', 350, 0, 0, 'a', arm_max_angle, 3);
        }
        directions.AddTailNode('d', ir_to_wheel, 0.5);
        directions.AddTailNode('t', -PI / 2, turn_time);
        if (current_block.pos == '4') {
          dist_final = guide4 - north_guide + arc_radius;
          directions.AddTailNode('l', dist_final);
        } else if (current_block.pos == '5') {
          dist_final = guide5 - north_guide + arc_radius + 2;
          directions.AddTailNode('l', dist_final);
        } else if (current_block.pos == '6') {
          dist_final = guide6 - north_guide + arc_radius;
          directions.AddTailNode('l', dist_final);
        }
        directions.AddTailNode('t', -PI / 2, turn_time);
        break;
    } // end switch
  } // end else
}