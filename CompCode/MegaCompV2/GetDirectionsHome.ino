void GetDirectionsHome(void) {
  // Go to the home dispenser
  if (current_block.face == 'w' || current_block.pos == '1' || (current_block.face == 'n' && current_block.pos == '2')) {
    home_dispense = true;
    switch (current_block.face) {
      // North Face
      case 'n':
        line_dist = false;
        arc_radius = -2;
        if (current_block.elev == 'l') {
          directions.AddTailNode('d', -10, 1, 0, 'a', arm_max_angle, 1.5);
          directions.AddTailNode('d', 10, 1);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('d', 5, 0.5);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        directions.AddTailNode('k', line_follow_speed, 0, 0, 'a', servo_home, 2);
        directions.AddTailNode('d', abs(arc_radius) + wheel_dist / 2 + ir_to_wheel, 1);
        directions.AddTailNode('o', -PI / 2, arc_time_big, arc_radius);
        directions.AddTailNode('d', -20, 0.5);
        break;

      // South Face
      case 's':
        directions.AddTailNode('d', -7, 0.8);
        directions.AddTailNode('t', -PI / 2, turn_time);
        if (current_block.pos == '1') {
          directions.AddTailNode('d', guide1, 1.5, 0, 'a', servo_home, 2);
        } else if (current_block.pos == '2') {
          directions.AddTailNode('d', guide2, 1.75, 0, 'a', servo_home, 2);
        } else if (current_block.pos == '3') {
          directions.AddTailNode('d', guide3, 2, 0, 'a', servo_home, 2);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        directions.AddTailNode('d', north_guide-10, 0.8);
        break;

      // West Face
      case 'w':
        directions.AddTailNode('k', -line_follow_speed, 0, 0, 'a', servo_home, 2);
        directions.AddTailNode('d', ir_to_wheel, 0.5);
        directions.AddTailNode('t', -PI / 2, turn_time);
        break;
    } // end switch
  } // end if

  // Go to opponent dispenser
  else {
    home_dispense = false;
    switch (current_block.face) {
      // North face
      case 'n':
        line_dist = false;
        arc_radius = -2;
        if (current_block.elev == 'l') {
          directions.AddTailNode('d', -10, 1, 0, 'a', arm_max_angle, 1.5);
          directions.AddTailNode('d', 10, 1);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('d', 5, 0.5);
        }
        directions.AddTailNode('t', -PI / 2, turn_time);
        directions.AddTailNode('k', line_follow_speed, 0, 0, 'a', servo_home, 2);
        directions.AddTailNode('d', abs(arc_radius) + wheel_dist / 2 + ir_to_wheel, 1);
        directions.AddTailNode('o', PI / 2, arc_time_big, arc_radius);
        directions.AddTailNode('d', -20, 0.5);
        break;

      // East face
      case 'e':
        line_dist = true;
        directions.AddTailNode('k', -line_follow_speed, 0, 0, 'a', servo_home, 2);
        directions.AddTailNode('d', ir_to_wheel, 0.5);
        directions.AddTailNode('t', PI / 2, turn_time);
        break;

      // South face
      case 's':
        directions.AddTailNode('d', -6, 0.5);
        directions.AddTailNode('t', PI / 2, turn_time);
        directions.AddTailNode('k', line_follow_speed, 0, 0, 'a', servo_home, 2);
        directions.AddTailNode('d', ir_to_wheel, 0.5);
        directions.AddTailNode('t', -PI / 2, turn_time);
        break;
    } // end switch
  } // end else
} // end function