void DistSense(void){
  for(int i=0; i<num_dist_vals; i++){
    dist_val += analogRead(dist_pin);
  }
  dist_val /= num_dist_vals;
  dist_actual = pow(dist_val/a, 1/b);
  dist_val = 0;
}