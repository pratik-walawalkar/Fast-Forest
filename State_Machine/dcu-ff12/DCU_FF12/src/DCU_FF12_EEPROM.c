/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_EEPROM.h"
#include "DCU_FF12_I2C.h"
#include "DCU_FF12_GPIO.h"
#include "FastGen_Config.h"
#include "FastGen_CAN_Var.h"
#include <p33EP512MU810.h>

#include"DCU_FF12_Timer.h"


//--------- EEPROM LAYOUT -----------
//
//-----------------------------------
//------- BLOCK 0 (512Kbit) ---------
//-----------------------------------
// Column0x00: DCU functions/tests/infos Byte0
// Column0x01: DCU functions/tests/infos Byte1
// Column0x02: DCU functions/tests/infos Byte2
// Column0x03: DCU functions/tests/infos Byte3
// Column0x04: (reserved)
// Column0x05: (reserved)
// Column0x06: (reserved)
// Column0x07: (reserved)
// Column0x08: (reserved)
// Column0x09: (reserved)
// Column0x0A: (reserved)
// Column0x0B: (reserved)
// Column0x0C: (reserved)
// Column0x0D: (reserved)
// Column0x0E: (reserved)
// Column0x0F: (reserved)
//
// Column0x10: Parameter 0x000-0x0FF Byte0 (currently used for 16bit)
// Column0x11: Parameter 0x000-0x0FF Byte1 (currently used for 16bit)
// Column0x12: Parameter 0x000-0x0FF Byte2 (reserved for 32bit)
// Column0x13: Parameter 0x000-0x0FF Byte3 (reserved for 32bit)
// Column0x14: Parameter 0x100-0x1FF Byte0 (currently used for 16bit)
// Column0x15: Parameter 0x100-0x1FF Byte1 (currently used for 16bit)
// Column0x16: Parameter 0x100-0x1FF Byte2 (reserved for 32bit)
// Column0x17: Parameter 0x100-0x1FF Byte3 (reserved for 32bit)
// Column0x18: Parameter 0x200-0x2FF Byte0 (currently used for 16bit)
// Column0x19: Parameter 0x200-0x2FF Byte1 (currently used for 16bit)
// Column0x1A: Parameter 0x200-0x2FF Byte2 (reserved for 32bit)
// Column0x1B: Parameter 0x200-0x2FF Byte3 (reserved for 32bit)
// Column0x1C: Parameter 0x300-0x3FF Byte0 (currently used for 16bit)
// Column0x1D: Parameter 0x300-0x3FF Byte1 (currently used for 16bit)
// Column0x1E: Parameter 0x300-0x3FF Byte2 (reserved for 32bit)
// Column0x1F: Parameter 0x300-0x3FF Byte3 (reserved for 32bit)
//
// Column0x20: Calibration 0x000-0x0FF Byte0 (currently used for 16bit)
// Column0x21: Calibration 0x000-0x0FF Byte1 (currently used for 16bit)
// Column0x22: Calibration 0x000-0x0FF Byte2 (reserved for 32bit)
// Column0x23: Calibration 0x000-0x0FF Byte3 (reserved for 32bit)
// Column0x24: Calibration 0x100-0x1FF Byte0 (currently used for 16bit)
// Column0x25: Calibration 0x100-0x1FF Byte1 (currently used for 16bit)
// Column0x26: Calibration 0x100-0x1FF Byte2 (reserved for 32bit)
// Column0x27: Calibration 0x100-0x1FF Byte3 (reserved for 32bit)
// Column0x28: Calibration 0x200-0x2FF Byte0 (currently used for 16bit)
// Column0x29: Calibration 0x200-0x2FF Byte1 (currently used for 16bit)
// Column0x2A: Calibration 0x200-0x2FF Byte2 (reserved for 32bit)
// Column0x2B: Calibration 0x200-0x2FF Byte3 (reserved for 32bit)
// Column0x2C: Calibration 0x300-0x3FF Byte0 (currently used for 16bit)
// Column0x2D: Calibration 0x300-0x3FF Byte1 (currently used for 16bit)
// Column0x2E: Calibration 0x300-0x3FF Byte2 (reserved for 32bit)
// Column0x2F: Calibration 0x300-0x3FF Byte3 (reserved for 32bit)
//
// Column0x30 - Column0xFF: NOT USED
//-----------------------------------
//
//-----------------------------------
//------- BLOCK 1 (512Kbit) ---------
//-----------------------------------
// NOT USED
//-----------------------------------


