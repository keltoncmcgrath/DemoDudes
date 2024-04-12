void DistSenseRight(void) {
  t = (millis() - t_start) / 1000;
  for(int i=0; i<num_dist_vals; i++){
    dist_val = analogRead(dist_pin_right);
    dist_valf += dist_alpha*dist_val + (1-dist_alpha)*dist_valf;
  }
  dist_actual = pow(dist_val/a1, 1/b1);
  dist_actual = constrain(dist_actual, 0, 1000);
  dist_actualf = dist_actual_alpha*dist_actual + (1-dist_actual_alpha)*dist_actualf;
  dist_valf = 0;
}