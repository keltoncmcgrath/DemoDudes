void Travel(void) {
  // Serial.print(directions.head->action[0]);
  // Serial.print('\t');
  // Serial.println(directions.head->action[1]);
  // Traveling actions
  switch (directions.head->action[0]) {
    // Drive straight for a distance
    case 'd':
      if (new_action) {
        dist_final = directions.head->final_val[0];
        time_final = directions.head->duration[0];
        theta1_final = dist_final / wheel_radius;
        theta2_final = dist_final / wheel_radius;
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      StraightLine();
      if (abs(theta1_des) >= abs(theta1_final) && abs(theta1_des) >= abs(theta2_final)) {
        md.setSpeeds(0, 0);
        next_node = true;
      }
      break;

    // Turn around center
    case 't':
      if (new_action) {
        turn_angle_final = directions.head->final_val[0];
        time_final = directions.head->duration[0];
        theta1_final = turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
        theta2_final = -turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      Turn();
      if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
        md.setSpeeds(0, 0);
        next_node = true;
      }
      break;

    // Turn in an arc
    case 'o':
      if (new_action) {
        arc_angle_final = directions.head->final_val[0];
        time_final = directions.head->duration[0];
        arc_radius = directions.head->radius;
        theta1_final = arc_angle_final * (arc_radius + wheel_dist_arc) / wheel_radius;
        theta2_final = arc_angle_final * arc_radius / wheel_radius;
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      Turn();
      if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
        md.setSpeeds(0, 0);
        next_node = true;
      }
      break;

    // Line follow
    case 'l':
      // Reset Values
      if (new_action) {
        if (line_dist && last_state!='a') {
          line_follow_speed = 50;
        } else {
          line_follow_speed = 250;
        }
        dist_final = directions.head->final_val[0];
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      // Follow Line
      LineFollow();
      if (line_dist) {
        DistSense();
        if (dist_actual <= dist_final) {
          md.setSpeeds(0, 0);
          next_node = true;
        }
      } else {
        ReadEncoderDist();
        if (dist_traveled >= dist_final) {
          md.setSpeeds(0, 0);
          next_node = true;
        }
      }
      break;

    // Distance Sensor Straight Line
    case 'r':
      if (new_action) {
        if(last_state != 'e'){
          line_speed = 300;
        } else {
          line_speed = 50;
        }
        dist_final = directions.head->final_val[0];
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      DistSense();
      StraightRange();
      if (dist_actual <= dist_final){
        md.setSpeeds(0, 0);
        next_node = true;
      }
      break;
  }

  // Servo actions
  switch (directions.head->action[1]) {
    // Arm servos
    case 'a':
      if (new_action) {
        arm_angle_final = directions.head->final_val[1];
        arm_t_final = directions.head->duration[1];
        arm_angle_start = arm_servo.read();
        new_action = false;
        if (directions.head->action[0] == '\0'){
          ResetTravelVars();
        }
      }
      ArmServo();
      if (arm_servo.read() == arm_angle_final) {
        next_node = true;
      }
      break;

    // Shovel servo
    case 's':
      if (new_action) {
        shov_angle_final = directions.head->final_val[1];
        shov_t_final = directions.head->duration[1];
        shov_angle_start = shovel_servo.read();
        new_action = false;
        if (directions.head->action[0] == '\0'){
          ResetTravelVars();
        }
      }
      ShovelServo();
      if (shovel_servo.read() == shov_angle_final) {
        next_node = true;
      }
      break;
  }

  // Delete current node, change state if null
  if (next_node) {
    directions.DeleteNode();
    if (directions.head == nullptr) {
      switch(last_state){
        case 'a':
          state = 'b';
          break;
        case 'b':
          state = 'c';
          break;
        case 'c':
          state = 'e';
          break;
        case 'e':
          state = 'f';
          break;
        case 'f':
          state = 'a';
          break;
      }
    }
    next_node = false;
    new_action = true;
  }
}