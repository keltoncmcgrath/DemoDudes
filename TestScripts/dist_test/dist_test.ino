int dist_pin = A8;

// Range Finder Vars
float dist_actual = 1000;   // cm
float dist_val;
float alpha = 0.1;
float a = exp(7.453976699);
float b = -0.907499336;
int num_dist_vals = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<50; i++){
    dist_val += analogRead(dist_pin);
  }
  dist_val /= 50;
  dist_actual = pow(dist_val/a, 1/b);
  dist_val = 0;
  Serial.println(dist_actual);
}
