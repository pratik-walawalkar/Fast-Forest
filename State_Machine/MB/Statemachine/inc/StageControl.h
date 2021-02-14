/**
 * @file StageControl.h
 * @author Fabian Bally (fabian.bally@fast-forest.de)
 * @brief 
 * @version 0.1
 * @date 2020-11-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "Global_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Checks if stage control has encountered an error
 * 
 * @return boolean 
 * @retval true Error has occured
 * @retval false No error
 */
boolean StageControl_isError(void);

/**
 * @brief Checks if previous error has cleared
 * 
 * @return boolean 
 * @retval true error has cleared
 * @retval false error has not yet cleared
 */
boolean StageControl_errorCleared(void);

/**
 * @brief Checks if startup should be done
 * 
 * @return boolean 
 * @retval true do startup now
 * @retval do nothing
 */
boolean StageControl_doStartup(void);

/**
 * @brief Initialize stage 01
 * 
 */
void StageControl_setStage01(void);

/**
 * @brief Initialize stage 02
 * 
 */
void StageControl_setState02(void);

/**
 * @brief Checks if stage 01 is finished initializing
 * 
 * @return boolean 
 * @retval true stage 01 is finished
 * @retval false stage 01 is not yet finished
 */
boolean StageControl_stage01Finished(void);

/**
 * @brief Checks if stage 02 is finished initializing
 * 
 * @return boolean 
 * @retval true stage 02 is finished
 * @retval false stage 02 is not yet finished
 */
boolean StageControl_stage02Finished(void);

// -------------------------------------------------- //
// vvvvvvvv NOTHING IMPORTANT FROM HERE ON vvvvvvvvvv //
// -------------------------------------------------- //

#pragma region MOCK

boolean errorFlag;
boolean stage01Set;
boolean stage02Set;
boolean systemReadySet;

boolean stage01FinishedFlag;
boolean stage02FinishedFlag;

boolean abortFlag;
boolean doStartupFlag;

#pragma endregion

#ifdef __cplusplus
}
#endif
