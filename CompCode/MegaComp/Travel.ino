void Travel(void) {
  // d = drive striaght line
  // a = arm servo
  // s = shovel servo
  // t = turn
  // o = arc
  // l = linefollow
  Serial.print(directions.head->action[0]);
  Serial.print('\t');
  Serial.println(directions.head->action[1]);
  
  switch(directions.head->action[0]){
    // Drive straight for a distance
    case 'd':
      if(new_action){
        dist_final = directions.head->final_val[0];
        time_final = directions.head->duration[0];
        theta1_final = dist_final / wheel_radius;
        theta2_final = dist_final / wheel_radius;
        ResetTravelVars();
        if(directions.head->action[1] == '\0'){
          new_action = false;
        }
      }
      StraightLine();
      if(abs(theta1_des)>=abs(theta1_final) && abs(theta1_des)>=abs(theta2_final)){
        md.setSpeeds(0, 0);
        next_node = true;
      }
      break;

    // Turn around center
    case 't':
      if(new_action){
        turn_angle_final = directions.head->final_val[0];
        time_final = directions.head->duration[0];
        theta1_final = turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
        theta2_final = -turn_angle_final * (wheel_dist_turn / 2) / wheel_radius;
        ResetTravelVars();
        if(directions.head->action[1] == '\0'){
          new_action = false;
        }
      }
      Turn();
      if(abs(theta1_des)>=abs(theta1_final) && abs(theta2_des)>=abs(theta2_final)){
        md.setSpeeds(0, 0);
        next_node = true;
      }
      break;

    // Turn in an arc
    case 'o':
      if(new_action){
        dist_final = directions.head->final_val[0];
        time_final = directions.head->duration[0];
        arc_radius = directions.head->radius;
        theta1_final = arc_angle_final * (arc_radius + wheel_dist_arc) / wheel_radius;
        theta2_final = arc_angle_final * arc_radius / wheel_radius;
        ResetTravelVars();
        if(directions.head->action[1] == '\0'){
          new_action = false;
        }
      }
      Turn();
      if(abs(theta1_des)>=abs(theta1_final) && abs(theta2_des)>=abs(theta2_final)){
        md.setSpeeds(0, 0);
        next_node = true;
      }
      break;

    // Line follow
    case 'l':
      
      break;
  }

  switch(directions.head->action[1]){
    // Arm servos
    case 'a':
      if(new_action){
        arm_angle_final = directions.head->final_val[1];
        arm_t_final = directions.head->duration[1];
        arm_angle_start = arm_servo.read();
        new_action = false;
        Serial.print(arm_servo.read());
        Serial.print('\t');
        Serial.println(arm_angle_final);
      }
      if(arm_servo.read() != arm_angle_final){
        ArmServo();
      }
      break;

    // Shovel servo
    case 's':
      if(new_action){
        shov_angle_final = directions.head->final_val[1];
        shov_t_final = directions.head->duration[1];
        shov_angle_start = shovel_servo.read();
        new_action = false;
      }
      if(shovel_servo.read() != shov_angle_final){
        ShovelServo();
      }
      break;
  }

  if(next_node){
    directions.DeleteNode();
    if(directions.head == nullptr){
      // Change state
    }
    next_node = false;
    new_action = true;
  }
}