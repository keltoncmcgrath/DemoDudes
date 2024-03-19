void TravelToLoc(void){
  // 
  switch (current_block.face){
    case 'n':
      if (turn_bool){
        turn_dir = 'r';
        turn_angle_final = pi;
        Turn(); // fix, change turn_bool to false and striaght_line_bool to true
      }

      if(straight_line_bool){
        if(current_block.pos == '1'){
          dist_final = guide1;
          time_final = 5;
          StraightLine();
        }
        else if(current_block.pos == '2'){
          // travel straight until black line sensed
        }
        else{
          dist_final = guide3;
          time_final = 6;
          StraightLine();
        }
      }



      break;

    case 'e':
      break;

    case 's':
      break;
    
    case 'w':
      break;
  }
}