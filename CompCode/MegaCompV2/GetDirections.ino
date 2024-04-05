void GetDirections(void) {
  /* Set instructions to travel to preffered block dump location */
  // Get directions from home dispenser
  if(home_dispense){
    switch (current_block.face) {
      // North Face
      case 'n':
        arc_radius = 0.8;
        directions.AddTailNode('o', PI / 2, turn_time+1.2, arc_radius);
        if (current_block.pos == '1') {
          dist_final = guide1 - arc_radius - (wheel_dist / 2);
          directions.AddTailNode('d', dist_final, 2.5, 0, 'a', arm_max_angle, 3);
        } else if (current_block.pos == '2') {
          dist_final = guide2 - arc_radius - (wheel_dist / 2);
          directions.AddTailNode('k', 300, 0, 0, 'a', arm_max_angle, 2);
          directions.AddTailNode('d', 6, 1);
        } else if (current_block.pos == '3') {
          dist_final = guide3 - arc_radius - (wheel_dist / 2) - 0.5;
          directions.AddTailNode('d', dist_final, 3, 0, 'a', arm_max_angle, 3);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        if (current_block.elev == 'l') {
          directions.AddTailNode('d', -4, 1, 0, 'a', arm_collect_angle, 1);
        } else if (current_block.elev == 'u') {
          // directions.AddTailNode('d', -6, 0.5);
        }
        break;

      // East Face
      case 'e':
        arc_radius = 0.8;
        line_dist = false;
        directions.AddTailNode('o', PI / 2, turn_time+1.2, arc_radius);
        if (current_block.elev == 'l') {
          directions.AddTailNode('k', 300, 0, 0, 'a', arm_low_dump_angle, 3);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('k', 300, 0, 0, 'a', arm_max_angle, 3);
        }
        directions.AddTailNode('d', 5, 1);
        directions.AddTailNode('t', PI / 2, turn_time);
        if (current_block.pos == '4') {
          dist_final = guide4 - north_guide - arc_radius;
          directions.AddTailNode('l', dist_final);
        } else if (current_block.pos == '5') {
          dist_final = guide5 - north_guide - arc_radius;
          directions.AddTailNode('l', dist_final);
        } else if (current_block.pos == '6') {
          dist_final = guide6 - north_guide + arc_radius + 7;
          directions.AddTailNode('d', dist_final, 4);
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
          directions.AddTailNode('d', guide1+1, 3);
        } else if (current_block.pos == '2') {
          directions.AddTailNode('k', 300);
          directions.AddTailNode('d', 6, 1);
        } else if (current_block.pos == '3') {
          directions.AddTailNode('d', guide3, 4);
        }
        directions.AddTailNode('t', -PI / 2, turn_time);
        if(current_block.elev == 'l'){
          directions.AddTailNode('d', -6, 1, 0, 'a', arm_collect_angle, 2);
        } else if (current_block.elev == 'u'){
          directions.AddTailNode('d', -1, 1);
        }
        break;

      // West Face
      case 'w':
        line_dist = false;
        if(current_block.elev == 'l'){
          directions.AddTailNode('t', PI, turn_time+1, 0, 'a', arm_low_dump_angle, '1');
        } else if (current_block.elev == 'u'){
          directions.AddTailNode('t', PI, turn_time+1, 0, 'a', arm_max_angle, '1');
        }
        if (current_block.pos == '4') {
          if (current_block.elev == 'l') {
            directions.AddTailNode('l', guide4-north_guide+1, 0, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('l', guide4-north_guide+1);
          }
        } else if (current_block.pos == '5') {
          if (current_block.elev == 'l') {
            directions.AddTailNode('l', guide5-north_guide-6, 0, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('l', guide5-north_guide-6);
          }
        } else if (current_block.pos == '6') {
          if (current_block.elev == 'l') {
            directions.AddTailNode('d', guide6-north_guide, 4, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('d', guide6-north_guide, 4);
          }
        }
        directions.AddTailNode('t', -PI / 2, turn_time);
        break;
    } // end switch
  } // end if

  // Get directions from opponent dispenser
  else{
    switch (current_block.face) {
      // North Face
      case 'n':
        arc_radius = 0.8;
        directions.AddTailNode('o', -PI / 2, turn_time+1.2, arc_radius);
        if (current_block.pos == '1') {
          dist_final = guide3 - arc_radius - (wheel_dist / 2);
          directions.AddTailNode('d', dist_final, 2.5, 0, 'a', arm_max_angle, 3);
        } else if (current_block.pos == '2') {
          dist_final = guide2 - arc_radius - (wheel_dist / 2);
          directions.AddTailNode('k', 300, 0, 0, 'a', arm_max_angle, 2);
          directions.AddTailNode('d', 6, 1);
        } else if (current_block.pos == '3') {
          dist_final = guide1 - arc_radius - (wheel_dist / 2) - 0.5;
          directions.AddTailNode('d', dist_final, 3, 0, 'a', arm_max_angle, 3);
        }
        directions.AddTailNode('t', -PI / 2, turn_time);
        if (current_block.elev == 'l') {
          directions.AddTailNode('d', -4, 1, 0, 'a', arm_collect_angle, 1);
        } else if (current_block.elev == 'u') {
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
          if (current_block.elev == 'l') {
            directions.AddTailNode('l', guide4-north_guide+1, 0, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('l', guide4-north_guide+1);
          }
        } else if (current_block.pos == '5') {
          if (current_block.elev == 'l') {
            directions.AddTailNode('l', guide5-north_guide-6, 0, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('l', guide5-north_guide-6);
          }
        } else if (current_block.pos == '6') {
          if (current_block.elev == 'l') {
            directions.AddTailNode('d', guide6-north_guide, 4, 0, 's', shov_low_dump_angle, 2);
          } else if (current_block.elev == 'u') {
            directions.AddTailNode('d', guide6-north_guide, 4);
          }
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        break;

      // South Face
      case 's':
        line_dist = true;
        line_speed = 300;
        directions.AddTailNode('t', -PI, turn_time + 1);
        if(current_block.elev == 'l'){
          directions.AddTailNode('l', dist_to_wall, 0, 0, 'a', arm_low_dump_angle, 2);
        } else if (current_block.elev == 'u'){
          directions.AddTailNode('l', dist_to_wall, 0, 0, 'a', arm_max_angle, 2);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        if (current_block.pos == '1') {
          directions.AddTailNode('d', guide3+1, 3);
        } else if (current_block.pos == '2') {
          directions.AddTailNode('k', 300);
          directions.AddTailNode('d', 6, 1);
        } else if (current_block.pos == '3') {
          directions.AddTailNode('d', guide1, 3);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        if(current_block.elev == 'l'){
          directions.AddTailNode('d', -6, 1, 0, 'a', arm_collect_angle, 2);
        } else if (current_block.elev == 'u'){
          directions.AddTailNode('d', -1, 0.5);
        }
        break;

      // West Face
      case 'w':
        arc_radius = 0.8;
        line_dist = false;
        directions.AddTailNode('o', -PI / 2, turn_time+1.2, arc_radius);
        if (current_block.elev == 'l') {
          directions.AddTailNode('k', 300, 0, 0, 'a', arm_low_dump_angle, 3);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('k', 300, 0, 0, 'a', arm_max_angle, 3);
        }
        directions.AddTailNode('d', 5, 1);
        directions.AddTailNode('t', -PI / 2, turn_time);
        if (current_block.pos == '4') {
          dist_final = guide4 - north_guide + arc_radius;
          directions.AddTailNode('l', dist_final);
        } else if (current_block.pos == '5') {
          dist_final = guide5 - north_guide + arc_radius;
          directions.AddTailNode('l', dist_final);
        } else if (current_block.pos == '6') {
          dist_final = guide6 - north_guide + arc_radius;
          directions.AddTailNode('d', dist_final, 4);
        }
        directions.AddTailNode('t', -PI / 2, turn_time);
        break;
    } // end switch
  } // end else
}