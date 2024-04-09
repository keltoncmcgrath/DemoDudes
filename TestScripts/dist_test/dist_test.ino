int dist_pin = A8;

// Range Finder Vars
float dist_actual = 1000;   // cm
float dist_val;
float last_dist_val = 0;
float alpha = 0.01;
float a = exp(7.453976699);
float b = -0.907499336;
int num_dist_vals = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<100; i++){
    dist_val += alpha*analogRead(dist_pin) + (1-alpha)*last_dist_val;
  }
  dist_actual = pow(dist_val/a, 1/b);
  dist_val = 0;
  if(dist_actual < 20){
    // Serial.println(dist_actual);
  }
  Serial.println(dist_actual);
}
