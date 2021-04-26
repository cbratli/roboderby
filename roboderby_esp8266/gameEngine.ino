void runGameEngine()
{
          switch (GameStateMachine.getCurrentStateAndUpdateMachine())
          {
              case gameState::INIT:
               if (GameStateMachine.isEnteringState())
              {  
                // Just to show how it works.
               client.publish(mqttChannelOut, "INFO:GAMESTATE::INIT");
              }

              if (GameStateMachine.getTotalTimeInState_ms() > 700)
                {
                  GameStateMachine.transitionToState(gameState::ACCEPT_PROGRAMMING);    
                }
              break;  
          
            case gameState::ACCEPT_PROGRAMMING:
            if (GameStateMachine.isEnteringState())
              {  
               client.publish(mqttChannelOut, "INFO:GAMESTATE::ACCEPT_PROGRAMMING");
              }
              
              if (GameStateMachine.getTotalTimeInState_ms() > TIME_TO_ACCEPT_COMMANDS)
              {
                GameStateMachine.transitionToState(gameState::PROGRAMMING_RECEIVED_SOON_TO_RUN);    
              }
              break;
            
            case gameState::PROGRAMMING_RECEIVED_SOON_TO_RUN:
              if (GameStateMachine.isEnteringState())
              {  
               client.publish(mqttChannelOut, "GAMESTATE::PROGRAMMING_RECEIVED_SOON_TO_RUN:");
              }

              if (GameStateMachine.getTotalTimeInState_ms() > TIME_IN_RECEIVED_SOON_TO_RUN)
              {
                GameStateMachine.transitionToState(gameState::RUN_COMMANDS);    
              }
              break;


            case gameState::RUN_COMMANDS:
              if (GameStateMachine.isEnteringState())
              {  
                client.publish(mqttChannelOut, "GAMESTATE::RUN_COMMANDS:");
              }

              if (GameStateMachine.getTotalTimeInState_ms() > T_RUN_COMMAND)
              {
                GameStateMachine.transitionToState(gameState::ROUND_CLEANUP);    
              }
            break;
          

              case gameState::ROUND_CLEANUP:
              if (GameStateMachine.isEnteringState())
              {  
                client.publish(mqttChannelOut, "gameState::RUN_COMMANDS:");
              }

              if (GameStateMachine.getTotalTimeInState_ms() > T_CLEANUP)
              {
                GameStateMachine.transitionToState(gameState::INIT);
              }
            break;
          }
    
}
