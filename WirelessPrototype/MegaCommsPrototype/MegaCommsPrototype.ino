// Block Data Storage
char *red1[] = {"w4l", "w5l", "w6l", "e4l", "e5l", "e6l", "n2u", "w5u", "s2u", "e5u"};
char *red2[] = {"n1l", "n2l", "n3l", "s1l", "s2l", "s3l", "n2u", "w5u", "s2u", "e5u"};
char *yellow1[] = {"n1l", "n2l", "n3l", "s1l", "s2l", "s3l", "n2u", "w5u", "s2u", "e5u"};
char *yellow2[] = {"w4l", "w5l", "w6l", "e4l", "e5l", "e6l", "n2u", "w5u", "s2u", "e5u"};
char *blue[] = {"n1u", "n3u", "s1u", "s3u", "n2u", "w5u", "s2u", "e5u"};
// IMPORTANT: If blue block on upper middle mags, wont cancel same loc on other arrays

// Variables
int num_blocks;
int flag = 33;
char rc[4] = {};

// Determine Block Location Function
void BlockLoc(char rc[4]){
  // Change optimal red locations if red block on chassis
  if(rc[3] == 'r'){
    for(int i=0; i<6; i++){                                                            // Loop over strings in red arrays
      if((rc[0]==red1[i][0]) && (rc[1]==red1[i][1]) && (rc[2]==red1[i][2])){           // Match red block on chassis to first red choices
        red1[i][0] = 88;
        red1[i][1] = 88;
        red1[i][2] = 88;
      }
      else if((rc[0]==red2[i][0]) && (rc[1]==red2[i][1]) && (rc[2]==red2[i][2])){      // Match red block on chassis to second red choices
        red2[i][0] = 88;
        red2[i][1] = 88;
        red2[i][2] = 88;
      }
    }
  }
  // Change optimal yellow locations if yellow block on chassis
  else if(rc[3] == 'y'){
    for(int i=0; i<6; i++){                                                            // Loop over strings in red arrays
      if((rc[0]==yellow1[i][0]) && (rc[1]==yellow1[i][1]) && (rc[2]==yellow1[i][2])){           // Match red block on chassis to first red choices
        yellow1[i][0] = 88;
        yellow1[i][1] = 88;
        yellow1[i][2] = 88;
      }
      else if((rc[0]==yellow2[i][0]) && (rc[1]==yellow2[i][1]) && (rc[2]==yellow2[i][2])){      // Match red block on chassis to second red choices
        yellow2[i][0] = 88;
        yellow2[i][1] = 88;
        yellow2[i][2] = 88;
      }
    }
  }
  // Change optimal blue locations if yellow blue on chassis
  else if(rc[3] == 'b'){
    for(int i=0; i<4; i++){                                                            // Loop over strings in red arrays
      if((rc[0]==blue[i][0]) && (rc[1]==blue[i][1]) && (rc[2]==blue[i][2])){           // Match red block on chassis to first red choices
        blue[i][0] = 88;
        blue[i][1] = 88;
        blue[i][2] = 88;
      }
    }
  }
}


void setup() {
  // Begin serial communication
  Serial.begin(9600);
  Serial2.begin(9600);
}


void loop() {
  // Read incoming block information
  if(Serial2.available()>=2){                         // Checks for start flag and number of blocks
    if(Serial2.read() == flag){                       // Checks if the first byte is the start flag
      num_blocks = Serial2.read() - '0';              // Store number of blocks
      while(true){                                    // While we know data is sending
        if(Serial2.available()>=num_blocks*4){        // If all the data is sent
          for(int i=0; i<num_blocks; i++){            // Read the data block by block
            rc[0] = Serial2.read();
            rc[1] = Serial2.read();
            rc[2] = Serial2.read();
            rc[3] = Serial2.read();
            BlockLoc(rc);
          }
          break;
        }
      }
    }
  }
//   if(Serial2.available()){
//     Serial.write(rc);
//   }
}
