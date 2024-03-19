void ColorSense(){
  // Reading Red Light 
  digitalWrite(red_pin, LOW);
  delay(color_delay_time);
  red_val = analogRead(photo_trans_pin);
  digitalWrite(red_pin, HIGH);

  // Reading Green Light
  digitalWrite(green_pin, LOW);
  delay(color_delay_time);
  green_val = analogRead(photo_trans_pin);
  digitalWrite(green_pin, HIGH);

  // Reading Blue Light
  digitalWrite(blue_pin, LOW);
  delay(color_delay_time);
  blue_val = analogRead(photo_trans_pin);
  digitalWrite(blue_pin,HIGH);

  // Identify Color of Block
  for(int i=0; i<3; i++){
    if(red_val > color_ranges[i][0][0] & red_val < color_ranges[i][0][1]){
      if(green_val > color_ranges[i][1][0] & green_val < color_ranges[i][1][1]){
        if(blue_val > color_ranges[i][2][0] & blue_val < color_ranges[i][2][1]){
          is_color = true;
          if(i==0){
            current_block.color = 'r';
          }
          else if(i==1){
            current_block.color = 'y';
          }
          else{
            current_block.color = 'b';
          }
        }
      }    
    }
  }
  // Block Has No Color
  if(!is_color){
    current_block.color = '\0';
  }
  is_color = false;
}