#define EEPROM_HW_ADDRESS (0xA0) //ToDo: second 512Kbit block b0 bit

#define EEPROM_DCU_FUNCTIONS_BYTE0 (0x00)
#define EEPROM_DCU_FUNCTIONS_BYTE1 (0x01)
#define EEPROM_DCU_FUNCTIONS_BYTE2 (0x02)
#define EEPROM_DCU_FUNCTIONS_BYTE3 (0x03)

#define EEPROM_DCU_FUNCTIONS_SELFTEST_ROW (0x10)
#define EEPROM_DCU_FUNCTIONS_SELFTEST_MAGIC_BYTE (0x78)

#define EEPROM_DCU_FUNCTIONS_MB_HWID_ROW (0x20)
#define EEPROM_DCU_FUNCTIONS_DCU_HW_revision_ROW (0x30)
#define EEPROM_DCU_FUNCTIONS_DCU_board_number_ROW (0x31)
#define EEPROM_DCU_FUNCTIONS_MB_HW_revision_ROW (0x32)
#define EEPROM_DCU_FUNCTIONS_MB_board_number_ROW (0x33)
#define EEPROM_DCU_FUNCTIONS_DCU_SW_major_version_ROW (0x34)
#define EEPROM_DCU_FUNCTIONS_DCU_SW_minor_version_ROW (0x35)
#define EEPROM_DCU_FUNCTIONS_MB_SW_major_version_ROW (0x36)
#define EEPROM_DCU_FUNCTIONS_MB_SW_minor_version_ROW (0x37)


uint16 EEPROM_Parameter  [FASTGEN_CONFIG_SIZE_PARAMETER_ARRAY];
uint16 EEPROM_Calibration[FASTGEN_CONFIG_SIZE_CALIBRATION_ARRAY];

DCU_portpin eepromWriteProtectPin = { DCU_PORT_D, 15 };

void InitEEPROM(void)
{
    InitDigitalOutput(eepromWriteProtectPin, DCU_PIN_SET);
}

result_t SelfTestEEPROMCommunication(void)
{
    result_t result = R_SUCCESS;

    uint8 data = ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_SELFTEST_ROW);
    if(data != EEPROM_DCU_FUNCTIONS_SELFTEST_MAGIC_BYTE)
    {
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_SELFTEST_ROW, EEPROM_DCU_FUNCTIONS_SELFTEST_MAGIC_BYTE);
        data = ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_SELFTEST_ROW);
        if(data != EEPROM_DCU_FUNCTIONS_SELFTEST_MAGIC_BYTE)
        {            
            result = R_FAILED;
        }
    }
    
    return result;
}

result_t SelfTestEEPROMHWID(void)
{
    result_t result = R_SUCCESS;

    uint8 eepromHWID = ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_HWID_ROW);
    if(eepromHWID != dcuVar.modelBoard.mb_hardware_id)
    {        
        result = R_FAILED;
    }
    
    return result;
}

