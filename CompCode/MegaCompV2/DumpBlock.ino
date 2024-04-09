void DumpBlock(void) {
  switch (current_block.pos) {
    // Block placed on guidelines
    case '1':
    case '3':
    case '4':
    case '6':
      line_dist = true;
      if (current_block.elev == 'l') {
        directions.AddTailNode('r', dump_dist_lower, 0, 0, 's', shov_low_dump_angle, 1);
      } else if (current_block.elev == 'u') {
        directions.AddTailNode('r', dump_dist_upper, 0);
        directions.AddTailNode('\0', 0, 0, 0, 's', shov_dump_angle, 1);
        directions.AddTailNode('\0', 0, 0, 0, 's', shov_collect_angle, 1);
      }
      break;

    // Block placed on black line
    case '2':
    case '5':
      line_dist = true;
      if (current_block.elev == 'l') {
        line_dist = true;
        directions.AddTailNode('l', dump_dist_lower, 0, 0, 's', shov_low_dump_angle, 1);
      } else if (current_block.elev == 'u') {
        directions.AddTailNode('l', dump_dist_upper, 0);
        directions.AddTailNode('\0', 0, 0, 0, 's', shov_dump_angle, 1);
        directions.AddTailNode('\0', 0, 0, 0, 's', shov_collect_angle, 1);
      }
      break;
  }
}