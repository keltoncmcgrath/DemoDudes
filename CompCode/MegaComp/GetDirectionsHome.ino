void GetDirectionsHome(void) {
  switch (current_block.face) {
    case 'n':
      directions.AddTailNode('d', -6, 2);
      if(current_block.elev == 'l'){
        directions.AddTailNode('d', 6, 2, 0, 'a', arm_max_angle, 1);
      }
      directions.AddTailNode('t', PI/2, turn_time);
      if(current_block.pos == '1'){
        directions.AddTailNode('d', guide1, 4, 0, 'a', arm_collect_angle, 2);
      } else if (current_block.pos == '2'){
        directions.AddTailNode('d', guide2, 4.5, 0, 'a', arm_collect_angle, 2);
      } else if (current_block.pos == '3'){
        directions.AddTailNode('d', guide3, 5, 0, 'a', arm_collect_angle, 2);
      }
      directions.AddTailNode('t', PI/2, turn_time);
      break;

    case 'e':
      // Back up to black line (different distances if l or u)
      break;

    case 's':
      break;

    case 'w':
      // Back up to black line (different distances if l or u)
      break;
  }
}