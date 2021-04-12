/* Arduino library for using stateMachines
 *
 * Created to teach my child how to program his "idea",
 * the autonomous fishing boat that can capture one
 * fish, and then needs to go home again :-)
 * library host on
 * https://github.com/cbratli/Arduino
 *
 *  Created on: Dec 14, 2014
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "Arduino.h"


class StateMachine
{
public:
	StateMachine(int16_t initialState);
	void transitionToState(int16_t nextState);
	int16_t getCurrentStateAndUpdateMachine();
	int16_t getCurrentStateNoUpdate();
	long getTotalTimeInState_ms();
	long getNumberOfScansInState();
	boolean isEnteringState();
	boolean isExitingState();
	void setStatePersistency_ms(int16_t newDelay);


private:
	int16_t currentState;
	int16_t lastState;		//
	long numberOfScansInState;	// Number of times the code has run.
					//(first run gets is 1 when getCurrentStateAndUpdateMachine() has run)
	long startTimeInState_ms;	// [ms] Number of ms of millis() when entering state.
	int16_t statePersistency_ms;	// [ms] Number of ms you have to be in a state
					// before you can leave for another state
				
};

#endif /* STATEMACHINE_H_ */
