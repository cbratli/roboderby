#include "StateMachine.h"

//MovingSum<int,6>  distanceCounterDiffOver6Samples;  // Note that it needs to be defined at the end of the template class.
//int distanceMovingSum = 0;
const int MOTOR1_PIN1 = 4;
const int MOTOR1_PIN2 = 5;
const int MOTOR2_PIN1 = 12;
const int MOTOR2_PIN2 = 14;


int prevTravelDistance = 0;

const int dT_ms=50;
float dT = 20e-3;

struct programConfig
{
  bool activateSteering;
  bool activateForwardMotor;
};

typedef struct programConfig ProgramConfig;

struct point
{
  float x;
  float y;
};
typedef struct point Point;


 enum gameStates {
  INIT,
  WAITING,
  RUNNING,
  STOP
 };
 volatile gameStates gameState = WAITING;



const int ESC_STATE_GOING_FORWARD = 1;
const int ESC_STATE_FORWARD_STOPPED = 2;
const int ESC_STATE_REVERSE_WAIT = 3;
const int ESC_STATE_REVERSE = 4;
const int ESC_STATE_READY_TO_START = 5;
const int ESC_STATE_INIT = 10;


const int GAMESTATE_INIT = 1;
const int GAMESTATE_ACCEPT_PROGRAMMING = 2;
const int GAMESTATE_PROGRAMMING_RECEIVED_SOON_TO_RUN = 3;
const int GAMESTATE_RUN_COMMANDS = 4;
const int GAMESTATE_ROUND_CLEANUP = 5;


StateMachine EscStateMachine(GAMESTATE_INIT);
//StateMachine EscStateMachine(gameStates.INIT);

long TIME_TO_ACCEPT_COMMANDS = 6000; 
long TIME_IN_RECEIVED_SOON_TO_RUN = 1000;
long T_RUN_COMMAND = 1000;
long T_CLEANUP = 1000;


struct carInfo
{
  bool forward;
  int ticksTravelled;
  int distanceMovingSum;
  int stoppedCounter;
  int counter;
  int state;
  int reverseRetries;
};

typedef struct carInfo CarInfo;
CarInfo carTravel; 


/*
 * Pins
 */

const int FORWARD_MOTOR_PWM_PIN = 10;


int LEFT_SENSOR_MAX_DIST = 1000;
int RIGHT_SENSOR_MAX_DIST = 1000;



const int LED_PIN = 13;  //Onboard LED
const int STARTMODULE_PIN = 5;
const int PHOTOSENSOR_PIN = 2;

const int SERVO_STEERING_PIN = 9;
const int SERVO_MOTOR_PIN = 3;

const int LED_SLOW_BLINK_US = 750000; // micro seconds (us)
const int LED_FAST_BLINK_US = 200000; //micro seconds (us)



/*
 * Constants
 */






