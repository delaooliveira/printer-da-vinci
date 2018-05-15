/* 2-D printer arduino firmware 
 * Desinged by André João Pedro de Oliveira <andredelao@gmail.com> 
 */
#include <Stepper.h>

//Functions
void read_command();
void G28(Stepper, Stepper);
void G00(Stepper, Stepper, float, float);

//Global variables
String command;
bool taskDone = false;
 
void setup() {
  
  // Definitions of the pins and constants
  #define X_end               50
  #define Y_end               51
  #define Z_actuator          52
  #define X_step_1            53
  #define X_step_2            54
  #define X_step_3            55
  #define X_step_4            56
  #define Y_step_1            57
  #define Y_step_2            58
  #define Y_step_3            59
  #define Y_step_4            60 
  #define start               63
  #define emergency           64
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
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
   
  //while(digitalRead(start) == 0){
    // Wait for the button press to start the loop
  //}
}

  Stepper Motor_x(stepsPerRevolution, X_step_1, X_step_2, X_step_3, X_step_4);
  Stepper Motor_y(stepsPerRevolution, Y_step_1, Y_step_2, Y_step_3, Y_step_4);

void loop() {
  
  Motor_x.setSpeed(MAX_MOTOR_SPEED);
  Motor_y.setSpeed(MAX_MOTOR_SPEED);
  while(true){
  //while(digitalRead(emergency) == 0){ // Does the printing loop while the emergency button is NOT pressed
    read_command(); 

    if (command.startsWith("G28")) G28(Motor_x, Motor_y);
    
    
  }//}
 
}

void read_command(){

  while(!Serial){Serial.println("Waiting USB port to be available...");}

  if (Serial.available() > 0){
    command = Serial.readString();
    Serial.print("Command recived:");
    Serial.println(command);
    Serial.write(1); // token reguarding the acquaintance of the command line
    taskDone = false;    
  }
  
}

void G28(Stepper Motor_x, Stepper Motor_y){
  while(digitalRead(X_end) == 0){
    Motor_x.step(-1);
  }
  while(digitalRead(Y_end) == 0){
    Motor_y.step(-1);
  }
  taskDone = true;
}

void G00(Stepper Motor_x, Stepper Motor_y, float x_desired, float y_desired){

  //Calculating the number os steps each axis should move
  int xSteps, ySteps;
  xSteps = round(x_desired*anglePerStep*gearRadius*gearRatio);
  ySteps = round(y_desired*anglePerStep*gearRadius*gearRatio);

  // Moving each motor a 10th of the total each loop (Arduino doesn't support multi-threading)
  for (int i=0; i<10; i++){
    Motor_x.step(round(xSteps/10));   
    Motor_y.step(round(ySteps/10));
  }

  // This will correct any last differences
  Motor_x.step(xSteps%10);
  Motor_y.step(ySteps%10);

  taskDone = true;
}

