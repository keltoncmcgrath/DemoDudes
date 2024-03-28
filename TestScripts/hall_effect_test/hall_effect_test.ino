int pin = A9;
int val;
int last_val = 0;
float alpha = 0.05;

void setup() {
  Serial.begin(9600);
}

void loop() {
  for(int i=0; i<100; i++){
    val = alpha*analogRead(pin) + (1-alpha)*last_val;
    last_val = val;
  }
  Serial.println(val);

  val = 0;
  last_val = 0;
}