void CheckForSWVersionChangesEEPROM(void)
{
    int16 major = 0;
    int16 minor = 0;
    
    major = (ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_SW_major_version_ROW) & 0x00FF);    
    major |= ((((uint16)ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_DCU_SW_major_version_ROW)) << 8) & 0xFF00);
    minor = (ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_SW_minor_version_ROW) & 0x00FF);    
    minor |= ((((uint16)ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_DCU_SW_minor_version_ROW)) << 8) & 0xFF00);
    if(major != FASTGEN_CONFIG_CI_DCU_SW_major_version ||
       minor != FASTGEN_CONFIG_CI_DCU_SW_minor_version)
    {
        can_var.send.EI_DCU_softwareVersionChanged = 1;
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_DCU_SW_major_version_ROW, (((FASTGEN_CONFIG_CI_DCU_SW_major_version) >> 8) & 0x00FF));
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_SW_major_version_ROW, (FASTGEN_CONFIG_CI_DCU_SW_major_version) & 0x00FF);
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_DCU_SW_minor_version_ROW, (((FASTGEN_CONFIG_CI_DCU_SW_minor_version) >> 8) & 0x00FF));
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_SW_minor_version_ROW, (FASTGEN_CONFIG_CI_DCU_SW_minor_version) & 0x00FF);
        
    }
    
    major = (ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_SW_major_version_ROW) & 0x00FF);    
    major |= ((((uint16)ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_MB_SW_major_version_ROW)) << 8) & 0xFF00);
    minor = (ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_SW_minor_version_ROW) & 0x00FF);    
    minor |= ((((uint16)ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_MB_SW_minor_version_ROW)) << 8) & 0xFF00);
    if(major != FASTGEN_CONFIG_CI_MB_SW_major_version ||
       minor != FASTGEN_CONFIG_CI_MB_SW_minor_version)
    {
        can_var.send.EI_MB_softwareVersionChanged = 1;
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_MB_SW_major_version_ROW, (((FASTGEN_CONFIG_CI_MB_SW_major_version) >> 8) & 0x00FF));
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_SW_major_version_ROW, (FASTGEN_CONFIG_CI_MB_SW_major_version) & 0x00FF);
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_MB_SW_minor_version_ROW, (((FASTGEN_CONFIG_CI_MB_SW_minor_version) >> 8) & 0x00FF));
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_SW_minor_version_ROW, (FASTGEN_CONFIG_CI_MB_SW_minor_version) & 0x00FF);
    }    
}

void GetAllInfosFromEEPROM(void)
{            
    FASTGEN_CONFIG_SET_CI_DCU_HW_revision((ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_HW_revision_ROW) & 0x00FF));
    FASTGEN_CONFIG_SET_CI_DCU_board_number((ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_board_number_ROW) & 0x00FF));
    FASTGEN_CONFIG_SET_CI_MB_HW_revision((ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_HW_revision_ROW) & 0x00FF));
    FASTGEN_CONFIG_SET_CI_MB_board_number((ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_board_number_ROW) & 0x00FF));
}

void SaveAllInfosToEEPROM(void)
{    
    if (FASTGEN_CONFIG_CI_DCU_HW_revision != (ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_HW_revision_ROW) & 0x00FF))
    {
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_HW_revision_ROW, (FASTGEN_CONFIG_CI_DCU_HW_revision) & 0x00FF);
    }
    if (FASTGEN_CONFIG_CI_DCU_board_number != (ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_board_number_ROW) & 0x00FF))
    {
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_DCU_board_number_ROW, (FASTGEN_CONFIG_CI_DCU_board_number) & 0x00FF);
    }
    if (FASTGEN_CONFIG_CI_MB_HW_revision != (ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_HW_revision_ROW) & 0x00FF))
    {
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_HW_revision_ROW, (FASTGEN_CONFIG_CI_MB_HW_revision) & 0x00FF);
    }
    if (FASTGEN_CONFIG_CI_MB_board_number != (ReadEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_board_number_ROW) & 0x00FF))
    {
        WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_board_number_ROW, (FASTGEN_CONFIG_CI_MB_board_number) & 0x00FF);
    }
    
    can_var.send.ST_EEPROM_newParameterSaved = 1;
}

