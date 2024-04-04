void HallEffect(void) {
  for(int i=0; i<100; i++){
    mag_val = mag_alpha*analogRead(mag_pin) + (1-mag_alpha)*mag_val_last;
    mag_val_last = mag_val;
  }
}