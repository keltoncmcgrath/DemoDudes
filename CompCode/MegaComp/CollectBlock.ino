void CollectBlock(void) {
  /* Set instructions to collect a block from dispenser */
  
  directions.AddTailNode('d', collect_dist, 1.5, 0, 's', shov_collect_angle, 1);
  directions.AddTailNode('d', -collect_dist+0.1, 1.5);
}