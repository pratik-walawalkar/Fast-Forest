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
    
    void idle();
    void waitStage01();
    void waitStage02();
    void systemReady();
    void error();

}

void StateMachine_run(void)
{
    /** TODO: Implement you own function 
     * 
     */
    if (doStartup|stage01){

        current_stage = idle;

        void idle(current_stage){
            if(isError)
            {
                current_stage = error;   
            }
            else if(doStartup|stage01())
            {
                current_stage = waitStage01;
            }
            while(~doStartup && ~isError){
                current_stage = idle;
            }
        }
        void waitStage01(current_stage){
            if(~doStartup)
            {
                current_stage = idle;   
            }
            else if(stage01Ready|stage02())
            {
                current_stage = waitStage02;
            }
            while(doStartup && ~stage01Ready){
                current_stage = waitStage01;
            }          
        }
        void waitStage02(current_stage){
            if(~stage01Ready)
            {
                current_stage = waitStage01;    
            }
            else if(stage01Ready && stage02Ready)
            {
                current_stage = systemReady;
            } 
            while(~stage02Ready && stage01Ready){
                current_stage = waitStage02;
            } 
        }
        void systemReady(current_stage){
            if(~stage02Ready)
            {
                current_stage = waitStage02;   
            }
            else if(abort)
            {
                current_stage = idle;
            } 
            else if(isError)
            {
                current_stage = Error;
            }
            while(stage02Ready && stageReady){
                current_stage = systemReady;
            }         
        }
        void error(current_stage){
            if(errorCleared && ~isError)
            {
                current_stage = idle;   
            }
            while(~errorCleared){
                current_stage = error;
            }
        }    
    }    
}