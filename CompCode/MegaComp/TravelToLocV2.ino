void TravelToLocV2(void) {
  switch (current_block.face) {
    // North Face
    case 'n':
      arc_radius = 0.8;
      directions.AddTailNode('o', pi / 2, turn_time, arc_radius);
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
      directions.AddTailNode('t', pi / 2, turn_time);
      break;

    // East Face
    case 'e':
      arc_radius = 0.8;
      dist_final = east_guide - arc_radius - (wheel_dist_arc / 2);
      directions.AddTailNode('o', pi / 2, turn_time, arc_radius);
      if (current_block.elev == 'l') {
        directions.AddTailNode('d', dist_final, 6, 0, 'a', arm_collect_angle, 3);
      } else if (current_block.elev == 'u') {
        directions.AddTailNode('d', dist_final, 6, 0, 'a', arm_max_angle, 3);
      }
      directions.AddTailNode('t', pi / 2, turn_time);
      if (current_block.pos == '4') {
        dist_final = guide4 - north_guide - arc_radius;
        directions.AddTailNode('l', dist_final, 4);
      } else if (current_block.pos == '5'){
        dist_final = guide5 - north_guide - arc_radius;
        directions.AddTailNode('l', dist_final, 4.5);
      } else if (current_block.pos == '6'){
        dist_final = guide6 - north_guide - arc_radius;
        directions.AddTailNode('l', dist_final, 5);
      }
        break;

    // South Face
    case 's':
      break;

    // West Face
    case 'w':
      break;
  }
}