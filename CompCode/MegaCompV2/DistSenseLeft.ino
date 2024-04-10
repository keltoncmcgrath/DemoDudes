void DistSenseLeft(void) {
  for(int i=0; i<num_dist_vals; i++){
    dist_val += dist_alpha*analogRead(dist_pin_left) + (1-dist_alpha)*last_dist_val;
  }
  dist_actual = pow(dist_val/a2, 1/b2);
  dist_val = 0;
}