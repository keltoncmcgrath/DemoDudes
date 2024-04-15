void ReadEncoderDist(void) {
  counts1 = encoder1.read();
  counts2 = encoder2.read();
  dist_traveled = ((float(counts1+counts2)/2) / float(counts_per_rev * gear_ratio)) * 2 * PI * wheel_radius;
}