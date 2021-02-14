/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_Application.h"
#include "DCU_FF12.h"
#include "DCU_FF12_GPIO.h"
#include "DCU_FF12_Oscillator.h"
#include "DCU_FF12_Timer.h"
#include "DCU_FF12_CAN.h"
#include "DCU_FF12_ADC.h"
#include "DCU_FF12_I2C.h"
#include "DCU_FF12_EEPROM.h"
#include "DCU_FF12_Configuration.h"
#include "DCU_FF12_SoftStart.h"
#include "DCU_FF12_HWID.h"
#include "DCU_FF12_VoltageSensing.h"
#include "FastGen_CAN_Functions.h"
#include "FastGen_CAN_Var.h"
#include "FastGen_Config.h"
#include <p33EP512MU810.h>

result_t Init(void)
{
    result_t result = R_SUCCESS;

    InitPinsAsDefault();
    InitOscillator();
    InitPPS();
    
    InitAndActivateSoftStartOverride();
    InitHWID();
        
    InitTimer1As1ms();
            
    InitStatusAndLEDs();

    InitCAN();
    
    if(RCONbits.WDTO == 1)
    {
        can_var.send.EW_DCU_resetCausedByWatchdog = 1;
        RCONbits.WDTO = 0;
    }
    
    InitI2C2();   
    InitEEPROM();    

    return result;
}

result_t SelfTest(void)
{
    result_t result = R_SUCCESS;

    if(SelfTestStatusLEDs() != R_SUCCESS)
    {
        result |= R_FAILED_SELFTEST_NON_CRITICAL;
    }    
    
    if(SelfTestHWID() != R_SUCCESS)
    {
        can_var.send.EC_MB_HwIdMissmatch = 1;    
        result |= R_FAILED; // critical   
        return result;
    }
    
    if(SelfTestEEPROMCommunication() != R_SUCCESS)
    {
        can_var.send.EC_EEPROM_communication = 1;   
        result |= R_FAILED; // critical
        return result;
    }    
    
    if(SelfTestEEPROMHWID() != R_SUCCESS)
    {
        can_var.send.EE_EEPROM_HwIdMissmatch = 1;
        CleanConfiguration();
        result |= R_FAILED_SELFTEST_NON_CRITICAL;
    }
    
    CheckForSWVersionChangesEEPROM();         

    return result;
}

void PostDCUSelfTest(void)
{ 
    LoadConfiguration();
    if(FASTGEN_CONFIG_CP_DCU_disable_watchdog != 1)
    {        
        RCONbits.SWDTEN = 1;                         // Enable Watchdog
        __builtin_clrwdt();                          // Clear Watchdog
    }
}

void PostMBInit(void)
{ 
    InitDCUVoltageSensing();    
    InitADC();
}

void Application5us(void)
{
    if(dcuVar.modelBoard.app_5us_func != NULLPTR)
        dcuVar.modelBoard.app_5us_func();
}

void Application1ms(void)
{
    if(dcuVar.modelBoard.app_1ms_func != NULLPTR)
        dcuVar.modelBoard.app_1ms_func();
}

void Application10ms(void)
{
    CheckStatusLedFeedbacks();
    CheckDCUVoltages();    
    
    #ifdef DCU_SEND_STATUS_10MS
        FASTGEN_Send_CAN2_Status();
    #endif        
        
    ConfigurationHandler10ms();

    if(dcuVar.modelBoard.app_10ms_func != NULLPTR)
        dcuVar.modelBoard.app_10ms_func();
}

void Application100ms(void)
{
    #ifndef DCU_SEND_STATUS_10MS
        FASTGEN_Send_CAN2_Status();
    #endif
    CheckStatusAndUpdateLEDs();
    
    FASTGEN_Send_CAN2_Error();
    
    if(dcuVar.modelBoard.app_100ms_func != NULLPTR)
        dcuVar.modelBoard.app_100ms_func();
}

void Application500ms(void)
{
    FASTGEN_Send_CAN2_Warning();

    if(dcuVar.modelBoard.app_500ms_func != NULLPTR)
        dcuVar.modelBoard.app_500ms_func();
}

void Application1s(void)
{
    DoDCUVoltagesCalibration();
    
    ConfigurationHandler1s();

    if(dcuVar.modelBoard.app_1s_func != NULLPTR)
        dcuVar.modelBoard.app_1s_func();
    
    __builtin_clrwdt();
}

void ApplicationIdle(void)
{

}

void FaultLoop(void)
{
    SetFaultStatus();
    while(TRUE)
    {
        FASTGEN_Send_CAN2_Error();
        Wait_ms(500); 
        __builtin_clrwdt();
    }
}

void ApplicationADCInterrupt(void)
{
    if(dcuVar.modelBoard.dma_callback_func != NULLPTR)
        dcuVar.modelBoard.dma_callback_func();
}

void Reset(void)
{
    Wait_ms(500); 
    while(TRUE)
    {
        __asm__ volatile ("reset");
    }
}


