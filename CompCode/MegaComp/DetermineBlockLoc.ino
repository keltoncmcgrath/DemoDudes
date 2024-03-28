void DetermineBlockLoc(void) {
  // Red Blocks
  if (current_block.color == 'r') {
    if (use_first) {
      for (int i = 0; i < (sizeof(red1) / sizeof(struct block)); i++) {
        if (!red1[i].placed) {
          current_block.face = red1[i].face;
          current_block.pos = red1[i].pos;
          current_block.elev = red1[i].elev;
          red1[i].placed = true;
          break;
        }
      }
    } else {
      for (int i = 0; i < (sizeof(red2) / sizeof(struct block)); i++) {
        if (!red2[i].placed) {
          current_block.face = red2[i].face;
          current_block.pos = red2[i].pos;
          current_block.elev = red2[i].elev;
          red2[i].placed = true;
          break;
        }
      }
    }
  }

  // Yellow Blocks
  else if (current_block.color == 'y') {  // If block is yellow
    if (use_first) {
      for (int i = 0; i < (sizeof(yellow1) / sizeof(struct block)); i++) {
        if (!yellow1[i].placed) {
          current_block.face = yellow1[i].face;
          current_block.pos = yellow1[i].pos;
          current_block.elev = yellow1[i].elev;
          yellow1[i].placed = true;
          break;
        }
      }
    } else {
      for (int i = 0; i < (sizeof(yellow2) / sizeof(struct block)); i++) {
        if (!yellow2[i].placed) {
          current_block.face = yellow2[i].face;
          current_block.pos = yellow2[i].pos;
          current_block.elev = yellow2[i].elev;
          yellow2[i].placed = true;
          break;
        }
      }
    }
  }

  // Blue Blocks
  else if (current_block.color == 'b') {  // If block is blue
    if (use_first) {
      for (int i = 0; i < (sizeof(blue) / sizeof(struct block)); i++) {
        if (!blue[i].placed) {
          current_block.face = blue[i].face;
          current_block.pos = blue[i].pos;
          current_block.elev = blue[i].elev;
          blue[i].placed = true;
          break;
        }
      }
    }
  }
}