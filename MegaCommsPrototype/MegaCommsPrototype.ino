// Block Data Storage
char red1[] = {};
char blue[] = {};
char yellow[] = {};
char* block_data;

// Variables
int rc;
int num_blocks;
int flag = 33;
bool data_ready = false;

// Functions
void fill_devices(size_t size){
  block_data = malloc(size * sizeof(block_data));
}


void setup() {
  // Begin serial communication
  Serial.begin(9600);
  Serial3.begin(9600);
}


void loop() {
  // Check for start flag
  if(!data_ready){
    if(Serial3.available()>=2){                   // Check if theres data to read
      if(Serial3.read() == flag){                 // Check for start flag
        num_blocks = Serial3.read()-'0';          // Store number of blocks
        data_ready = true;
      }
    }
  }

  // Read data when it's in buffer
  if(data_ready){               
    if(Serial3.available()>=num_blocks*4){
      // char block_data[num_blocks*4] = {};       // Initialize block_data array
      block_data = (char*)calloc(num_blocks*4, sizeof(char));
      for(int i=0; i<num_blocks; i++){          // Save data for each block
        block_data[i*4+0] = Serial3.read();
        block_data[i*4+1] = Serial3.read();
        block_data[i*4+2] = Serial3.read();
        block_data[i*4+3] = Serial3.read();
      }
      Serial.write(block_data);
      data_ready = false;
    }
  }

  // Dump buffer
  while(Serial3.available()){
    Serial3.read();
  }
  // if(Serial3.available()){
  //   rc = Serial3.read();
  //   Serial.print(rc);
  // }

}
