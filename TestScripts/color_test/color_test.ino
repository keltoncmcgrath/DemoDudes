#include <Servo.h>
Servo shovel_servo;
int shovel_servo_pin = 23;

int blue_pin = 33;
int green_pin = 34;
int red_pin = 35;
int photo_trans_pin = A10;

float color_alpha = 0.01;
float color_sense_alpha = 0.4;
bool is_color = false;
long red_sum = 0; 
long green_sum = 0;
long blue_sum = 0;
long red_val, red_valf = 0;
long green_val, green_valf = 0;
long blue_val, blue_valf = 0;
float red_avg[4];
float green_avg[4];
float blue_avg[4];
float red_std[4] = {0, 0, 0, 0};
float green_std[4] = {0, 0, 0, 0};
float blue_std[4] = {0, 0, 0, 0};
int color_delay_time = 10;
const int color_samples = 1000;
const int color_samples_sense = 10;
int red_calibration_vals[color_samples];
int green_calibration_vals[color_samples];
int blue_calibration_vals[color_samples];
int color_ranges[4][3][2];
int color_vals[color_samples][3];


void ColorSense(){
  for(int i=0; i<color_samples_sense; i++){
    // Reading Red Light 
    digitalWrite(red_pin, LOW);
    delay(color_delay_time);
    // red_val = color_alpha*analogRead(photo_trans_pin) + (1-color_alpha)*last_red_val;
    red_val = analogRead(photo_trans_pin);
    digitalWrite(red_pin, HIGH);
    red_valf = color_sense_alpha*red_val + (1-color_sense_alpha)*red_valf;

    // Reading Green Light
    digitalWrite(green_pin, LOW);
    delay(color_delay_time);
    // green_val = color_alpha*analogRead(photo_trans_pin) + (1-color_alpha)*last_green_val;
    green_val = analogRead(photo_trans_pin);
    digitalWrite(green_pin, HIGH);
    green_valf = color_sense_alpha*green_val + (1-color_sense_alpha)*green_valf;

    // Reading Blue Light
    digitalWrite(blue_pin, LOW);
    delay(color_delay_time);
    // blue_val = color_alpha*analogRead(photo_trans_pin) + (1-color_alpha)*last_blue_val;
    blue_val = analogRead(photo_trans_pin);
    digitalWrite(blue_pin,HIGH);
    blue_valf = color_sense_alpha*blue_val + (1-color_sense_alpha)*blue_val;
  }
  // red_val /= color_samples;
  // green_val /= color_samples;
  // blue_val /= color_samples;
  Serial.print(red_valf);
  Serial.print('\t');
  Serial.print(green_valf);
  Serial.print('\t');
  Serial.println(blue_valf);

  for(int i=0; i<4; i++){
    if(red_valf > color_ranges[i][0][0] & red_valf < color_ranges[i][0][1]){
      if(green_valf > color_ranges[i][1][0] & green_valf < color_ranges[i][1][1]){
        if(blue_valf > color_ranges[i][2][0] & blue_valf < color_ranges[i][2][1]){
          is_color = true;
          if(i==0){
            Serial.println("Block is RED");
          }
          else if(i==1){
            Serial.println("Block is YELLOW");
          }
          else if (i == 2){
            Serial.println("Block is BLUE");
          } else {
            Serial.println("No Block");
          }
        }
      }    
    }
  }
  if(!is_color){
    Serial.println("Block Color Not Found");
  }
  is_color = false;
  red_val, red_valf = 0;
  green_val, green_valf = 0;
  blue_val, blue_valf = 0;
}

