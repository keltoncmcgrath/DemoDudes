void DetermineBlockLoc(void) {
  // Red Blocks
  if (current_block.color == 'r') {
    if (use_first) {
      if (home_dispense) { // Pick west face blocks if at home dispenser
        for (int i = 0; i < (sizeof(red1) / sizeof(struct block)); i++) {
          if (!red1[i].placed && red1[i].face == 'w'){
            current_block.face = red1[i].face;
            current_block.pos = red1[i].pos;
            current_block.elev = red1[i].elev;
            red1[i].placed = true;
            break;
          } else if (!red1[i].placed && red1[i].pos == '1') {
            current_block.face = red1[i].face;
            current_block.pos = red1[i].pos;
            current_block.elev = red1[i].elev;
            red1[i].placed = true;
            break;
          } // end if
        } // end for
      } else if (!home_dispense) { // Pick east face blocks if at opponent dispenser
        for (int i = 0; i < (sizeof(red1) / sizeof(struct block)); i++) {
          if (!red1[i].placed && red1[i].face == 'e'){
            current_block.face = red1[i].face;
            current_block.pos = red1[i].pos;
            current_block.elev = red1[i].elev;
            red1[i].placed = true;
            break;
          } else if (!red1[i].placed && red1[i].pos == '3') {
            current_block.face = red1[i].face;
            current_block.pos = red1[i].pos;
            current_block.elev = red1[i].elev;
            red1[i].placed = true;
            break;
          } // end if
        } // end for
      } // end dispense if else
      if (current_block.face == '\0'){
        for (int i = 0; i < (sizeof(red1) / sizeof(struct block)); i++) {
          if (!red1[i].placed) {
            current_block.face = red1[i].face;
            current_block.pos = red1[i].pos;
            current_block.elev = red1[i].elev;
            red1[i].placed = true;
            break;
          } // end if
        } // end for
      } // end if
    } else { // Use second set of red arrays
      if (home_dispense) { // Pick west face blocks if at home dispenser
        for (int i = 0; i < (sizeof(red2) / sizeof(struct block)); i++) {
          if (!red2[i].placed && red2[i].face == 'w'){
            current_block.face = red2[i].face;
            current_block.pos = red2[i].pos;
            current_block.elev = red2[i].elev;
            red2[i].placed = true;
            break;
          } else if (!red2[i].placed && red2[i].pos == '1') {
            current_block.face = red2[i].face;
            current_block.pos = red2[i].pos;
            current_block.elev = red2[i].elev;
            red2[i].placed = true;
            break;
          } // end if
        } // end for
      } else if (!home_dispense) { // Pick east face blocks if at opponent dispenser
        for (int i = 0; i < (sizeof(red2) / sizeof(struct block)); i++) {
          if (!red2[i].placed && red2[i].face == 'e'){
            current_block.face = red2[i].face;
            current_block.pos = red2[i].pos;
            current_block.elev = red2[i].elev;
            red2[i].placed = true;
            break;
          } else if (!red2[i].placed && red2[i].pos == '1') {
            current_block.face = red2[i].face;
            current_block.pos = red2[i].pos;
            current_block.elev = red2[i].elev;
            red2[i].placed = true;
            break;
          } // end if
        } // end for
      } // end dispense if else
      if (current_block.face == '\0'){
        for (int i = 0; i < (sizeof(red2) / sizeof(struct block)); i++) {
          if (!red2[i].placed) {
            current_block.face = red2[i].face;
            current_block.pos = red2[i].pos;
            current_block.elev = red2[i].elev;
            red2[i].placed = true;
            break;
          } // end if
        } // end for
      } // end if
    } // end else
  } // end if

  // Yellow Blocks
  else if (current_block.color == 'y') {  // If block is yellow
    if (use_first) {
      if (home_dispense) { // Pick west face blocks if at home dispenser
        for (int i = 0; i < (sizeof(yellow1) / sizeof(struct block)); i++) {
          if (!yellow1[i].placed && yellow1[i].face == 'w'){
            current_block.face = yellow1[i].face;
            current_block.pos = yellow1[i].pos;
            current_block.elev = yellow1[i].elev;
            yellow1[i].placed = true;
            break;
          } else if (!yellow1[i].placed && yellow1[i].pos == '1') {
            current_block.face = yellow1[i].face;
            current_block.pos = yellow1[i].pos;
            current_block.elev = yellow1[i].elev;
            yellow1[i].placed = true;
            break;
          } // end if
        } // end for
      } else if (!home_dispense) { // Pick east face blocks if at opponent dispenser
        for (int i = 0; i < (sizeof(yellow1) / sizeof(struct block)); i++) {
          if (!yellow1[i].placed && yellow1[i].face == 'e'){
            current_block.face = yellow1[i].face;
            current_block.pos = yellow1[i].pos;
            current_block.elev = yellow1[i].elev;
            yellow1[i].placed = true;
            break;
          } else if (!yellow1[i].placed && yellow1[i].pos == '3') {
            current_block.face = yellow1[i].face;
            current_block.pos = yellow1[i].pos;
            current_block.elev = yellow1[i].elev;
            yellow1[i].placed = true;
            break;
          } // end if
        } // end for
      } // end dispense if else
      if (current_block.face == '\0'){
        for (int i = 0; i < (sizeof(yellow1) / sizeof(struct block)); i++) {
          if (!yellow1[i].placed) {
            current_block.face = yellow1[i].face;
            current_block.pos = yellow1[i].pos;
            current_block.elev = yellow1[i].elev;
            yellow1[i].placed = true;
            break;
          } // end if
        } // end for
      } // end if
    } else { // Use second set of yellow blocks
      if (home_dispense) { // Pick west face blocks if at home dispenser
        for (int i = 0; i < (sizeof(yellow2) / sizeof(struct block)); i++) {
          if (!yellow2[i].placed && yellow2[i].face == 'w'){
            current_block.face = yellow2[i].face;
            current_block.pos = yellow2[i].pos;
            current_block.elev = yellow2[i].elev;
            yellow2[i].placed = true;
            break;
          } else if (!yellow2[i].placed && yellow2[i].pos == '1') {
            current_block.face = yellow2[i].face;
            current_block.pos = yellow2[i].pos;
            current_block.elev = yellow2[i].elev;
            yellow2[i].placed = true;
            break;
          } // end if
        } // end for
      } else if (!home_dispense) { // Pick east face blocks if at opponent dispenser
        for (int i = 0; i < (sizeof(yellow2) / sizeof(struct block)); i++) {
          if (!yellow2[i].placed && yellow2[i].face == 'e'){
            current_block.face = yellow2[i].face;
            current_block.pos = yellow2[i].pos;
            current_block.elev = yellow2[i].elev;
            yellow2[i].placed = true;
            break;
          } else if (!yellow2[i].placed && yellow2[i].pos == '1') {
            current_block.face = yellow2[i].face;
            current_block.pos = yellow2[i].pos;
            current_block.elev = yellow2[i].elev;
            yellow2[i].placed = true;
            break;
          } // end if
        } // end for
      } // end dispense if else
      if (current_block.face == '\0'){
        for (int i = 0; i < (sizeof(yellow2) / sizeof(struct block)); i++) {
          if (!yellow2[i].placed) {
            current_block.face = yellow2[i].face;
            current_block.pos = yellow2[i].pos;
            current_block.elev = yellow2[i].elev;
            yellow2[i].placed = true;
            break;
          } // end if
        } // end for
      } // end if
    } // end if else
  } // end else if

  // Blue Blocks
  else if (current_block.color == 'b') {  // If block is blue
    if (home_dispense) { // Pick west face blocks if at home dispenser
      for (int i = 0; i < (sizeof(blue) / sizeof(struct block)); i++) {
        if (!blue[i].placed && blue[i].face == 'w'){
          current_block.face = blue[i].face;
          current_block.pos = blue[i].pos;
          current_block.elev = blue[i].elev;
          blue[i].placed = true;
          break;
        } // end if
      } // end for
    } else if (!home_dispense) { // Pick east face blocks if at opponent dispenser
      for (int i = 0; i < (sizeof(blue) / sizeof(struct block)); i++) {
        if (!blue[i].placed && blue[i].face == 'e'){
          current_block.face = blue[i].face;
          current_block.pos = blue[i].pos;
          current_block.elev = blue[i].elev;
          blue[i].placed = true;
          break;
        } // end if
      } // end for
    } // end dispense if else
    if (current_block.face == '\0'){
      for (int i = 0; i < (sizeof(blue) / sizeof(struct block)); i++) {
        if (!blue[i].placed) {
          current_block.face = blue[i].face;
          current_block.pos = blue[i].pos;
          current_block.elev = blue[i].elev;
          blue[i].placed = true;
          break;
        } // end if
      } // end for
    } // end if
  } // end blue else if
} // end func