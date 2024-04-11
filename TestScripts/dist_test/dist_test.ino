int dist_pin_right = A9;
int dist_pin_left = A8;

// Range Finder Vars
float dist_actual_right, dist_actual_left;   // cm
float dist_actual_rightf, dist_actual_leftf;
float dist_val_right, dist_val_left;
float dist_val_rightf, dist_val_leftf;
float last_dist_val_right, last_dist_val_left = 0;
float alpha = 0.01;
float alpha_actual = 0.5;
float a1 = exp(7.453976699);
float b1 = -0.907499336;
float a2 = exp(7.869624);
float b2 = -1.05102;
int num_dist_vals = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // Right distance sensor
  for(int i=0; i<num_dist_vals; i++){
    dist_val_right = analogRead(dist_pin_right);
    dist_val_rightf += alpha*dist_val_right + (1-alpha)*dist_val_rightf;
  }
  dist_actual_right = pow(dist_val_right/a1, 1/b1);
  dist_actual_right = constrain(dist_actual_right, 0, 1000);
  dist_actual_rightf = alpha_actual*dist_actual_right + (1-alpha_actual)*dist_actual_rightf;
  dist_val_right = 0;
  

  // Left distance sensor
  for(int i=0; i<num_dist_vals; i++){
    dist_val_left = analogRead(dist_pin_left);
    dist_val_leftf += alpha*dist_val_left + (1-alpha)*dist_val_leftf;
  }
  dist_actual_left = pow(dist_val_left/a2, 1/b2) - 1.1;
  dist_actual_left = constrain(dist_actual_left, 0, 1000);
  dist_actual_leftf = alpha_actual*dist_actual_left + (1-alpha_actual)*dist_actual_leftf;
  dist_val_left = 0;
  
  // PRINT
  Serial.print(dist_actual_rightf);
  Serial.print('\t');
  Serial.print(dist_actual_leftf);
  Serial.print('\t');
  Serial.println(dist_actual_rightf - dist_actual_leftf);
}
