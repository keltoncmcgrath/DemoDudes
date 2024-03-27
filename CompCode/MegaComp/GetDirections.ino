void GetDirections(void) {
  /* Set instructions to travel to preffered block dump location */
  switch (current_block.face) {
    // North Face
    case 'n':
      arc_radius = 0.8;
      directions.AddTailNode('o', PI / 2, turn_time, arc_radius);
      if (current_block.pos == '1') {
        dist_final = guide1 - arc_radius - (wheel_dist_arc / 2);
        directions.AddTailNode('d', dist_final, 4, 0, 'a', arm_max_angle, 3);
      } else if (current_block.pos == '2') {
        dist_final = guide2 - arc_radius - (wheel_dist_arc / 2);
        directions.AddTailNode('d', dist_final, 4.5, 0, 'a', arm_max_angle, 3);
      } else if (current_block.pos == '3') {
        dist_final = guide3 - arc_radius - (wheel_dist_arc / 2);
        directions.AddTailNode('d', dist_final, 5, 0, 'a', arm_max_angle, 3);
      }
      directions.AddTailNode('t', PI / 2, turn_time);
      if (current_block.elev == 'l') {
        directions.AddTailNode('d', -6, 2, 0, 'a', arm_collect_angle, 2);
      } else if (current_block.elev == 'l') {
        directions.AddTailNode('d', -4, 2);
      }
      break;

    // East Face
    case 'e':
      arc_radius = 0.8;
      dist_final = east_guide - arc_radius - (wheel_dist_arc / 2);
      line_dist = false;
      directions.AddTailNode('o', PI / 2, turn_time, arc_radius);
      if (current_block.elev == 'l') {
        directions.AddTailNode('d', dist_final, 6, 0, 'a', arm_collect_angle, 3);
      } else if (current_block.elev == 'u') {
        directions.AddTailNode('d', dist_final, 6, 0, 'a', arm_max_angle, 3);
      }
      directions.AddTailNode('t', PI / 2, turn_time);
      if (current_block.pos == '4') {
        dist_final = guide4 - north_guide - arc_radius;
        directions.AddTailNode('l', dist_final);
      } else if (current_block.pos == '5') {
        dist_final = guide5 - north_guide - arc_radius;
        directions.AddTailNode('l', dist_final);
      } else if (current_block.pos == '6') {
        dist_final = guide6 - north_guide - arc_radius;
        directions.AddTailNode('l', dist_final);
      }
      directions.AddTailNode('t', PI / 2, turn_time);
      break;

    // South Face
    case 's':
      line_speed = 300;
      directions.AddTailNode('t', PI, turn_time + 1);
      if (current_block.elev == 'l') {
        directions.AddTailNode('r', dist_to_wall, 0, 0, 'a', arm_collect_angle, 2);
      } else if (current_block.elev == 'u') {
        directions.AddTailNode('r', dist_to_wall, 0, 0, 'a', arm_max_angle, 2);
      }
      directions.AddTailNode('t', -PI / 2, turn_time);
      if (current_block.pos == '1') {
        directions.AddTailNode('d', guide1, 3);
      } else if (current_block.pos == '2') {
        directions.AddTailNode('d', guide2, 3.5);
      } else if (current_block.pos == '3') {
        directions.AddTailNode('d', guide3, 4);
      }
      directions.AddTailNode('t', -PI / 2, turn_time);
      break;

    // West Face
    case 'w':
      line_dist = false;
      directions.AddTailNode('t', PI, turn_time);
      if (current_block.pos == '4') {
        if (current_block.elev == 'l') {
          directions.AddTailNode('l', guide4, 0, 0, 'a', arm_collect_angle, 2);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('l', guide4, 0, 0, 'a', arm_max_angle, 2);
        }
      } else if (current_block.pos == '5') {
        if (current_block.elev == 'l') {
          directions.AddTailNode('l', guide5, 0, 0, 'a', arm_collect_angle, 2);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('l', guide5, 0, 0, 'a', arm_max_angle, 2);
        }
      } else if (current_block.pos == '6') {
        if (current_block.elev == 'l') {
          directions.AddTailNode('l', guide6, 0, 0, 'a', arm_collect_angle, 2);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('l', guide6, 0, 0, 'a', arm_max_angle, 2);
        }
      }
      directions.AddTailNode('t', -PI / 2, turn_time);
      break;
  }
}