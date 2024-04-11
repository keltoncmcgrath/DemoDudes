int pin1 = 36;
int pin2 = 37;

float t_start;
float t;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin1, OUTPUT), digitalWrite(pin1, LOW);
  pinMode(pin2, OUTPUT), digitalWrite(pin2, LOW);
  t_start = millis();
}

void loop() {
  t = (millis() - t_start) / 1000;
  if(int(t)%2 == 0){
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  }

}