void ColorCalibration(){
  for(int j=0; j<4; j++){
    if(j == 0){
      Serial.println("Place Red Block on Sensor");
      shovel_servo.write(110);
      delay(100);
      shovel_servo.write(120);
    }
    else if(j == 1){
      Serial.println("Place Yellow Block on Sensor");
      shovel_servo.write(110);
      delay(100);
      shovel_servo.write(120);
    }
    else if (j == 2){
      Serial.println("Place Blue Block on Sensor");
      shovel_servo.write(110);
      delay(100);
      shovel_servo.write(120);
    } else {
      Serial.println("Take Block off of Sensor");
      shovel_servo.write(110);
      delay(100);
      shovel_servo.write(120);
    }
    delay(5000);
    Serial.println("Reading Color Values...");
    for(int i=0; i<color_samples; i++){
      digitalWrite(red_pin, LOW);
      delay(color_delay_time);
      red_val = analogRead(photo_trans_pin);
      red_calibration_vals[i] = red_val;
      red_sum += red_val;
      digitalWrite(red_pin, HIGH);

      digitalWrite(green_pin, LOW);
      delay(color_delay_time);
      green_val = analogRead(photo_trans_pin);
      green_calibration_vals[i] = green_val;
      green_sum += green_val;
      digitalWrite(green_pin, HIGH);

      digitalWrite(blue_pin, LOW);
      delay(color_delay_time);
      blue_val = analogRead(photo_trans_pin);
      blue_calibration_vals[i] = blue_val;
      blue_sum += blue_val;
      digitalWrite(blue_pin, HIGH);    
    }

    // Calculate Avgerages
    red_avg[j] = red_sum/color_samples;
    green_avg[j] = green_sum/color_samples;
    blue_avg[j] = blue_sum/color_samples;

    // Calculate Standard Deviations
    for(int i=0; i<color_samples; i++){
      red_std[j] += pow((red_calibration_vals[i] - red_avg[j]), 2);
      green_std[j] += pow((green_calibration_vals[i] - green_avg[j]), 2);
      blue_std[j] += pow((blue_calibration_vals[i] - blue_avg[j]), 2);
    }
    red_std[j] = sqrt(red_std[j]/(color_samples-1));
    green_std[j] = sqrt(green_std[j]/(color_samples-1));
    blue_std[j] = sqrt(blue_std[j]/(color_samples-1));

    // Set Sums to Zero
    red_sum = 0;
    green_sum = 0;
    blue_sum = 0;

    // Find Ranges of Values for Each Block
    for(int i=0; i<4; i++){
      if(i == 3){
        // Red
        color_ranges[i][0][0] = 0;
        color_ranges[i][0][1] = red_avg[i]+4*red_std[i];
        // Green
        color_ranges[i][1][0] = 0;
        color_ranges[i][1][1] = green_avg[i]+4*green_std[i];
        // Blue
        color_ranges[i][2][0] = 0;
        color_ranges[i][2][1] = blue_avg[i]+4*blue_std[i];
      } else {
        // Red
        color_ranges[i][0][0] = red_avg[i]-4*red_std[i];
        color_ranges[i][0][1] = red_avg[i]+4*red_std[i];
        // Green
        color_ranges[i][1][0] = green_avg[i]-4*green_std[i];
        color_ranges[i][1][1] = green_avg[i]+4*green_std[i];
        // Blue
        color_ranges[i][2][0] = blue_avg[i]-4*blue_std[i];
        color_ranges[i][2][1] = blue_avg[i]+4*blue_std[i];
      }
    }
  }
  for(int i=0; i<4; i++){
    Serial.print(color_ranges[i][0][0]);
    Serial.print('\t');
    Serial.println(color_ranges[i][0][1]);

    Serial.print(color_ranges[i][1][0]);
    Serial.print('\t');
    Serial.println(color_ranges[i][1][1]);

    Serial.print(color_ranges[i][2][0]);
    Serial.print('\t');
    Serial.println(color_ranges[i][2][1]);
    Serial.println();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  shovel_servo.attach(shovel_servo_pin);
  shovel_servo.write(120);

  pinMode(blue_pin, OUTPUT), digitalWrite(blue_pin, HIGH);
  pinMode(red_pin, OUTPUT), digitalWrite(red_pin, HIGH);
  pinMode(green_pin, OUTPUT), digitalWrite(green_pin, HIGH);

  Serial.println("Ready to Calibrate?");
  Serial.println("3...");
  delay(1000);
  Serial.println("2...");
  delay(1000);
  Serial.println("1...");
  delay(1000);
  ColorCalibration();
}

void loop() {
  ColorSense();
}
