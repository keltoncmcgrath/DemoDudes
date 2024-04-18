void ReadBlockInfo(void) {
  while (true) {                                            // Wait until all blocks are read and compared
    if (Serial2.available() >= 3) {                         // If possible start flag and number of blocks are recieved
      num_blocks = Serial2.readStringUntil(flag).toInt(); // Read number of blocks
      Serial.println(num_blocks);
      while (true) {                                      // Wait until all data is recieved
        if (Serial2.available() >= num_blocks * 4) {      // If all data is present
          for (int i = 0; i < num_blocks; i++) {          // Iterate thorugh each block
            read_block.face = Serial2.read();             // Read face value
            read_block.pos = Serial2.read();              // Read position value
            read_block.elev = Serial2.read();             // Read elevation value
            read_block.color = Serial2.read();            // Read Block Color

            // Cross off read block from desired block position array
            for (int j = 0; j < (sizeof(red1) / sizeof(struct block)); j++) {                                                    // Loop over length of array
              if (read_block.face == red1[j].face && read_block.pos == red1[j].pos && read_block.elev == red1[j].elev) {         // Check if all position identifiers are the same for first array
                red1[j].placed = true;                                                                                           // If they are, changed placed bool to true
                red1[j].color = read_block.color;                                                                                // And change color to block color
              } else if (read_block.face == red2[j].face && read_block.pos == red2[j].pos && read_block.elev == red2[j].elev) {  // Check if all position identifiers are the same for first array
                red2[j].placed = true;                                                                                           // If they are, changed placed bool to true
                red2[j].color = read_block.color;                                                                                // And change color to block color
              } // end if else
            } // end red for
            for (int j = 0; j < (sizeof(yellow1) / sizeof(struct block)); j++) {
              if (read_block.face == yellow1[j].face && read_block.pos == yellow1[j].pos && read_block.elev == yellow1[j].elev) {
                yellow1[j].placed = true;
                yellow1[j].color = read_block.color;
              } else if (read_block.face == yellow2[j].face && read_block.pos == yellow2[j].pos && read_block.elev == yellow2[j].elev) {
                yellow2[j].placed = true;
                yellow2[j].color = read_block.color;
              } // end if else
            } // end yellow for
            for (int j = 0; j < (sizeof(blue) / sizeof(struct block)); j++) {
              if (read_block.face == blue[j].face && read_block.pos == blue[j].pos && read_block.elev == blue[j].elev) {
                blue[j].placed = true;
                blue[j].color = read_block.color;
              } // end if
            } // end blue for
          } // end numblock for
          break;
        } // end serial if
      } // end while

      // Determine which block loc array to use
      for (int i = 0; i < (sizeof(red1)/sizeof(struct block)); i++) {    // Iterate through desired red block locations
        if (red1[i].placed) {            // If there is a block placed in red1
          if (red1[i].color == 'r') {    // If that block is red
            use_first = true;            // Use red1
            break;
          } else {                       // Else if that block is not red
            use_first = false;           // Use red2
            break;
          } // end if else
        } else if (red2[i].placed) {     // Iterate through desired red block locations
          // if (red2[i].placed) {          // If there is a block placed in red2
          if (red2[i].color == 'r') {  // If that block is red
            use_first = false;         // Use red2
            break;
          } else {                     // Else if that block is not red
            use_first = true;          // Use red1
            break;
          } // end red2 color if else
        } // end red1/red2 placed if else
      } // end for
      break;
    } // end serial>=2 if
  } // end while
} // end func