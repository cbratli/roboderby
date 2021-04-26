/*
 * stateMachine.cpp
 *
 *  Created on: 2014 dec 13
 *      Author: bratli
 */


#include "StateMachine.h"

StateMachine::StateMachine(int16_t initialState)
{
	this->currentState = initialState;
	this->setStatePersistency_ms(0);	// No persistency in a state.
	this->transitionToState(initialState);

}


void StateMachine::transitionToState(int16_t nextState)
{
	if (millis()-this->startTimeInState_ms >= this->statePersistency_ms)
	{
		this->lastState = this->currentState;
		this->currentState = nextState;
		this->numberOfScansInState = 0;
		this->startTimeInState_ms = millis();
	}
}


int16_t StateMachine::getCurrentStateAndUpdateMachine()
{
	this->numberOfScansInState++;
	return this->currentState;
}

int16_t StateMachine::getCurrentStateNoUpdate()
{
	return this->currentState;
}


long StateMachine::getTotalTimeInState_ms()
{
	return millis() - this->startTimeInState_ms;
}

long StateMachine::getNumberOfScansInState()
{
	return this->numberOfScansInState;
}

boolean StateMachine::isEnteringState()
{
	return this->numberOfScansInState==1;
}

boolean StateMachine::isExitingState()
{
	return this->numberOfScansInState==0;
}

void StateMachine::setStatePersistency_ms(int16_t statePersistency_ms)
{
	this->statePersistency_ms = statePersistency_ms;
}
