/* 2-D printer arduino firmware 
 * Desinged by André João Pedro de Oliveira <andredelao@gmail.com> 
 */
#include <Stepper.h>

//Functions
void read_command(String*);
void home(Stepper, Stepper);

//Global variables
String command;
 
void setup() {
  
  // Definitions of the pins and constants
  #define X_end               1
  #define Y_end               1
  #define Z_actuator          1
  #define X_step_1            1
  #define X_step_2            1
  #define X_step_3            1
  #define X_step_4            1
  #define Y_step_1            1
  #define Y_step_2            1
  #define Y_step_3            1
  #define Y_step_4            1  
  #define start               1 
  #define emergency           1
  #define stepsPerRevolution  2048
  #define anglePerStep        0.00306796157 // 2*pi/stepsPerRevolution | Unit: rad
  #define gearRadius          25            // Unit: mm
  #define gearRatio           10
  #define MAX_MOTOR_SPEED     10            // Unit: RPM

  // Modes of each pin defined above
  pinMode(X_end, INPUT);
  pinMode(Y_end, INPUT);
  pinMode(Z_actuator, OUTPUT);
  pinMode(X_step_1, OUTPUT);
  pinMode(X_step_2, OUTPUT);
  pinMode(X_step_3, OUTPUT);
  pinMode(X_step_4, OUTPUT);
  pinMode(Y_step_1, OUTPUT);
  pinMode(Y_step_2, OUTPUT);
  pinMode(Y_step_3, OUTPUT);
  pinMode(Y_step_4, OUTPUT);
  pinMode(start, INPUT);
  pinMode(emergency, INPUT);
 
  // Initializing the stepper motors
  Stepper Motor_x(stepsPerRevolution, X_step_1, X_step_2, X_step_3, X_step_4);
  Stepper Motor_y(stepsPerRevolution, Y_step_1, Y_step_2, Y_step_3, Y_step_4);
  Motor_x.setSpeed(MAX_MOTOR_SPEED);
  Motor_y.setSpeed(MAX_MOTOR_SPEED);

  Serial.begin(9600);
   
  while(digitalRead(start) == 0){
    // Wait for the button press to start the loop
  }
}

void loop() {
  
  while(digitalRead(emergency) == 0){ // Does the printing loop while the emergency button is NOT pressed
    read_command(&command); 
    
  }
 
}

void read_command(String *command){

  if (Serial.available() > 0){
    *command = Serial.readString();
    Serial.write("Command Recieved");
  }
  
}

void G28(Stepper Motor_x, Stepper Motor_y){
  while(digitalRead(X_end) == 0){
    Motor_x.step(-stepsPerRevolution);
  }
  while(digitalRead(Y_end) == 0){
    Motor_y.step(-stepsPerRevolution);
  }
}

void G00(Stepper Motor_x, Stepper Motor_y, float x_desired, float y_desired){

  //Calculating the number os steps each axis should move
  int xSteps, ySteps, meanStep;
  xSteps = round(x_desired*anglePerStep*gearRadius*gearRatio);
  ySteps = round(y_desired*anglePerStep*gearRadius*gearRatio);

  meanStep = (xSteps+ySteps)/2;
  
  for (int i=0; i<meanStep; i++){
    Motor_x.step(meanStep);   // FIX-ME: x and y steps are differents // Should do weighted average or sometinhg like that so they reach end point together
    Motor_y.step(meanStep);
  }
  
}

