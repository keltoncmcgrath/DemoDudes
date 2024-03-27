void GetDirectionsHome(void) {
  switch (current_block.face) {
    case 'n':
      directions.AddTailNode('d', -6, 2);
      if (current_block.elev == 'l') {
        directions.AddTailNode('d', 6, 2, 0, 'a', arm_max_angle, 1);
      }
      directions.AddTailNode('t', PI / 2, turn_time);
      if (current_block.pos == '1') {
        directions.AddTailNode('d', guide1, 4, 0, 'a', arm_collect_angle, 2);
      } else if (current_block.pos == '2') {
        directions.AddTailNode('d', guide2, 4.5, 0, 'a', arm_collect_angle, 2);
      } else if (current_block.pos == '3') {
        directions.AddTailNode('d', guide3, 5, 0, 'a', arm_collect_angle, 2);
      }
      directions.AddTailNode('t', PI / 2, turn_time);
      break;

    case 'e':
      line_dist = false;
      directions.AddTailNode('k', -100);
      directions.AddTailNode('t', PI / 2, turn_time);
      if (current_block.pos == '4') {
        directions.AddTailNode('l', guide4 - north_guide - 3);
      } else if (current_block.pos == '5') {
        directions.AddTailNode('l', guide5 - north_guide - 3);
      } else if (current_block.pos == '6') {
        directions.AddTailNode('l', guide6 - north_guide - 3);
      }
      directions.AddTailNode('t', -PI / 2, turn_time);
      directions.AddTailNode('k', 300, 0, 0, 'a', arm_collect_angle, 2);
      directions.AddTailNode('t', PI / 2, turn_time);
      break;

    case 's':
      break;

    case 'w':
      // Back up to black line (different distances if l or u)
      break;
  }
}