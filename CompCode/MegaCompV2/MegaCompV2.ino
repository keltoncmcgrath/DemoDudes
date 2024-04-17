///////////////////////////////
///-- LIBRARIES & OBJECTS --///
///////////////////////////////
// Include Libraries
#include <DualTB9051FTGMotorShieldUnoMega.h>
#include <Encoder.h>
#include <Servo.h>
#include <QTRSensors.h>

// Create Objects
DualTB9051FTGMotorShieldUnoMega md;
QTRSensors qtr;
Servo arm_servo;
Servo shovel_servo;

//////////////////////////////
///------ STRUCTURES ------///
//////////////////////////////
struct block {
  char face;
  char pos;
  char elev;
  bool placed;
  char color;
  void Reset(void){
    face = '\0';
    pos = '\0';
    elev = '\0';
    placed = false;
    color = '\0';
  }
};

struct node {
  char action[2];
  float final_val[2];
  float duration[2];
  float radius;
  struct node *next;
};

class linked_list {
public:
  struct node *head;
  // Initialization function
  linked_list() {
    this->head = nullptr;
  }
  // New node function
  void AddNode(char action1, float final1, float duration1 = 0, float radius1 = 0, char action2 = '\0', float final2 = 0, float duration2 = 0) {
    struct node *temp = new node;
    temp->action[0] = action1;
    temp->action[1] = action2;
    temp->final_val[0] = final1;
    temp->final_val[1] = final2;
    temp->duration[0] = duration1;
    temp->duration[1] = duration2;
    temp->radius = radius1;
    temp->next = head;
    head = temp;
  }
  // Delete node function
  void DeleteNode() {
    struct node *temp = new node;
    temp = head;
    head = head->next;
    delete temp;
  }
  // Add new node at tail end of LL
  void AddTailNode(char action1, float final1, float duration1 = 0, float radius1 = 0, char action2 = '\0', float final2 = 0, float duration2 = 0) {
    // Construct node to be added
    struct node *temp = new node;
    temp->action[0] = action1;
    temp->action[1] = action2;
    temp->final_val[0] = final1;
    temp->final_val[1] = final2;
    temp->duration[0] = duration1;
    temp->duration[1] = duration2;
    temp->radius = radius1;
    temp->next = nullptr;
    // Insert as first node if head is null
    if (head == nullptr) {
      temp->next = head;
      head = temp;
      return;
    }
    // Find last node
    struct node *last = head;
    last->next = head->next;
    while (last->next != nullptr) {
      last = last->next;
    }
    // Insert new node
    last->next = temp;
  }
  // Delete node at tail end of LL
  void DeleteTailNode() {
    struct node *temp = head;
    // Delete current node if it's the last
    if (head->next == nullptr) {
      head = head->next;
      delete temp;
      return;
    }
    // Find second to last node
    while (temp->next->next != nullptr) {
      temp = temp->next;
    }
    // Delete last node and point second to last to null
    delete temp->next;
    temp->next = nullptr;
  }
} directions;
 

//////////////////////////////
///--- PIN DECLARATIONS ---///
//////////////////////////////
// Digital
int encoder1_pinA = 19;
int encoder1_pinB = 18;
int encoder2_pinA = 20;
int encoder2_pinB = 21;
int arm_servo_pin = 22;
int shovel_servo_pin = 23;
int limit_switch_pin = 32;
int blue_pin = 33;
int green_pin = 34;
int red_pin = 35;
const uint8_t ir_pins[] = { 24, 25, 26, 28, 27, 29, 30, 31 };
int right_turn_pin = 36;
int left_turn_pin = 37;

// Analog
int dist_pin_right = A9;
int dist_pin_left = A8;
int photo_trans_pin = A10;


//////////////////////////////
///--- GLOBAL VARIABLES ---///
//////////////////////////////
// General Vars
float t;
float t_old;
float t_start;
float delta_T;
float pi = 3.1415927;
char state = 'a';
char last_state;

