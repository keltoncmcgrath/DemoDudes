uint8_t pins[] = {24, 25, 26, 27, 28, 29, 30, 31};
int data;

void setup() {
  // put your setup code here, to run once:
  for(int i=0; i<8; i++){
    Serial.begin(9600);
    pinMode(pins[i], INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i<8; i++){
    data = digitalRead(pins[i]);
    Serial.print(data);
    Serial.print("\t");
  }
  Serial.println();
}
