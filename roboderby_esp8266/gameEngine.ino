void runGameEngine()
{
      
          switch (EscStateMachine.getCurrentStateAndUpdateMachine())
          {
              case GAMESTATE_INIT:
           // case ESC_STATE_INIT:
               if (EscStateMachine.isEnteringState())
              {  
                // Just to show how it works.
               client.publish(mqttChannelOut, "INFO:GAMESTATE_INIT");
              }

              if (EscStateMachine.getTotalTimeInState_ms() > 700)
                {
                  //EscStateMachine.transitionToState(ESC_STATE_READY_TO_START);    
                  EscStateMachine.transitionToState(GAMESTATE_ACCEPT_PROGRAMMING);    
                }
              break;  
          
            case GAMESTATE_ACCEPT_PROGRAMMING:
            if (EscStateMachine.isEnteringState())
              {  
               client.publish(mqttChannelOut, "INFO:GAMESTATE_ACCEPT_PROGRAMMING");
              }
              
              if (EscStateMachine.getTotalTimeInState_ms() > TIME_TO_ACCEPT_COMMANDS)
              {
                EscStateMachine.transitionToState(GAMESTATE_PROGRAMMING_RECEIVED_SOON_TO_RUN);    
              }
              break;
            
            case GAMESTATE_PROGRAMMING_RECEIVED_SOON_TO_RUN:
              if (EscStateMachine.isEnteringState())
              {  
               client.publish(mqttChannelOut, "GAMESTATE_PROGRAMMING_RECEIVED_SOON_TO_RUN:");
              }

              if (EscStateMachine.getTotalTimeInState_ms() > TIME_IN_RECEIVED_SOON_TO_RUN)
              {
                EscStateMachine.transitionToState(GAMESTATE_RUN_COMMANDS);    
              }
              break;


            case GAMESTATE_RUN_COMMANDS:
              if (EscStateMachine.isEnteringState())
              {  
                client.publish(mqttChannelOut, "GAMESTATE_RUN_COMMANDS:");
              }

              if (EscStateMachine.getTotalTimeInState_ms() > T_RUN_COMMAND)
              {
                EscStateMachine.transitionToState(GAMESTATE_ROUND_CLEANUP);    
              }
            break;
          

              case GAMESTATE_ROUND_CLEANUP:
              if (EscStateMachine.isEnteringState())
              {  
                client.publish(mqttChannelOut, "GAMESTATE_RUN_COMMANDS:");
              }

              if (EscStateMachine.getTotalTimeInState_ms() > T_CLEANUP)
              {
                EscStateMachine.transitionToState(GAMESTATE_INIT);    
              }
            break;
          }
    
}

