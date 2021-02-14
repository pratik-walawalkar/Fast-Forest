/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-09-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_Configuration.h"
#include "DCU_FF12_EEPROM.h"
#include "FastGen_CAN_Functions.h"
#include "FastGen_CAN_Var.h"
#include "FastGen_Config.h"


void LoadConfiguration(void)
{
    GetAllInfosFromEEPROM();
    GetAllParametersFromEEPROM();
    GetAllCalibrationsFromEEPROM();
}

void CleanConfiguration(void)
{
    CleanAllInfosFromEEPROM();
    CleanAllParametersFromEEPROM();
    CleanAllCalibrationsFromEEPROM();
    can_var.send.EW_EEPROM_wasCleared = 1;
}

void ConfigurationHandler10ms(void)
{   
    static uint16 ConfigCursor = 0;
    static uint16 CurrentConfigArrayCursor = 0;
    uint16 CurrentConfigArrayLimit = 1;
    
    if(FASTGEN_CONFIG_CC_EEPROM_send_Config >= FASTGEN_CONFIG_FIRST_MUX_PARAMETER_ARRAY &&
       FASTGEN_CONFIG_CC_EEPROM_send_Config <= FASTGEN_CONFIG_LAST_MUX_INFORMATION_ARRAY)
    {     
        FASTGEN_Send_ConfigMux(FASTGEN_CONFIG_CC_EEPROM_send_Config);
        
        FASTGEN_CONFIG_SET_CC_EEPROM_send_Config(0);        
    }
    else if(FASTGEN_CONFIG_CC_EEPROM_send_all_Config == 1)
    {     
        FASTGEN_CONFIG_SET_CC_EEPROM_send_all_Config(0);
        
        if(ConfigCursor == 0)
        {
            ConfigCursor = 1;
            CurrentConfigArrayCursor = 0;
        }
    }    
    
    if (ConfigCursor > 0) 
    {
        switch (ConfigCursor)
        {
            case 1:
                CurrentConfigArrayLimit = FASTGEN_CONFIG_SIZE_PARAMETER_ARRAY; 
                FASTGEN_Send_Parameter(CurrentConfigArrayCursor);
                CurrentConfigArrayCursor++;
                break;
            case 2:
                CurrentConfigArrayLimit = FASTGEN_CONFIG_SIZE_CALIBRATION_ARRAY; 
                FASTGEN_Send_Calibration(CurrentConfigArrayCursor);
                CurrentConfigArrayCursor++;
                break;
            case 3:
                CurrentConfigArrayLimit = FASTGEN_CONFIG_SIZE_INFORMATION_ARRAY; 
                FASTGEN_Send_Information(CurrentConfigArrayCursor);
                CurrentConfigArrayCursor++;
                break;
            default:
                ConfigCursor = 0;

        }
        if (CurrentConfigArrayCursor >= CurrentConfigArrayLimit) 
        {
            CurrentConfigArrayCursor = 0;
            ConfigCursor++;
        }
    }
}

void ConfigurationHandler1s(void)
{
    can_var.send.ST_EEPROM_newParameterSaved = 0;
    can_var.send.ST_EEPROM_newCalibrationSaved = 0;

    if(dcuVar.addNewParameterToEEPROM == 1)
    {
        SaveNewParameterToEEPROM();
        dcuVar.addNewParameterToEEPROM = 0;
    }
    
    if(dcuVar.addNewCalibrationToEEPROM == 1)
    {
        SaveNewCalibrationToEEPROM();
        dcuVar.addNewCalibrationToEEPROM = 0;
    }
    
    if(can_var.receive.Flag_config == 1)
    {
        if(FASTGEN_CONFIG_CC_EEPROM_clean_Parameter == 1)
        {
            FASTGEN_CONFIG_SET_CC_EEPROM_clean_Parameter(0);
            CleanAllParametersFromEEPROM();
        }
        else if(FASTGEN_CONFIG_CC_EEPROM_clean_Calibration == 1)
        {
            FASTGEN_CONFIG_SET_CC_EEPROM_clean_Calibration(0);
            CleanAllCalibrationsFromEEPROM();
        }
        else if(FASTGEN_CONFIG_CC_DCU_set_DCU_HW_revision > 0)
        {
            FASTGEN_CONFIG_SET_CI_DCU_HW_revision(((FASTGEN_CONFIG_CC_DCU_set_DCU_HW_revision) & 0x00FF));
            SaveAllInfosToEEPROM();
            FASTGEN_CONFIG_SET_CC_DCU_set_DCU_HW_revision(0);
        }
        else if(FASTGEN_CONFIG_CC_DCU_set_DCU_board_number > 0)
        {
            FASTGEN_CONFIG_SET_CI_DCU_board_number(((FASTGEN_CONFIG_CC_DCU_set_DCU_board_number) & 0x00FF));
            SaveAllInfosToEEPROM();
            FASTGEN_CONFIG_SET_CC_DCU_set_DCU_board_number(0);
        }
        else if(FASTGEN_CONFIG_CC_DCU_set_MB_HW_revision > 0)
        {
            FASTGEN_CONFIG_SET_CI_MB_HW_revision(((FASTGEN_CONFIG_CC_DCU_set_MB_HW_revision) & 0x00FF));
            SaveAllInfosToEEPROM();
            FASTGEN_CONFIG_SET_CC_DCU_set_MB_HW_revision(0);
        }
        else if(FASTGEN_CONFIG_CC_DCU_set_MB_board_number > 0)
        {
            FASTGEN_CONFIG_SET_CI_MB_board_number(((FASTGEN_CONFIG_CC_DCU_set_MB_board_number) & 0x00FF));
            SaveAllInfosToEEPROM();
            FASTGEN_CONFIG_SET_CC_DCU_set_MB_board_number(0);
        }
        else
        {            
            SaveNewParameterToEEPROM();
        }
        
        can_var.receive.Flag_config = 0;
    }
}