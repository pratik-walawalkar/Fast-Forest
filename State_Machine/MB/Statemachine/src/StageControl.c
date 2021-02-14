/**
 * @file StageControl.c
 * @author Fabian Bally (fabian.bally@fast-forest.de)
 * @brief 
 * @version 0.1
 * @date 2020-11-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "StageControl.h"

boolean errorFlag = FALSE;
boolean stage01Set = FALSE;
boolean stage02Set = FALSE;
boolean systemReadySet = FALSE;

boolean stage01FinishedFlag = FALSE;
boolean stage02FinishedFlag = FALSE;

boolean abortFlag = FALSE;
boolean doStartupSwitch = FALSE;

boolean isError(void)
{
    return errorFlag;
}
boolean errorCleared(void)
{
    return !errorFlag;
}

void setStage01(void)
{
    stage01Set = TRUE;
}

void setState02(void)
{
    stage02Set = TRUE;
}

void systemReady(void)
{
    return;
}

boolean doStartup(void)
{
    if (doStartupSwitch)
    {
        doStartupSwitch = FALSE;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

boolean stage01Ready(void)
{
    return stage01FinishedFlag;
}

boolean stage02Ready(void)
{
    return stage02FinishedFlag;
}

