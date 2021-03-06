 enum PS4ControllerState {
    READY_FOR_INPUT,
    GETTING_INPUTS_L2,
    GETTING_INPUTS_LEFTSTICK
    };
StateMachine PS4ControllerStateMachine(PS4ControllerState::READY_FOR_INPUT);



void runPS4ControllerEngine()
{
          
          bool hasPS4ControllerInputR2L2 = (PS4.L2()||PS4.R2()||PS4.L1()||PS4.R1()) && PS4.isConnected();
          bool hasPS4ControllerInputLeftStick = (abs(PS4.LStickX()) > 10 || abs(PS4.LStickY()) > 10) && PS4.isConnected();
    
          switch (PS4ControllerStateMachine.getCurrentStateAndUpdateMachine())
          {
              case PS4ControllerState::READY_FOR_INPUT:
               if (PS4ControllerStateMachine.isEnteringState())
              {  
                // Just to show how it works.
               client.publish(mqttChannelOut, "INFO:PS4ControllerState::READY_FOR_INPUT");
              }

              if (hasPS4ControllerInputR2L2)
                {
                  PS4ControllerStateMachine.transitionToState(PS4ControllerState::GETTING_INPUTS_L2);    
                }
              if (hasPS4ControllerInputLeftStick)
              {
                PS4ControllerStateMachine.transitionToState(PS4ControllerState::GETTING_INPUTS_LEFTSTICK);    
                
                }
                
              break;  
          
            case PS4ControllerState::GETTING_INPUTS_L2:
              if (PS4ControllerStateMachine.isEnteringState())
              {  
               client.publish(mqttChannelOut, "INFO:PS4STATE_GETTING_INPUTS_L2");
              }
                setLeftMotorPwm(((float)(PS4.L2Value()-PS4.L1()*255.0))/255.0);
                setRightMotorPwm(((float)(PS4.R2Value()-PS4.R1()*255.0))/255.0);

              if (!hasPS4ControllerInputR2L2) {
                disableMotors();
                PS4ControllerStateMachine.transitionToState(PS4ControllerState::READY_FOR_INPUT);    
              }
              break;

            case PS4ControllerState::GETTING_INPUTS_LEFTSTICK:
              if (PS4ControllerStateMachine.isEnteringState())
              {  
               client.publish(mqttChannelOut, "INFO:PS4ControllerState::GETTING_INPUTS_LEFTSTICK");
              }
                float left = PS4.LStickY()/127.0;
                float right = PS4.LStickY()/127.0;

                left = left - PS4.LStickX()/127.0;
                right = right + PS4.LStickX()/127.0;
                
                setLeftMotorPwm(left);
                setRightMotorPwm(right);

              if (!hasPS4ControllerInputLeftStick) {
                disableMotors();
                PS4ControllerStateMachine.transitionToState(PS4ControllerState::READY_FOR_INPUT);    
              }
              break;
          }
}
