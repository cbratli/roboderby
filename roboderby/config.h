#include "StateMachine.h"

char* bluetoothIdPS4ControllerWillConnectTo = "94:e6:f7:20:bb:69";
//char* bluetoothIdPS4ControllerWillConnectTo = "E4:A7:A0:21:E9:AD";
//const int MOTOR1_PIN1 = 21;
const int MOTOR1_PIN1 = 22;
const int MOTOR1_PIN2 = 23;
const int MOTOR2_PIN1 = 25;
const int MOTOR2_PIN2 = 26;

const float MOTOR_PWM_RANGE = 255.0;

const int dT_ms=50;
float dT = dT_ms/1000;
 
long TIME_TO_ACCEPT_COMMANDS = 6000; 
long TIME_IN_RECEIVED_SOON_TO_RUN = 1000;
long T_RUN_COMMAND = 1000;
long T_CLEANUP = 1000;
