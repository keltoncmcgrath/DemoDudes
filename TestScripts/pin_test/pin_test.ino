uint8_t pins[] = {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
int num_pins = 12;
int data;

void setup() {
  // put your setup code here, to run once:
  for(int i=0; i<num_pins; i++){
    Serial.begin(9600);
    pinMode(pins[i], INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i<num_pins; i++){
    data = digitalRead(pins[i]);
    Serial.print(data);
    Serial.print("\t");
  }
  Serial.println();
}
