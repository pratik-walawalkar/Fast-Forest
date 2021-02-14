/**
 * @file Statemachine.c
 * @author Pratik Walawalkar (pratik.walawalkar@fast-forest.de)
 * @brief DESCIPTION OF WHAT THIS IS
 * @version 0.1
 * @date 07-01-2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Statemachine.h"

int StateMachine_getCurrentState(void)
{
    /** TODO: Remove return statement and implement
     * your own function
    */
    
    //void idle();
    //void waitStage01();
    //void waitStage02();
    //void systemReady();
    //void error();

}

void StateMachine_run(void)
{
    /** TODO: Implement you own function 
     * 
     */
    if (doStartup|stage01){

        current_stage = 0;

        switch (current_state){
            case 0:
                if(isError)
                {
                    current_stage -- ;   
                }
                else if(doStartup|stage01())
                {
                    current_stage ++;
                }
                while(~doStartup && ~isError){
                    current_stage = 0;
                }    
            case 1:
                if(~doStartup)
                {
                    current_stage --;   
                }
                else if(stage01Ready|stage02())
                {
                    current_stage ++;
                }
                while(doStartup && ~stage01Ready){
                    current_stage = 1;
                }             
            case 2:
                if(~stage01Ready)
                {
                    current_stage --;    
                }
                else if(stage01Ready && stage02Ready)
                {
                    current_stage ++;
                } 
                while(~stage02Ready && stage01Ready){
                    current_stage = 2;
                }            
            case 3:
                if(~stage02Ready)
                {
                    current_stage --;   
                }
                else if(abort)
                {
                    current_stage = 0;
                } 
                else if(isError)
                {
                    current_stage = -1;
                }
                while(stage02Ready && stageReady){
                    current_stage = systemReady;
                }
            case -1:
                if(errorCleared && ~isError)
                {
                    current_stage ++;   
                }
                while(~errorCleared){
                    current_stage = -1;
                }                
            }
        
        }    
    }    
}


