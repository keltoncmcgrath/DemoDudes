#include <DualTB9051FTGMotorShieldUnoMega.h>
#include <Encoder.h>

DualTB9051FTGMotorShieldUnoMega myMotorDriver;
Encoder myEncoder1(19, 18);
Encoder myEncoder2(20, 21);

double GearRatio = 70;
double EncoderCountsPerRev = 64;

unsigned long t_ms = 0;
double theta1, omega1, omega1_des, omega1f = 0;
double theta1_old = 0;
double theta1_des, theta1_desf = 0;
double theta2, omega2, omega2_des, omega2f = 0;
double theta2_old = 0;
double theta2_des, theta2_desf = 0;
double V1, V2;  // motor voltage command
double t, t_old, t0, deltaT;
double Kp, Kd, Ki;  //PID gains
double error1, dErrordt1, integralError1 = 0;
double error2, dErrordt2, integralError2 = 0;
double StepSize;
double alpha;  //digital filter weight
double alpha_theta = 0.1;
double StepDuration;


void setup() {

  Serial.begin(250000);  //need to use high baud rate when printing during feedback control
  Serial.println("Arduino Ready to Receive Control Parameters");
  myMotorDriver.init();
  myMotorDriver.enableDrivers();
  myEncoder1.write(0);
  myEncoder2.write(0);

  String inString = "";
  while (Serial.available() == 0)
    ;
  inString = Serial.readStringUntil(' ');
  Kp = inString.toFloat();
  inString = Serial.readStringUntil(' ');
  Kd = inString.toFloat();
  inString = Serial.readStringUntil(' ');
  Ki = inString.toFloat();
  inString = Serial.readStringUntil(' ');
  StepSize = inString.toFloat();
  inString = Serial.readStringUntil(' ');
  alpha = inString.toFloat();
  inString = Serial.readStringUntil(' ');
  StepDuration = inString.toFloat();  // your desired velocity for 4.5.5 is StepSize/StepDuration
  t0 = micros() / 1000000.;           // do this once before starting feedback control
  t_old = 0;                          // do this once before starting feedback control
}


void loop() {

  // Time and time step
  t = micros() / 1000000.0 - t0;  // Current time minus initialization time
  deltaT = t - t_old;             // Sample time

  // Encoder sensing
  theta1 = myEncoder1.read() * 2 * PI / (GearRatio * EncoderCountsPerRev);  //Shaft position in radians
  theta2 = myEncoder2.read() * 2 * PI / (GearRatio * EncoderCountsPerRev);

  // Define Desired Trajectory
  omega1_des = StepSize / (StepDuration - 1);  // desired velocity is zero for a step in desired position
  omega2_des = StepSize / (StepDuration - 1);  // desired velocity is zero for a step in desired position
  theta1_des += omega1_des * deltaT;             // step input = a constant desired position
  theta2_des += omega2_des * deltaT;             // step input = a constant desired position
  theta1_desf = alpha_theta*theta1_des + (1-alpha_theta)*theta1_desf;
  theta2_desf = alpha_theta*theta2_des + (1-alpha_theta)*theta2_desf;
  if (theta1_desf >= StepSize) {
    theta1_desf = StepSize;
  }
  if (theta2_desf >= StepSize) {
    theta2_desf = StepSize;
  }

  // Calculate omega and filter
  omega1 = (theta1 - theta1_old) / deltaT;
  omega1f = alpha * omega1 + (1 - alpha) * omega1f;
  omega2 = (theta2 - theta2_old) / deltaT;
  omega2f = alpha * omega2 + (1 - alpha) * omega2f;

  // Compute Errors
  error1 = theta1_desf - theta1;
  dErrordt1 = omega1_des - omega1f;
  integralError1 += error1 * deltaT;
  integralError1 = constrain(integralError1, -10. / Ki, 10. / Ki);
  error2 = theta2_desf - theta2;
  dErrordt2 = omega2_des - omega2f;
  integralError2 += error2 * deltaT;
  integralError2 = constrain(integralError2, -10. / Ki, 10. / Ki);
  // PID Control Law
  V1 = Kp * error1 + Kd * dErrordt1 + Ki * integralError1;
  V2 = Kp * error2 + Kd * dErrordt2 + Ki * integralError2;

  myMotorDriver.setM1Speed(400 * V1 / 10);  // Scale and send motor command
  myMotorDriver.setM2Speed(400 * V2 / 10);

  // Prepare for next iteration
  t_old = t;
  theta1_old = theta1;
  theta2_old = theta2;
  /////////////////////////////////////////////////////

  if (t < StepDuration) {
    Serial.print(t, 5);
    Serial.print('\t');
    Serial.print((theta1 + theta2) / 2, 5);
    // Serial.print(theta2);
    Serial.print('\t');
    Serial.print((dErrordt1 + dErrordt2) / 2, 5);
    Serial.print('\t');
    Serial.print((integralError1 + integralError2) / 2, 5);
    Serial.print('\t');
    Serial.print((V1 + V2) / 2, 5);
    Serial.print('\t');
    Serial.print((theta1_desf + theta2_desf) / 2, 5);
    Serial.println();
  }
}
