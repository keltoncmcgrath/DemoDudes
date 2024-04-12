void ColorCalibration(){
  for(int j=0; j<4; j++){
    if(j == 0){
      Serial.println("Place Red Block on Sensor");
      shovel_servo.write(110);
      delay(100);
      shovel_servo.write(120);
    }
    else if(j == 1){
      Serial.println("Place Yellow Block on Sensor");
      shovel_servo.write(110);
      delay(100);
      shovel_servo.write(120);
    }
    else if (j == 2){
      Serial.println("Place Blue Block on Sensor");
      shovel_servo.write(110);
      delay(100);
      shovel_servo.write(120);
    } else {
      Serial.println("Take Block off of Sensor");
      shovel_servo.write(110);
      delay(100);
      shovel_servo.write(120);
    }
    delay(5000);
    Serial.println("Reading Color Values...");
    for(int i=0; i<color_samples; i++){
      digitalWrite(red_pin, LOW);
      delay(color_delay_time);
      red_val = analogRead(photo_trans_pin);
      red_calibration_vals[i] = red_val;
      red_sum += red_val;
      digitalWrite(red_pin, HIGH);

      digitalWrite(green_pin, LOW);
      delay(color_delay_time);
      green_val = analogRead(photo_trans_pin);
      green_calibration_vals[i] = green_val;
      green_sum += green_val;
      digitalWrite(green_pin, HIGH);

      digitalWrite(blue_pin, LOW);
      delay(color_delay_time);
      blue_val = analogRead(photo_trans_pin);
      blue_calibration_vals[i] = blue_val;
      blue_sum += blue_val;
      digitalWrite(blue_pin, HIGH);    
    }

    // Calculate Avgerages
    red_avg[j] = red_sum/color_samples;
    green_avg[j] = green_sum/color_samples;
    blue_avg[j] = blue_sum/color_samples;

    // Calculate Standard Deviations
    for(int i=0; i<color_samples; i++){
      red_std[j] += pow((red_calibration_vals[i] - red_avg[j]), 2);
      green_std[j] += pow((green_calibration_vals[i] - green_avg[j]), 2);
      blue_std[j] += pow((blue_calibration_vals[i] - blue_avg[j]), 2);
    }
    red_std[j] = sqrt(red_std[j]/(color_samples-1));
    green_std[j] = sqrt(green_std[j]/(color_samples-1));
    blue_std[j] = sqrt(blue_std[j]/(color_samples-1));

    // Set Sums to Zero
    red_sum = 0;
    green_sum = 0;
    blue_sum = 0;

    // Find Ranges of Values for Each Block
    for(int i=0; i<4; i++){
      if(i == 4){
        // Red
        color_ranges[i][0][0] = red_avg[i]-6*red_std[i];
        color_ranges[i][0][1] = red_avg[i]+3*red_std[i];
        // Green
        color_ranges[i][1][0] = green_avg[i]-6*green_std[i];
        color_ranges[i][1][1] = green_avg[i]+3*green_std[i];
        // Blue
        color_ranges[i][2][0] = blue_avg[i]-6*blue_std[i];
        color_ranges[i][2][1] = blue_avg[i]+3*blue_std[i];
      } else {
        // Red
        color_ranges[i][0][0] = red_avg[i]-4*red_std[i];
        color_ranges[i][0][1] = red_avg[i]+4*red_std[i];
        // Green
        color_ranges[i][1][0] = green_avg[i]-4*green_std[i];
        color_ranges[i][1][1] = green_avg[i]+4*green_std[i];
        // Blue
        color_ranges[i][2][0] = blue_avg[i]-4*blue_std[i];
        color_ranges[i][2][1] = blue_avg[i]+4*blue_std[i];
      }
    }
  }
  for(int i=0; i<4; i++){
    Serial.print(color_ranges[i][0][0]);
    Serial.print('\t');
    Serial.println(color_ranges[i][0][1]);

    Serial.print(color_ranges[i][1][0]);
    Serial.print('\t');
    Serial.println(color_ranges[i][1][1]);

    Serial.print(color_ranges[i][2][0]);
    Serial.print('\t');
    Serial.println(color_ranges[i][2][1]);
    Serial.println();
  }
}