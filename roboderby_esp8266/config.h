#include "StateMachine.h"

char* bluetoothIdPS4ControllerWillConnectTo = "94:e6:f7:20:bb:69";
const int MOTOR1_PIN1 = 22;
const int MOTOR1_PIN2 = 23;
const int MOTOR2_PIN1 = 25;
const int MOTOR2_PIN2 = 26;

const float MOTOR_PWM_RANGE = 255.0;

const int dT_ms=50;
float dT = dT_ms/1000;

 enum gameState {
  INIT,
  ACCEPT_PROGRAMMING,
  PROGRAMMING_RECEIVED_SOON_TO_RUN,
  RUN_COMMANDS,
  ROUND_CLEANUP
 };
StateMachine GameStateMachine(gameState::INIT);
 
 enum PS4ControllerState {
    READY_FOR_INPUT,
    GETTING_INPUTS_L2,
    GETTING_INPUTS_LEFTSTICK
    };
StateMachine PS4ControllerStateMachine(PS4ControllerState::READY_FOR_INPUT);


long TIME_TO_ACCEPT_COMMANDS = 6000; 
long TIME_IN_RECEIVED_SOON_TO_RUN = 1000;
long T_RUN_COMMAND = 1000;
long T_CLEANUP = 1000;
