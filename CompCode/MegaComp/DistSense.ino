void DistSense(void){
  dist_val = analogRead(dist_pin);
  dist_actual = pow(dist_val/a, 1/b);
  Serial.println(dist_actual);
}