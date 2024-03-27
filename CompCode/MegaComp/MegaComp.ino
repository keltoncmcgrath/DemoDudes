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
  // Add new node at end of LL
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
  // Delete node at end of LL
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
const uint8_t ir_pins[] = { 24, 25, 26, 27, 28, 29, 30, 31 };
int right_turn_pin;
int left_turn_pin;

// Analog
int dist_pin = A8;
int mag_pin = A9;
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
struct block current_block = { 's', '1', 'l', false, 'y' };
struct block read_block;
struct block red1[10] = {
  { 'w', '4', 'l', false },
  { 'w', '5', 'l', false },
  { 'w', '6', 'l', false },
  { 'e', '4', 'l', false },
  { 'e', '5', 'l', false },
  { 'e', '6', 'l', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};
struct block red2[10] = {
  { 'n', '1', 'l', false },
  { 'n', '2', 'l', false },
  { 'n', '3', 'l', false },
  { 's', '1', 'l', false },
  { 's', '2', 'l', false },
  { 's', '3', 'l', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};
struct block yellow1[10] = {
  { 'n', '1', 'l', false },
  { 'n', '2', 'l', false },
  { 'n', '3', 'l', false },
  { 's', '1', 'l', false },
  { 's', '2', 'l', false },
  { 's', '3', 'l', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};
struct block yellow2[10] = {
  { 'w', '4', 'l', false },
  { 'w', '5', 'l', false },
  { 'w', '6', 'l', false },
  { 'e', '4', 'l', false },
  { 'e', '5', 'l', false },
  { 'e', '6', 'l', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};
struct block blue[8] = {
  { 'n', '1', 'u', false },
  { 'n', '3', 'u', false },
  { 's', '1', 'u', false },
  { 's', '3', 'u', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};

// Trajectory Vars
float KP = 500;
int straight_kp = 125;
int counts_per_rev = 64;
int gear_ratio = 131;
float wheel_radius = 3.5;        // cm
float wheel_dist_arc = 20.35;    // cm
float wheel_dist_turn;           // cm
float turn_time = 2;             // s
float dist_final;                // cm
float turn_angle_final;          // rad
float arc_radius;                // cm
float arc_angle_final;           // rad
float time_final;
char turn_dir;
char travel_dir;
float theta1;
float theta2;
float theta1_final;
float theta2_final;
float theta1_des;
float theta2_des;
float omega1_des;
float omega2_des;
long counts1;
long counts2;
float dist_traveled;
int m1s;
int m2s;

// Travel Variables
float east_guide = 122.5;   // cm
float south_guide = 68.58;  // cm
float north_guide = 34.5;   // cm
float guide1 = 55;          // cm
float guide2 = 61.5;        // cm
float guide3 = 70;          // cm
float guide4 = 55;          // cm, measure dist later
float guide5 = 61.5;        // cm, measure dist later
float guide6 = 70;          // cm, measure dist later
float collect_dist = 5.5;   // cm
int line_follow_speed = 300;
int line_speed;

// Encoder Vars
int encoder1_count;
int encoder2_count;
Encoder encoder1(encoder1_pinA, encoder1_pinB);
Encoder encoder2(encoder2_pinA, encoder2_pinB);

// Arm Servo Vars
int servo_home = 93;
int arm_max_angle = 25;
int arm_collect_angle = 73;
int arm_low_dump_angle = 75;
float arm_angle_des;
int arm_angle_start;
int arm_angle_final;
float arm_t_final = 1;
float arm_tol = 0.1;

// Shovel Servo Vars
int shov_max_angle = 150;
int shov_dump_angle = 180;
int shov_collect_angle = 75;
float shov_angle_des;
int shov_angle_start;
int shov_angle_final;
float shov_t_final = 1;
float shov_tol = 0.1;

// Color Sensor Vars
int color_delay_time = 10;
int block_wait_time = 5;
bool is_color = false;
int red_sum = 0;
int green_sum = 0;
int blue_sum = 0;
int red_val;
int green_val;
int blue_val;
float red_avg[3];
float green_avg[3];
float blue_avg[3];
float red_std[3] = { 0, 0, 0 };
float green_std[3] = { 0, 0, 0 };
float blue_std[3] = { 0, 0, 0 };

// Color Calibration Vars
const int color_samples = 100;
int red_calibration_vals[color_samples];
int green_calibration_vals[color_samples];
int blue_calibration_vals[color_samples];
int color_vals[color_samples][3];
int color_ranges[3][3][2] = {
  { { 261, 328 }, { 8, 53 }, { -5, 19 } },
  { { 340, 370 }, { 330, 365 }, { 15, 50 } },
  { { -4, 24 }, { 14, 63 }, { 12, 63 } }
};  // Rows: ranges for each block (ryb)   Cols: Ranges for each LED (rgb)

// Line Following Vars
int kp = 300;
int ir_bias[] = { 144, 93, 93, 93, 136, 93, 98, 119 };
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count];
int ir_unbiased[ir_sensor_count];
float ir_sensor_spacing[] = { 0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6 };  // cm
float ir_dist_desired = 2.8;                                           // cm
float ir_dist_actual;
float ir_error;
bool all_black;
float num = 0;
float den = 0;

// Range Finder Vars
float dump_dist_upper = 13;  // cm
float dump_dist_lower = 11;  // cm
float dist_collect = 14;     // cm
float dist_actual = 1000;    // cm
float dist_to_wall = 15;
int num_dist_vals = 50;
float dist_val = 0;
float dist_desired;
float a = exp(7.453976699);
float b = -0.907499336;
bool stop = false;

// Mag Vars
float mag_val;
int mag_ss = 517;

// Event Bools
bool final_stage;
bool final_final_stage;
bool turn_bool;
bool straight_bool;
bool servo_bool;
bool line_follow_bool;
bool new_action;
bool next_node;
bool line_dist;
bool second_line;
bool last_line_state;


///////////////////////////////
///--------- SETUP ---------///
///////////////////////////////
void setup() {
  // Begin Serial Comms
  Serial.begin(9600);
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

  // Setup IR Array
  qtr.setTypeRC();
  qtr.setSensorPins(ir_pins, ir_sensor_count);

  // Set Color Sensor Pins
  pinMode(blue_pin, OUTPUT);          digitalWrite(blue_pin, HIGH);
  pinMode(red_pin, OUTPUT);           digitalWrite(red_pin, HIGH);
  pinMode(green_pin, OUTPUT);         digitalWrite(green_pin, HIGH);

  // Setup turn LEDs
  pinMode(right_turn_pin, OUTPUT);    pinMode(left_turn_pin, OUTPUT);

  // Check for Start Command and Read Block Info
  Serial.println("Ready For Signal...");
  while (true) {
    if (Serial2.available()) {
      rc = Serial2.read();
      if (rc == flag) {
        ReadBlockInfo();
        break;
      }
    }
  }
}


//////////////////////////////
///--------- LOOP ---------///
//////////////////////////////
void loop() {
  if(state != 'd'){
    Serial.println(state);
  }
  switch (state) {
    // Initiate Dispenser Travel
    case 'a':
      directions.AddTailNode('l', dist_collect, 0, 0, 'a', arm_collect_angle, 2);
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
      // ColorSense();
      // Continue if color is detected
      if (current_block.color != '\0') {  //current_block.color != '\0'
        // DetermineBlockLoc();
        Serial.print(current_block.face);
        Serial.print('\t');
        Serial.print(current_block.pos);
        Serial.print('\t');
        Serial.print(current_block.elev);
        Serial.print('\t');
        Serial.println(current_block.color);
        GetDirections();
        new_action = true;
        last_state = state;
        state = 'd';
      }
      // Else collect another block
      else if (t > block_wait_time) {
        state = 'b';
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

    case 'f':
      GetDirectionsHome();
      new_action = true;
      last_state = state;
      state = 'd';
      break;
  }
}