// Block Info Vars
char rc;
int flag = 33;
int num_blocks;
char block_color;
bool use_first = true;
struct block current_block = { 'n', '1', 'l', false, 'y' };
struct block read_block;
// struct block red1[6] = {
//   { 'e', '4', 'l', true },
//   { 'e', '4', 'u', true },
//   { 'e', '5', 'l', true },
//   { 'e', '5', 'u', true },
//   { 'e', '6', 'l', true },
//   { 'e', '6', 'u', true }
// };
// struct block red1[12] = {
//   { 'e', '4', 'l', false },
//   { 'e', '5', 'l', false },
//   { 'e', '6', 'l', false },
//   { 'e', '4', 'u', false },
//   { 'e', '5', 'u', false },
//   { 'e', '6', 'u', false },
//   { 'n', '3', 'l', false },
//   { 'n', '3', 'u', false },
//   { 's', '3', 'l', false },
//   { 's', '3', 'u', false },
//   { 's', '2', 'l', false },
//   { 's', '2', 'u', false }
// };
struct block red1[6] = {
  { 'w', '4', 'l', false },
  { 'e', '4', 'l', false },
  { 'w', '6', 'l', false },
  { 'e', '6', 'l', false },
  { 'w', '5', 'l', false },
  { 'e', '5', 'l', false }
};
struct block red2[6] = {
  { 'n', '1', 'l', false },
  { 's', '1', 'l', false },
  { 'n', '3', 'l', false },
  { 's', '3', 'l', false },
  { 'n', '2', 'l', false },
  { 's', '2', 'l', false }
};
struct block yellow1[10] = {
  { 'n', '1', 'l', false },
  { 's', '1', 'l', false },
  { 'n', '3', 'l', false },
  { 's', '3', 'l', false },
  { 'n', '2', 'l', false },
  { 's', '2', 'l', false },
  { 'w', '5', 'u', false },
  { 'e', '5', 'u', false },
  { 'n', '2', 'u', false },
  { 's', '2', 'u', false }
};
struct block yellow2[10] = {
  { 'w', '4', 'l', false },
  { 'e', '4', 'l', false },
  { 'w', '6', 'l', false },
  { 'e', '6', 'l', false },
  { 'w', '5', 'l', false },
  { 'e', '5', 'l', false },
  { 'w', '5', 'u', false },
  { 'e', '5', 'u', false },
  { 'n', '2', 'u', false },
  { 's', '2', 'u', false }
};
struct block blue[4] = {
  { 'w', '4', 'u', false },
  { 'w', '6', 'u', false },
  { 'e', '4', 'u', false },
  { 'e', '6', 'u', false }
};

// Control Vars
float dump_KP = 20;
float KP = 56;
float KI = 500;
float KD = 1.56;
float error1, d_error1, integral_error1;
float error2, d_error2, integral_error2;
int m1s, m2s;
float V1, V2;

// Travel Vars
float dist_traveled;
float dist_final;
float speed_dist_error;
float speed_alpha = 0.1;
float turn_angle_final;        // rad
float arc_radius;              // cm
float arc_angle_final;         // rad
float time_final;
int m1_current, m2_current;
float theta1, theta1_des, theta1_desf, theta1_final, theta1_old;
float theta2, theta2_des, theta2_desf, theta2_final, theta2_old;
float omega1, omega1_des;
float omega2, omega2_des;
long counts1;
long counts2;
float theta_alpha = 0.075;

// Travel Constants
int counts_per_rev = 64;
int gear_ratio = 131;
float wheel_radius = 4.2;       // cm
float wheel_dist = 18.7;        // cm
float turn_time = 0.8;          // s
float arc_time_big = 2;         // s
float arc_time_little = 1.5;    // s
float ir_to_wheel = 5;          // cm
float ir_to_wheel_time = 0.3;   // s
int max_current = 150;          // mA

// Travel Vals
float east_guide = 122;     // cm
float south_guide = 68.58;  // cm
float north_guide = 33.5;   // cm
float guide1 = 53;          // cm
float guide2 = 61;          // cm
float guide3 = 69;          // cm
float guide4 = 52;          // cm
float guide5 = 59.5;        // cm
float guide6 = 67.5;        // cm
float next_pos_dist = 8;    // cm
float collect_dist = 6;   // cm
int line_follow_speed = 390;
int line_base = 75;
int line_speed = 390;

// Encoder Vars
int encoder1_count;
int encoder2_count;
Encoder encoder1(encoder1_pinA, encoder1_pinB);
Encoder encoder2(encoder2_pinA, encoder2_pinB);

// Arm Servo Vars
int servo_home = 93;
int arm_max_angle = 20;
int arm_collect_angle = 80;
int arm_low_dump_angle = 80;
float arm_angle_des;
int arm_angle_start;
int arm_angle_final;
float arm_t_final = 1;
float arm_tol = 0.1;

// Shovel Servo Vars
int shov_max_angle = 20; // 20
int shov_low_dump_angle = 80; // 80
int shov_dump_angle = 0;
int shov_button_angle = 95;
int shov_travel_angle = 110;
int shov_collect_angle = 120; // 120
float shov_angle_des;
int shov_angle_start;
int shov_angle_final;
float shov_t_final = 1;
float shov_tol = 0.1;

// Color Sensor Vars
int color_samples_sense = 10;
float color_alpha = 0.01;
float color_sense_alpha = 0.5;
int color_delay_time = 10;
int block_wait_time = 2;
bool is_color = false;
long red_sum = 0;
long green_sum = 0;
long blue_sum = 0;
long red_val, red_valf;
long green_val, green_valf;
long blue_val, blue_valf;
float red_avg[3];
float green_avg[3];
float blue_avg[3];
float red_std[3] = { 0, 0, 0 };
float green_std[3] = { 0, 0, 0 };
float blue_std[3] = { 0, 0, 0 };

// Color Calibration Vars
const int color_samples = 10;
int red_calibration_vals[color_samples];
int green_calibration_vals[color_samples];
int blue_calibration_vals[color_samples];
int color_vals[color_samples][4];
int color_ranges[4][3][2] = {
  { { 594, 689 },   { 27, 98 },   { 2, 53 } },
  { { 826, 910 }, { 623, 700 }, { 36, 109 } },
  {   { 15, 88 },  { 59, 140 }, { 99, 199 } },
  {   { 0, 113 },   { 0, 126 },  { 0, 115 } }
};  // Rows: ranges for each block (rybx)   Cols: Ranges for each LED (rgb)

