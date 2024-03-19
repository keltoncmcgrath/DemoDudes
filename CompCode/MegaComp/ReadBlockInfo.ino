void ReadBlockInfo(void){
  while(true){                                        // Wait until all blocks are read and compared
    if(Serial2.available()>=2){                       // If possible start flag and number of blocks are recieved
      if(Serial2.read() == flag){                     // If first byte is start flag
        num_blocks = Serial2.read() - '0';            // Read number of blocks
        while(true){                                    // Wait until all data is recieved
          if(Serial2.available() >= num_blocks*4){      // If all data is present
            for(int i=0; i<num_blocks; i++){            // Iterate thorugh each block
              read_block.face = Serial2.read();         // Read face value
              read_block.pos = Serial2.read();          // Read position value
              read_block.elev = Serial2.read();         // Read elevation value
              block_color = Serial2.read();             // Read Block Color


              // Cross off read block from desired block position array
              if(block_color == 'r'){                                                                                                 // If block is red
                for(int j=0; j<(sizeof(red1)/sizeof(struct block)); j++){                                                             // Loop over length of array
                  if(read_block.face==red1[j].face && read_block.pos==red1[j].pos && read_block.elev==red1[j].elev){                  // Check if all position identifiers are the same for first array
                    red1[j].placed = true;                                                                                            // If they are, changed placed bool to true
                  }
                  else if(read_block.face==red2[j].face && read_block.pos==red2[j].pos && read_block.elev==red2[j].elev){             // Check if all position identifiers are the same for first array
                    red2[j].placed = true;                                                                                            // If they are, changed placed bool to true
                  }
                }
              }
              else if(block_color == 'y'){                                                                                            // If block is yellow
                for(int j=0; j<(sizeof(yellow1)/sizeof(struct block)); j++){
                  if(read_block.face==yellow1[j].face && read_block.pos==yellow1[j].pos && read_block.elev==yellow1[j].elev){
                    yellow1[j].placed = true;
                  }
                  else if(read_block.face==yellow2[j].face && read_block.pos==yellow2[j].pos && read_block.elev==yellow2[j].elev){
                    yellow2[j].placed = true;
                  }
                }
              }
              else if(block_color == 'b'){                                                                                            // If block is blue
                for(int j=0; j<(sizeof(blue)/sizeof(struct block)); j++){
                  if(read_block.face==blue[j].face && read_block.pos==blue[j].pos && read_block.elev==blue[j].elev){
                    blue[j].placed = true;
                  }
                }
              }
              // End of checking block


            }
            break;
          }
        }
        break;
      }
    }
  }
}