void CollectBlock(void) {
  /* Set instructions to collect a block from dispenser */
  directions.AddTailNode('d', collect_dist, 0.75, 0, 's', shov_collect_angle, 0.5);
  directions.AddTailNode('d', -collect_dist+0.1, 0.75);
  directions.AddTailNode('\0', 0, 0, 0, 'a', 40, 0.1);
  directions.AddTailNode('\0', 0, 0, 0, 'a', arm_collect_angle, 0.1);
}