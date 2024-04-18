void ColorSense(){
  for(int i=0; i<color_samples_sense; i++){
    // Reading Red Light 
    digitalWrite(red_pin, LOW);
    delay(color_delay_time);
    red_val = analogRead(photo_trans_pin);
    digitalWrite(red_pin, HIGH);
    red_valf = color_sense_alpha*red_val + (1-color_sense_alpha)*red_valf;

    // Reading Green Light
    digitalWrite(green_pin, LOW);
    delay(color_delay_time);
    green_val = analogRead(photo_trans_pin);
    digitalWrite(green_pin, HIGH);
    green_valf = color_sense_alpha*green_val + (1-color_sense_alpha)*green_valf;

    // Reading Blue Light
    digitalWrite(blue_pin, LOW);
    delay(color_delay_time);
    blue_val = analogRead(photo_trans_pin);
    digitalWrite(blue_pin,HIGH);
    blue_valf = color_sense_alpha*blue_val + (1-color_sense_alpha)*blue_val;
  }
  Serial.print(red_valf);
  Serial.print('\t');
  Serial.print(green_valf);
  Serial.print('\t');
  Serial.println(blue_valf);

  for(int i=0; i<4; i++){
    if(red_valf > color_ranges[i][0][0] & red_valf < color_ranges[i][0][1]){
      if(green_valf > color_ranges[i][1][0] & green_valf < color_ranges[i][1][1]){
        if(blue_valf > color_ranges[i][2][0] & blue_valf < color_ranges[i][2][1]){
          is_color = true;
          if(i==0){
            current_block.color = 'r';
            break;
          } else if(i==1){
            current_block.color = 'y';
            break;
          } else if (i == 2){
            current_block.color = 'b';
            break;
          } else {
            current_block.color = 'x';
            break;
          }
        }
      }
    }
  }
  if(!is_color){
    Serial.println("Block Color Not Found");
  }
  is_color = false;
  red_val, red_valf = 0;
  green_val, green_valf = 0;
  blue_val, blue_valf = 0;
}