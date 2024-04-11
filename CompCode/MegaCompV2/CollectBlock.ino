void CollectBlock(void) {
  /* Set instructions to collect a block from dispenser */
  directions.AddTailNode('\0', 0, 0, 0, 's', shov_collect_angle, 0.25);
  directions.AddTailNode('d', collect_dist, 0.75, 0, 'a', servo_home, 0.75);
  directions.AddTailNode('d', -collect_dist, 1.5, 0, 'a', arm_collect_angle, 0.75);
  directions.AddTailNode('\0', 0, 0, 0, 'a', 30, 0.1);
  directions.AddTailNode('\0', 0, 0, 0, 'a', arm_collect_angle, 0.1);
}