void CleanAllInfosFromEEPROM(void)
{    
    WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_HWID_ROW, dcuVar.modelBoard.mb_hardware_id);
    // DCU infos will not be cleared
    WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_HW_revision_ROW, 0);
    WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_board_number_ROW, 0);    
    // DCU SW version will not be cleared    
    WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_SW_major_version_ROW, 0);
    WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_MB_SW_major_version_ROW, 0);
    WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE0, EEPROM_DCU_FUNCTIONS_MB_SW_minor_version_ROW, 0);
    WriteEEPROM(EEPROM_DCU_FUNCTIONS_BYTE1, EEPROM_DCU_FUNCTIONS_MB_SW_minor_version_ROW, 0);
}

void GetAllParametersFromEEPROM(void)
{
    uint16 Counter       = 0;
    uint16 check_Counter = 0;
   
    for (Counter = 0; Counter < FASTGEN_CONFIG_SIZE_PARAMETER_ARRAY; Counter++)
    {    
        ReadParameterFromEEPROM(Counter);
        if ((Parameter[Counter] == 0xFF) || (Parameter[Counter] == 0x00))
        {
            check_Counter++;
        }
    }
    if (check_Counter >= FASTGEN_CONFIG_SIZE_PARAMETER_ARRAY)
    {
        CleanAllParametersFromEEPROM();
    }   
}

void GetAllCalibrationsFromEEPROM(void)
{
    uint16 Counter = 0;
    
    for (Counter = 0; Counter < FASTGEN_CONFIG_SIZE_CALIBRATION_ARRAY; Counter++)
    {
        ReadCalibrationFromEEPROM(Counter);
    }
}

void CleanAllParametersFromEEPROM(void)
{
    uint16 Counter = 0;
    FASTGEN_CONFIG_INIT_Parameter();

    for (Counter = 0; Counter < FASTGEN_CONFIG_SIZE_PARAMETER_ARRAY; Counter++)
    {
       WriteParameterToEEPROM(Counter);   
    }
    
    can_var.send.ST_EEPROM_newParameterSaved = 1;
}

void CleanAllCalibrationsFromEEPROM(void)
{
    uint16 Counter = 0;
    FASTGEN_CONFIG_INIT_Calibration();

    for (Counter = 0; Counter < FASTGEN_CONFIG_SIZE_CALIBRATION_ARRAY; Counter++)
    {
        WriteCalibrationToEEPROM(Counter);
    }
    
    can_var.send.ST_EEPROM_newCalibrationSaved = 1;
}

void SaveNewParameterToEEPROM(void)
{
    uint16 Counter = 0;

    for (Counter = 0; Counter < FASTGEN_CONFIG_SIZE_PARAMETER_ARRAY; Counter++)
    {
        if (Parameter[Counter] != EEPROM_Parameter[Counter])
        {
            WriteParameterToEEPROM(Counter);
            can_var.send.ST_EEPROM_newParameterSaved = 1;
        }
    }
}

void SaveNewCalibrationToEEPROM(void)
{
    uint16 Counter = 0;

    for (Counter = 0; Counter < FASTGEN_CONFIG_SIZE_CALIBRATION_ARRAY; Counter++)
    {
        if (Calibration[Counter] != EEPROM_Calibration[Counter])
        {
            WriteCalibrationToEEPROM(Counter);
            can_var.send.ST_EEPROM_newCalibrationSaved = 1;
        }
    }
}

void WriteParameterToEEPROM(uint16 parameterId)
{    
    uint8  column  = 0;
    uint8  row     = 0;
    
    if(parameterId >= FASTGEN_CONFIG_SIZE_PARAMETER_ARRAY)
        return;
    
    column = parameterId / 0x100;
    row = parameterId % 0xFF;
    WriteEEPROM(0x10 + column + 0x01, row, ((Parameter[parameterId] >> 8) & 0x00FF));
    WriteEEPROM(0x10 + column + 0x00, row, (Parameter[parameterId] & 0x00FF));
    
    EEPROM_Parameter[parameterId] = Parameter[parameterId];    
}

