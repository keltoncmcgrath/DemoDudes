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
      // Hazards
      if (dist_final < 0) {
        if (int(t * 100) % 50 == 0) {
          digitalWrite(right_turn_pin, HIGH);
          digitalWrite(left_turn_pin, HIGH);
        } else if (int(t * 100) % 25 == 0) {
          digitalWrite(right_turn_pin, LOW);
          digitalWrite(left_turn_pin, LOW);
        }
      }
      // Action
      TimedDrive();
      m1_current = md.getM1CurrentMilliamps();
      m2_current = md.getM2CurrentMilliamps();
      if (abs(theta1_des) >= abs(theta1_final) && abs(theta1_des) >= abs(theta2_final)) {
        md.setSpeeds(0, 0);
        digitalWrite(right_turn_pin, LOW);
        digitalWrite(left_turn_pin, LOW);
        next_node = true;
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

      // Blinker
      if (turn_angle_final > 0) {
        if (int(t * 100) % 50 == 0) {
          digitalWrite(right_turn_pin, HIGH);
        } else if (int(t * 100) % 25 == 0) {
          digitalWrite(right_turn_pin, LOW);
        }
      } else if (turn_angle_final < 0) {
        if (int(t * 100) % 50 == 0) {
          digitalWrite(left_turn_pin, HIGH);
        } else if (int(t * 100) % 25 == 0) {
          digitalWrite(left_turn_pin, LOW);
        }
      }

      // Action
      TimedDrive();
      if (abs(theta1) >= abs(theta1_final) && abs(theta2) >= abs(theta2_final) && md.getM1CurrentMilliamps() <= 5 && md.getM2CurrentMilliamps() <= 5) {
        md.setSpeeds(0, 0);
        digitalWrite(right_turn_pin, LOW);
        digitalWrite(left_turn_pin, LOW);
        next_node = true;
      }
      break;

    // Turn in an arc
    case 'o':
      if (new_action) {
        arc_angle_final = directions.head->final_val[0];
        time_final = directions.head->duration[0];
        arc_radius = directions.head->radius;
        if (arc_angle_final > 0) {  // Arc Right
          theta1_final = arc_angle_final * (arc_radius / abs(arc_radius)) * (abs(arc_radius) + wheel_dist + 0.3) / wheel_radius;
          theta2_final = arc_angle_final * arc_radius / wheel_radius;
        } else if (arc_angle_final < 0) {  // Arc Left
          theta1_final = abs(arc_angle_final) * arc_radius / wheel_radius;
          theta2_final = abs(arc_angle_final) * (arc_radius / abs(arc_radius)) * (abs(arc_radius) + wheel_dist + 0.3) / wheel_radius;
        }
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }

      // Blinkers
      if (theta1_final > theta2_final) {
        if (int(t * 100) % 50 == 0) {
          digitalWrite(right_turn_pin, HIGH);
        } else if (int(t * 100) % 25 == 0) {
          digitalWrite(right_turn_pin, LOW);
        }
      } else {
        if (int(t * 100) % 50 == 0) {
          digitalWrite(left_turn_pin, HIGH);
        } else if (int(t * 100) % 25 == 0) {
          digitalWrite(left_turn_pin, LOW);
        }
      }

      // Action
      TimedDrive();
      if (abs(theta1_des) >= abs(theta1_final) && abs(theta2_des) >= abs(theta2_final)) {
        md.setSpeeds(0, 0);
        digitalWrite(right_turn_pin, LOW);
        digitalWrite(left_turn_pin, LOW);
        next_node = true;
      }
      break;

    // Line follow
    case 'l':
      // Initialize action
      if (new_action) {
        dist_final = directions.head->final_val[0];
        line_follow_speed = directions.head->duration[0];
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }

      // Follow Line
      LineFollow();
      m1_current = md.getM1CurrentMilliamps();
      m2_current = md.getM2CurrentMilliamps();
      Serial.print(m1_current);
      Serial.print('\t');
      Serial.print(m2_current);
      Serial.print('\t');
      Serial.println(!line_dist);
      if (line_dist) {  // Follow line until range finder trips
        DistSenseRight();
        if (last_state == 'e' || last_state == 'a') {  // Control speed based on distance from chassis
          line_follow_speed = line_base + dump_KP * (dist_actual - dist_final);
          line_follow_speed = constrain(line_follow_speed, line_base, 350);
        }
        if ((t > 0.1 && dist_actualf <= dist_final) || (last_state == 'e' && current_block.elev == 'l' && line_follow_speed < line_base+100 && (m1_current > max_current && m2_current > max_current))){
          md.setSpeeds(0, 0);
          line_follow_speed = line_speed;
          next_node = true;
        }
      } else {  // Follow line for set distance
        ReadEncoderDist();
        if ((abs(dist_traveled) >= abs(dist_final)) || (t > 0.5 && last_state == 'b' && (m1_current > 100 || m2_current > 100))) {
          md.setSpeeds(0, 0);
          dump_dist_upper = 13;
          line_follow_speed = line_speed;
          next_node = true;
        }
      }
      break;

    // Distance Sensor Straight Line
    case 'r':
      if (new_action) {
        dist_final = directions.head->final_val[0];
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }
      }
      
      // Read from left or right range finder
      if (dist_right) {
        DistSenseRight();
      } else {
        DistSenseLeft();
      }
      Serial.print(dist_actualf);
      Serial.print('\t');
      Serial.println(dist_final);
      if (last_state == 'e') {  // Control speed based on distance from chassis
        line_speed = line_base + dump_KP * (dist_actualf - dist_final);
        line_speed = constrain(line_speed, line_base, line_follow_speed);
      }
      StraightRange();
      m1_current = md.getM1CurrentMilliamps();
      m2_current = md.getM2CurrentMilliamps();
      if ((t > 0.1 && dist_actualf <= dist_final) || (last_state == 'e' && current_block.elev == 'l' && line_speed < line_base+100 && (m1_current > max_current && m2_current > max_current))) {
        md.setSpeeds(0, 0);
        line_speed = line_follow_speed;
        dump_dist_upper = 13;
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

      // Hazards
      if (line_speed < 0) {
        if (int(t * 100) % 50 == 0) {
          digitalWrite(right_turn_pin, HIGH);
          digitalWrite(left_turn_pin, HIGH);
        } else if (int(t * 100) % 25 == 0) {
          digitalWrite(right_turn_pin, LOW);
          digitalWrite(left_turn_pin, LOW);
        }
      }

      // Action
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
      if (home_dispense && line_speed > 0 && current_block.face == 'e' && !second_line) {  // || current_block.pos == '3'
        goto end_of_case;
      } else if (!home_dispense && line_speed > 0 && current_block.face == 'w' && !second_line) {
        goto end_of_case;
      }
      md.setSpeeds(0, 0);
      digitalWrite(right_turn_pin, LOW);
      digitalWrite(left_turn_pin, LOW);
      line_speed = line_follow_speed;
      next_node = true;

      // End case
      end_of_case:
      break;

    // Travel to Line Junction
    case 'j':
      // Initialize Action
      if (new_action) {
        line_follow_speed = directions.head->final_val[0];
        ResetTravelVars();
        if (directions.head->action[1] == '\0') {
          new_action = false;
        }  // end if
      }    // end if

      // Hazards
      if (line_follow_speed < 0) {
        if (int(t * 100) % 50 == 0) {
          digitalWrite(right_turn_pin, HIGH);
          digitalWrite(left_turn_pin, HIGH);
        } else if (int(t * 100) % 25 == 0) {
          digitalWrite(right_turn_pin, LOW);
          digitalWrite(left_turn_pin, LOW);
        }
      }

      // Action
      LineFollow();
      for (int i = 0; i < ir_sensor_count; i++) {
        if (ir_values[i] > 500) {
          black_count += 1;
        }  // end if
      }    // end for
      if (black_count >= ir_sensor_count / 2) {
        md.setSpeeds(0, 0);
        line_follow_speed = line_speed;
        next_node = true;
      }  // end if
      black_count = 0;
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
      if (arm_servo.read() != arm_angle_final) {
        ArmServo();
      } else if (arm_servo.read() == arm_angle_final && directions.head->action[0] == '\0') {
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
      if (shovel_servo.read() != shov_angle_final) {
        ShovelServo();
      } else if (shovel_servo.read() == shov_angle_final && directions.head->action[0] == '\0') {
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
        case 'g':
          state = 'a';
      }
    }
    next_node = false;
    new_action = true;
  }
}