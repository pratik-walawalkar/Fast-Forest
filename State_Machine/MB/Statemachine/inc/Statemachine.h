/**
 * @file Statemachine.c
 * @author YOUR NAME (YOUR EMAIL)
 * @brief DESCIPTION OF WHAT THIS IS
 * @version 0.1
 * @date DATE
 * 
 * @copyright Copyright (c) YEAR
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Returns the ID of the current state
 * 
 * @return int
 */
int StateMachine_getCurrentState(void);

/**
 * @brief Does one step of the statemachine
 * 
 */
void StateMachine_run(void);

#ifdef __cplusplus
}
#endif
