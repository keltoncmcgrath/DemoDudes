void CollectBlock(void) {
  /* Set instructions to collect a block from dispenser */
  line_dist = false;
  // directions.AddTailNode('\0', 0, 0, 0, 's', shov_collect_angle, 0.2);
  // directions.AddTailNode('d', collect_dist, 0.75, 0, 'a', servo_home, 0.75);
  directions.AddTailNode('l', collect_dist, 75, 0, 'a', servo_home, 0.3);
  directions.AddTailNode('\0', 0, 0, 0, 's', shov_collect_angle, 0.01);
  directions.AddTailNode('d', -collect_dist, 1.5, 0, 'a', arm_collect_angle, 0.75);
  directions.AddTailNode('\0', 0, 0, 0, 'a', 35, 0.1); // 30
  directions.AddTailNode('\0', 0, 0, 0, 'a', arm_collect_angle, 0.1);
}