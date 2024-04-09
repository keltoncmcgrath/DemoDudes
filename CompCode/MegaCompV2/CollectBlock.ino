void CollectBlock(void) {
  /* Set instructions to collect a block from dispenser */
  
  directions.AddTailNode('d', collect_dist, 0.75, 0, 's', shov_collect_angle, 0.5);
  directions.AddTailNode('d', -collect_dist+0.1, 0.75);
}