void WriteCalibrationToEEPROM(uint16 calibrationId)
{    
    uint8  column  = 0;
    uint8  row     = 0;
    
    if(calibrationId >= FASTGEN_CONFIG_SIZE_CALIBRATION_ARRAY)
        return;
    
    column = calibrationId / 0x100;
    row = calibrationId % 0xFF;
    WriteEEPROM(0x20 + column + 0x01, row, ((Calibration[calibrationId] >> 8) & 0x00FF));
    WriteEEPROM(0x20 + column + 0x00, row, (Calibration[calibrationId] & 0x00FF));
    
    EEPROM_Calibration[calibrationId] = Calibration[calibrationId];    
}

void ReadParameterFromEEPROM(uint16 parameterId)
{    
    uint8  column  = 0;
    uint8  row     = 0;
    
    if(parameterId >= FASTGEN_CONFIG_SIZE_PARAMETER_ARRAY)
        return;
    
    column = parameterId / 0x100;
    row = parameterId % 0xFF;
    EEPROM_Parameter[parameterId]  =           (ReadEEPROM(0x10 + column + 0x00, row)        & 0x00FF);
    EEPROM_Parameter[parameterId] |= ((((uint16)ReadEEPROM(0x10 + column + 0x01, row)) << 8) & 0xFF00);

    Parameter[parameterId] = EEPROM_Parameter[parameterId];  
}

void ReadCalibrationFromEEPROM(uint16 calibrationId)
{    
    uint8  column  = 0;
    uint8  row     = 0;
    
    if(calibrationId >= FASTGEN_CONFIG_SIZE_CALIBRATION_ARRAY)
        return;
    
    column = calibrationId / 0x100;
    row = calibrationId % 0xFF;
    EEPROM_Calibration[calibrationId]  =           (ReadEEPROM(0x20 + column + 0x00, row)        & 0x00FF);
    EEPROM_Calibration[calibrationId] |= ((((uint16)ReadEEPROM(0x20 + column + 0x01, row)) << 8) & 0xFF00);

    Calibration[calibrationId] = EEPROM_Calibration[calibrationId];  
}

void WriteEEPROM(uint8 highAdd, uint8 lowAdd, uint8 data)
{
    SetDigitalPin(eepromWriteProtectPin, DCU_PIN_UNSET);
    Wait_ms(1);

    WaitForI2C2Idle();              //Ensure Module is Idle
    StartI2C2();                    //Generate start condition
    WriteI2C2(EEPROM_HW_ADDRESS);	//Write Control byte
    WaitForI2C2Idle();

    WriteI2C2(highAdd);              //Write High Address
    WaitForI2C2Idle();  
    WriteI2C2(lowAdd);              //Write Low Address
    WaitForI2C2Idle();

    WriteI2C2(data);                 //Write Data
    WaitForI2C2Idle();
    StopI2C2();                      //Initiate Stop Condition        

    SetDigitalPin(eepromWriteProtectPin, DCU_PIN_SET);
    Wait_ms(5);
}

uint8 ReadEEPROM(uint8 highAdd, uint8 lowAdd)
{
    uint8 data = 0;

    WaitForI2C2Idle();               //Wait for bus Idle
    StartI2C2();                     //Generate Start condition
    WriteI2C2(EEPROM_HW_ADDRESS);    //send control byte for write; ControlByte is the address of the slave
    WaitForI2C2Idle();               //Wait for bus Idle
    
    WriteI2C2(highAdd);              //Send High Address
    WaitForI2C2Idle();               //Wait for bus Idle
    WriteI2C2(lowAdd);               //Send Low Address
    WaitForI2C2Idle();               //Wait for bus Idle
    
    RestartI2C2();                   //Generate Restart
    WriteI2C2(EEPROM_HW_ADDRESS | 0x01);   //send control byte for Read
    WaitForI2C2Idle();               //Wait for bus Idle
    data = ReadI2C2();               //Read Length number of bytes to Data    
    SendI2C2NonAck();                //send Not Ack
    StopI2C2();                      //Send Stop Condition

    return data;
}