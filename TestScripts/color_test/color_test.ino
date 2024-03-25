int red_pin = 35;
int green_pin = 34;
int blue_pin = 33;

void setup() {
  // Set Color Sensor Pins
  pinMode(blue_pin, OUTPUT);  digitalWrite(blue_pin, HIGH);
  pinMode(red_pin, OUTPUT);   digitalWrite(red_pin, HIGH);
  pinMode(green_pin,OUTPUT);  digitalWrite(green_pin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(blue_pin, LOW);
}
