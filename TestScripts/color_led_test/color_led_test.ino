int red_pin = 35;
int green_pin = 34;
int blue_pin = 33;
int photo_trans_pin = A10;

void setup() {
  // Set Color Sensor Pins
  Serial.begin(9600);
  pinMode(blue_pin, OUTPUT);  digitalWrite(blue_pin, HIGH);
  pinMode(red_pin, OUTPUT);   digitalWrite(red_pin, HIGH);
  pinMode(green_pin,OUTPUT);  digitalWrite(green_pin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(red_pin, LOW);
  delay(10);
  Serial.print(analogRead(photo_trans_pin));
  Serial.print('\t');
  digitalWrite(red_pin, HIGH);

  digitalWrite(green_pin, LOW);
  delay(10);
  Serial.print(analogRead(photo_trans_pin));
  Serial.print('\t');
  digitalWrite(green_pin, HIGH);

  digitalWrite(blue_pin, LOW);
  delay(10);
  Serial.print(analogRead(photo_trans_pin));
  Serial.print('\t');
  digitalWrite(blue_pin, HIGH);

  Serial.println();
}
