void DistSenseRight(void) {
  for(int i=0; i<num_dist_vals; i++){
    dist_val += dist_alpha*analogRead(dist_pin_right) + (1-dist_alpha)*last_dist_val;
  }
  dist_actual = pow(dist_val/a1, 1/b1);
  dist_val = 0;
}