// Line Following Vars
int kp = 250;
int ki = 600;
int kd = 4;
float ir_error, ir_error_last, ir_d_error, ir_integral_error;
int ir_bias[] = { 140, 94, 130, 134, 140, 140, 140, 140 };
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count];
int ir_unbiased[ir_sensor_count];
float ir_sensor_spacing[] = { 0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6 };  // cm
float ir_dist_desired = 2.8;                                           // cm
float ir_dist_actual, ir_dist_actualf;
float ir_alpha = 1;
bool all_black;
int black_count = 0;
float num = 0;
float den = 0;

// Range Finder Vars
float dist_alpha = 0.01;
float dist_actual_alpha = 0.4;
float dump_dist_upper = 13;
float dump_dist_lower;
float dump_dist_lower_right = 11;
float dump_dist_lower_left = 9.5;
float dist_collect = 10.4;
float dist_actual, dist_actualf;
float dist_to_wall = 11;
int num_dist_vals = 100;
float dist_val, dist_valf = 0;
float dist_desired;
float a1 = exp(7.453976699);
float b1 = -0.907499336;
float a2 = exp(7.869624);
float b2 = -1.05102;
bool stop = false;

// Event Bools
bool new_action;
bool next_node;
bool line_dist;
bool second_line;
bool last_line_state;
bool ramp_down = true;
bool home_dispense = true;
bool dist_right;


///////////////////////////////
///--------- SETUP ---------///
///////////////////////////////
void setup() {
  // Begin Serial Comms
  Serial.begin(250000);
  Serial2.begin(9600);
  while (!Serial && !Serial2) {
    ;  // Wait until connected
  }

  //Initializing and Enabling Driver Shield
  md.init();
  md.enableDrivers();

  // Attatch Pins to Servo Objects
  arm_servo.attach(arm_servo_pin);
  shovel_servo.attach(shovel_servo_pin);
  // shovel_servo.write(shov_max_angle);

  // Setup IR Array
  qtr.setTypeRC();
  qtr.setSensorPins(ir_pins, ir_sensor_count);

  // Set Color Sensor Pins
  pinMode(blue_pin, OUTPUT), digitalWrite(blue_pin, HIGH);
  pinMode(red_pin, OUTPUT), digitalWrite(red_pin, HIGH);
  pinMode(green_pin, OUTPUT), digitalWrite(green_pin, HIGH);

  // Setup turn LEDs
  pinMode(right_turn_pin, OUTPUT);
  pinMode(left_turn_pin, OUTPUT);

  // // Check for Start Command and Read Block Info
  // Serial.println("Ready For Signal...");
  // shovel_servo.write(shov_max_angle);
  // while (true) {
  //   if (Serial2.available()) {
  //     rc = Serial2.read();
  //     if (rc == flag) {
  //       Serial.println("START");
  //       shovel_servo.write(shov_button_angle);
  //       ReadBlockInfo();
  //       break;
  //     } // end if
  //   } // end if
  // } // end while
  // delay(200);
}


//////////////////////////////
///--------- LOOP ---------///
//////////////////////////////
void loop() {
  switch (state) {
    // Initiate Dispenser Travel
    case 'a':
      current_block.Reset();
      directions.AddTailNode('l', dist_collect, line_follow_speed, 0, 'a', servo_home, 2);
      dist_right = true;
      line_dist = true;
      new_action = true;
      last_state = state;
      state = 'd';
      break;


    // Collect Block
    case 'b':
      CollectBlock();
      new_action = true;
      last_state = state;
      state = 'd';
      break;


    // Color Sense
    case 'c':
      t = (millis() - t_start) / 1000;
      // Sense Color and change state
      if(current_block.color == '\0' || current_block.color == 'x') {
        ColorSense();
      } else {
        DetermineBlockLoc();
        Serial.print(current_block.face);
        Serial.print('\t');
        Serial.print(current_block.pos);
        Serial.print('\t');
        Serial.print(current_block.elev);
        Serial.print('\t');
        Serial.println(current_block.color);
        if (current_block.face != '\0') {
          GetDirections();
          shovel_servo.write(shov_travel_angle);
          new_action = true;
          last_state = state;
          state = 'd';
        } else {
          state = 'g';
        }
      }
      // Else collect another block
      if (t > block_wait_time) {
        if (current_block.color == '\0'){
          current_block.color = 'y';
        } else if (current_block.color == 'x') {
          current_block.Reset();
          last_state = 'a';
          state = 'b';
        }
      }
      break;


    // Travel
    case 'd':
      Travel();
      break;


    // Dump Block
    case 'e':
      DumpBlock();
      new_action = true;
      last_state = state;
      state = 'd';
      break;


    // Get Directions Home
    case 'f':
      GetDirectionsHome();
      new_action = true;
      last_state = state;
      state = 'd';
      break;
      
    // Dispose of Current Block
    case 'g':
      DumpExtraBlock();
      new_action = true;
      last_state = state;
      state = 'd';
  }
}