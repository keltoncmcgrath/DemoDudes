void HallEffect(void) {
  for(int i=0; i<100; i++){
    mag_val = alpha*analogRead(mag_pin) + (1-alpha)*mag_val_last;
    mag_val_last = mag_val;
  }
}