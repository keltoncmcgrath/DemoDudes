void Travel(void) {
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
      TimedDrive();
      if (abs(theta1_des) >= abs(theta1_final) && abs(theta1_des) >= abs(theta2_final)) {
        md.setSpeeds(0, 0);
        digitalWrite(right_turn_pin, LOW);
        digitalWrite(left_turn_pin, LOW);
        next_node = true;
      }
      break;
      // Hazards
      if (dist_final < 0) {
        if (int(t) % 2 == 0) {
          digitalWrite(right_turn_pin, HIGH);
          digitalWrite(left_turn_pin, HIGH);
        } else {
          digitalWrite(right_turn_pin, LOW);
          digitalWrite(left_turn_pin, LOW);
        }
      }
      break;

    // Turn around center
    case 't':
      if (new_action) {
        turn_angle_final = directions.head->final_val[0];
        time_final = directions.head->duration[0];
        theta1_final = turn_angle_final * (wheel_dist / 2) / wheel_radius;
        theta2_final = -turn_angle_final * (wheel_dist / 2) / wheel_radius;
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      TimedDrive();
      if (abs(theta1) >= abs(theta1_final) && abs(theta2) >= abs(theta2_final)) {
        md.setSpeeds(0, 0);
        digitalWrite(right_turn_pin, LOW);
        digitalWrite(left_turn_pin, LOW);
        next_node = true;
      }
      // Blinker
      if (turn_angle_final > 0) {
        if (int(t) % 2 == 0) {
          digitalWrite(right_turn_pin, HIGH);
        } else {
          digitalWrite(right_turn_pin, LOW);
        }
      } else if (turn_angle_final < 0) {
        if (int(t) % 2 == 0) {
          digitalWrite(left_turn_pin, HIGH);
        } else {
          digitalWrite(left_turn_pin, LOW);
        }
      }
      break;

    // Turn in an arc
    case 'o':
      if (new_action) {
        arc_angle_final = directions.head->final_val[0];
        time_final = directions.head->duration[0];
        arc_radius = directions.head->radius;
        if (arc_angle_final > 0) {  // Arc Right
          theta1_final = arc_angle_final * (arc_radius/abs(arc_radius))*(abs(arc_radius) + wheel_dist) / wheel_radius;
          theta2_final = arc_angle_final * arc_radius / wheel_radius;
        } else if (arc_angle_final < 0) {  // Arc Left
          theta1_final = abs(arc_angle_final) * arc_radius / wheel_radius;
          theta2_final = abs(arc_angle_final) * (arc_radius/abs(arc_radius))*(abs(arc_radius) + wheel_dist) / wheel_radius;
        }
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      TimedDrive();
      if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
        md.setSpeeds(0, 0);
        digitalWrite(right_turn_pin, LOW);
        digitalWrite(left_turn_pin, LOW);
        next_node = true;
      }
      // Blinkers
      if (theta1_final > theta2_final) {
        if (int(t) % 2 == 0) {
          digitalWrite(right_turn_pin, HIGH);
        } else {
          digitalWrite(right_turn_pin, LOW);
        }
      } else {
        if (int(t) % 2 == 0) {
          digitalWrite(left_turn_pin, HIGH);
        } else {
          digitalWrite(left_turn_pin, LOW);
        }
      }
      break;

    // Line follow
    case 'l':
      // Reset Values
      if (new_action) {
        dist_final = directions.head->final_val[0];
        ResetTravelVars();
        if (line_dist && last_state != 'a' && last_state != 'f' && dist_final != dist_to_wall) {
          line_follow_speed = 100;
        } else {
          line_follow_speed = 300;
        }
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      // Follow Line
      LineFollow();
      if (line_dist) {
        DistSense();
        Serial.println(dist_actual);
        if (dist_actual <= dist_final) {
          md.setSpeeds(0, 0);
          next_node = true;
        }
      } else {
        ReadEncoderDist();
        if (abs(dist_traveled) >= abs(dist_final)) {
          md.setSpeeds(0, 0);
          next_node = true;
        }
      }
      break;

    // Distance Sensor Straight Line
    case 'r':
      if (new_action) {
        if (last_state != 'e') {
          line_speed = 300;
        } else {
          line_speed = 100;
        }
        dist_final = directions.head->final_val[0];
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      DistSense();
      Serial.println(dist_actual);
      StraightRange();
      if (dist_actual <= dist_final) {
        md.setSpeeds(0, 0);
        next_node = true;
      }
      break;

    // Travel to black line
    case 'k':
      if (new_action) {
        last_line_state = true;
        second_line = false;
        line_speed = directions.head->final_val[0];
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      StraightRange();
      qtr.read(ir_values);
      for (int i = 0; i < ir_sensor_count; i++) {
        if (ir_values[i] < 500) {
          // Over white line
          if (!last_line_state) {
            second_line = true;
          }
          last_line_state = true;
          goto end_of_case;
        }
      }
      // Over black line
      last_line_state = false;
      if (home_dispense && line_speed > 0 && current_block.face == 'e' && !second_line) { // || current_block.pos == '3'
        goto end_of_case;
      } else if (!home_dispense && line_speed > 0 && current_block.face == 'w' && !second_line){
        goto end_of_case;
      }
      md.setSpeeds(0, 0);
      next_node = true;
      // End case
      end_of_case:
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
        if (directions.head->action[0] == '\0') {
          ResetTravelVars();
        }
      }
      ArmServo();
      if (arm_servo.read() == arm_angle_final && directions.head->action[0] == '\0') {
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
        if (directions.head->action[0] == '\0') {
          ResetTravelVars();
        }
      }
      ShovelServo();
      if (shovel_servo.read() == shov_angle_final && directions.head->action[0] == '\0') {
        next_node = true;
      }
      break;
  }

  // Delete current node, change state if null
  if (next_node) {
    directions.DeleteNode();
    if (directions.head == nullptr) {
      switch (last_state) {
        case 'a':
          state = 'b';
          break;
        case 'b':
          ResetTravelVars();
          state = 'c';
          break;
        case 'c':
          delay(20);
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