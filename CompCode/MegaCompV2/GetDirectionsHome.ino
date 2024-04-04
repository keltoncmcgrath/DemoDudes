void GetDirectionsHome(void) {
  // Go to the home dispenser
  if(current_block.face != 'e' && current_block.pos != '3' && (current_block.face != 's' && current_block.pos != '2'){
    home_dispense = true;
    switch (current_block.face) {
      case 'n':
        line_dist = false;
        arc_radius = -4;
        if (current_block.elev == 'l') {
          directions.AddTailNode('d', -10, 1 , 0, 'a', arm_max_angle, 1.5);
          directions.AddTailNode('d', 10, 1);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('d', 5, 0.5);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        directions.AddTailNode('k', 300, 0, 0, 'a', arm_collect_angle, 2);
        directions.AddTailNode('d', arc_radius + wheel_dist_arc / 2 + 6, 1);
        directions.AddTailNode('o', -PI / 2, turn_time, arc_radius);
        directions.AddTailNode('d', -12, 0.5);
        break;

      case 's':
        directions.AddTailNode('d', -8, 2);
        directions.AddTailNode('t', -PI / 2, turn_time);
        if (current_block.pos == '1') {
          directions.AddTailNode('d', guide1, 3, 0, 'a', arm_collect_angle, 2);
        } else if (current_block.pos == '2') {
          directions.AddTailNode('d', guide2, 3.5, 0, 'a', arm_collect_angle, 2);
        } else if (current_block.pos == '3') {
          directions.AddTailNode('d', guide3, 4, 0, 'a', arm_collect_angle, 2);
        }
        directions.AddTailNode('t', PI / 2, turn_time);
        directions.AddTailNode('d', north_guide, 3);
        break;

      case 'w':
        directions.AddTailNode('k', -100);
        directions.AddTailNode('d', 5, 1);
        directions.AddTailNode('t', -PI / 2, turn_time);
        break;
    } // end switch
  } // end if

  // Go to opponent dispenser
  else{
    home_dispense = false;
    switch (current_block.face) {
      // North face
      case 'n':
        line_dist = false;
        arc_radius = -4;
        if (current_block.elev == 'l') {
          directions.AddTailNode('d', -10, 1 , 0, 'a', arm_max_angle, 1.5);
          directions.AddTailNode('d', 10, 1);
        } else if (current_block.elev == 'u') {
          directions.AddTailNode('d', 5, 0.5);
        }
        directions.AddTailNode('t', -PI / 2, turn_time);
        directions.AddTailNode('k', 300, 0, 0, 'a', arm_collect_angle, 2);
        directions.AddTailNode('d', -arc_radius + wheel_dist_arc / 2 + 6, 1);
        directions.AddTailNode('o', PI / 2, turn_time, arc_radius);
        directions.AddTailNode('d', -12, 0.5);
        break;

      // East face
      case 'e':
        line_dist = true;
        directions.AddTailNode('k', -100, 0, 0, 'a', arm_collect_angle, 2);
        directions.AddTailNode('d', 5, 1);
        directions.AddTailNode('t', PI / 2, turn_time);
        break;

      // South face
      case 's':
        directions.AddTailNode('d', -8, 2);
        directions.AddTailNode('t', PI / 2, turn_time);
        directions.AddTailNode('k', 300, 0, 0, 'a', arm_collect_angle, 2);
        directions.AddTailNode('d', 5, 1);
        directions.AddTailNode('t', -PI / 2, turn_time);
        break;
    }// end switch
  } // end else
  
}