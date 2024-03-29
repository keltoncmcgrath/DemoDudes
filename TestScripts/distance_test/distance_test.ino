int dist_pin = A8;
float dist_val;
float dist_actual;
float a = exp(7.453976699);   //distance sensor lin fit variable
float b = -0.907499336;       //distance sensor lin fit variable
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  dist_val = analogRead(dist_pin);
  dist_actual = pow(dist_val/a, 1/b);
  Serial.print(dist_val);
  Serial.print("\t");
  Serial.println(dist_actual);
}
