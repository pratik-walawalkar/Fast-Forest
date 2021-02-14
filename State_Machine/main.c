/**
 * @file main.c
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief Main entry point for ECU
 * @version 0.1
 * @date 2019-11-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "Global_Types.h"
#include <DCU_FF12.h>
#include "FF12_MB_PE_main.h"


int main(void)
{
    DCU_Run(MB_Init, 
        MB_PerformSelfTests,
        MB_Application5us,
        MB_Application1ms,
        MB_Application10ms,
        MB_Application100ms,
        MB_Application500ms,            
        MB_Application1s,
        MB_MAJOR_VERSION, 
        MB_MINOR_VERSION, 
        122);  
}