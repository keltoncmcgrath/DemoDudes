void DumpExtraBlock(void) {
  directions.AddTailNode('d', -10, 0.5);
  if (home_dispense) {
    directions.AddTailNode('t', -PI / 2, turn_time);
  } else {
    directions.AddTailNode('t', PI / 2, turn_time);
  }
  directions.AddTailNode('d', 5, 0.3, 0, 's', shov_low_dump_angle - 20, 0.3);
  directions.AddTailNode('d', -5, 0.3, 0, 's', shov_collect_angle, 0.3);
  if (home_dispense) {
    directions.AddTailNode('t', PI / 2, turn_time);
  } else {
    directions.AddTailNode('t', -PI / 2, turn_time);
  }
}