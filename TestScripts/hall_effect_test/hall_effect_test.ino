int pin = A9;
long val;

void setup() {
  Serial.begin(9600);
}

void loop() {
  for(int i=0; i<100; i++){
    val += analogRead(pin);
  }
  val /= 100;

  if(val < 510 || val > 520){
    Serial.println("Ramp Down");
  }

  val = 0;
}
