/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.2
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */


#include "DCU.h"

#ifndef USE_X86_DUMMY
#include "DCU_FF12_Compiler.h"
#include "DCU_FF12.h"
#include "FastGen_CAN_Var.h"
#include "DCU_FF12_Configuration.h"


volatile dcu_var_t dcuVar = { };

void DCU_Run(MB_INIT_FUNC init_func,
        MB_SELFTEST_FUNC selftest_func,
        MB_APP_FUNC app_5us_func,
        MB_APP_FUNC app_1ms_func,
        MB_APP_FUNC app_10ms_func,
        MB_APP_FUNC app_100ms_func,
        MB_APP_FUNC app_500ms_func,
        MB_APP_FUNC app_1s_func,
        uint8 mb_major_version,
        uint8 mb_minor_version,
        uint8 mb_hardware_id)
{
    result_t result;
    uint8 i;

    dcuVar.modelBoard.initFunc = init_func;
    dcuVar.modelBoard.selftestFunc = selftest_func;
    dcuVar.modelBoard.app_5us_func = app_5us_func;
    dcuVar.modelBoard.app_1ms_func = app_1ms_func;
    dcuVar.modelBoard.app_10ms_func = app_10ms_func;
    dcuVar.modelBoard.app_100ms_func = app_100ms_func;
    dcuVar.modelBoard.app_500ms_func = app_500ms_func;
    dcuVar.modelBoard.app_1s_func = app_1s_func;
    dcuVar.modelBoard.mb_major_version = mb_major_version;
    dcuVar.modelBoard.mb_minor_version = mb_minor_version;
    dcuVar.modelBoard.mb_hardware_id = mb_hardware_id;
    dcuVar.modelBoard.dma_callback_func = NULLPTR;

    result = Init();
    if(result != R_SUCCESS)
    {
        Reset();
    }

    for(i = 0; i < 3; i++)
    {
        result = SelfTest();
        if(result == R_SUCCESS)
            break;
        can_var.send.EC_DCU_SelfTestFailed = 1;
    }
    if(result == R_FAILED) // continue on R_FAILED_SELFTEST_NON_CRITICAL
    {
        FaultLoop();
    }
    
    PostDCUSelfTest();

    result = dcuVar.modelBoard.initFunc();
    if(result != R_SUCCESS)
    {
        can_var.send.EC_MB_InitFailed = 1;
        FaultLoop();
    }
    
    PostMBInit();

    for(i = 0; i < 3; i++)
    {
        result = dcuVar.modelBoard.selftestFunc();
        if(result == R_SUCCESS)
            break;
        can_var.send.EC_MB_SelfTestFailed = 1;
    }
    if(result == R_FAILED) // continue on R_FAILED_SELFTEST_NON_CRITICAL
    {
        FaultLoop();
    }

    RunScheduler();
}

result_t DCU_InitDigitalInput(DCU_portpin portPin, DCU_pin_pull_config_t pullCfg)
{
    return InitDigitalInput(portPin, pullCfg);
}

result_t DCU_InitDigitalOutput(DCU_portpin portPin, DCU_pin_digital_value_t initialValue)
{
    return InitDigitalOutput(portPin, initialValue);
}

DCU_pin_digital_value_t DCU_DigitalGet(DCU_portpin portPin)
{
    return GetDigitalPin(portPin);
}

result_t DCU_DigitalSet(DCU_portpin portPin, DCU_pin_digital_value_t value)
{
    return SetDigitalPin(portPin, value);
}

result_t DCU_InitAnalogInput(DCU_portpin portPin, boolean useLowPass, DCU_analog_read_ref_t pVar)
{
    return InitAnalogInput(portPin, useLowPass, pVar);
}

result_t DCU_InitPWMOutput(DCU_portpin portPin, DCU_pwm_freq_t frequency, DCU_pwm_options options)
{
    //ToDo: waiting for finished hardware designs to know if needed and which pins / timers are allowed
    return R_SUCCESS;
}

result_t DCU_PWMSetDutyCylce(DCU_portpin portPin, DCU_pwm_dutycycle_t dutycycle)
{
    //ToDo: waiting for finished hardware designs to know if needed and which pins / timers are allowed
    return R_SUCCESS;
}

result_t DCU_InitAdditionalI2C(DCU_i2c_options options)
{
    return InitI2C1(options);
}

result_t DCU_I2CStart(DCU_i2c_t device)
{
    result_t result = R_FAILED_PARAMETER_ERROR;

    switch(device)
    {
        case DCU_I2C1:
            StartI2C1();
            result = R_SUCCESS;
            break;

        case DCU_I2C2:
            StartI2C2();
            result = R_SUCCESS;
            break;
    }

    return result;
}

result_t DCU_I2CStop(DCU_i2c_t device)
{
    result_t result = R_FAILED_PARAMETER_ERROR;

    switch(device)
    {
        case DCU_I2C1:
            StopI2C1();
            result = R_SUCCESS;
            break;

        case DCU_I2C2:
            StopI2C2();
            result = R_SUCCESS;
            break;
    }

    return result;
}

result_t DCU_I2CRestart(DCU_i2c_t device)
{
    result_t result = R_FAILED_PARAMETER_ERROR;

    switch(device)
    {
        case DCU_I2C1:
            RestartI2C1();
            result = R_SUCCESS;
            break;

        case DCU_I2C2:
            RestartI2C2();
            result = R_SUCCESS;
            break;
    }

    return result;
}

result_t DCU_I2CWrite(DCU_i2c_t device, uint8 data)
{
    result_t result = R_FAILED_PARAMETER_ERROR;

    switch(device)
    {
        case DCU_I2C1:
            WriteI2C1(data);
            result = R_SUCCESS;
            break;

        case DCU_I2C2:
            WriteI2C2(data);
            result = R_SUCCESS;
            break;
    }

    return result;
}

uint8 DCU_I2CRead(DCU_i2c_t device)
{
    uint8 result = 0;

    switch(device)
    {
        case DCU_I2C1:
            result = ReadI2C1();
            break;
        case DCU_I2C2:
            result = ReadI2C2();
            break;
    }

    return 0;
}

result_t DCU_I2CScanBus(DCU_i2c_t device, uint8 *addresses, uint8 maxCount)
{
    //ToDo: not needed atm
    return R_FAILED_NOT_IMPLEMENTED;
}

uint8 DCU_I2CGetAck(DCU_i2c_t device)
{
    uint8 result = 0;

    switch(device)
    {
        case DCU_I2C1:
            result = GetI2C1Ack();
            break;
        case DCU_I2C2:
            result = GetI2C2Ack();
            break;
    }

    return 0;
}

result_t DCU_I2CSendAck(DCU_i2c_t device)
{
    result_t result = R_FAILED_PARAMETER_ERROR;

    switch(device)
    {
        case DCU_I2C1:
            SendI2C1Ack();
            result = R_SUCCESS;
            break;

        case DCU_I2C2:
            SendI2C2Ack();
            result = R_SUCCESS;
            break;
    }

    return result;
}

result_t DCU_I2CSendNonAck(DCU_i2c_t device)
{
    result_t result = R_FAILED_PARAMETER_ERROR;

    switch(device)
    {
        case DCU_I2C1:
            SendI2C1NonAck();
            result = R_SUCCESS;
            break;

        case DCU_I2C2:
            SendI2C2NonAck();
            result = R_SUCCESS;
            break;
    }

    return result;
}

result_t DCU_I2CGetErrors(DCU_i2c_t device)
{
    //ToDo: not needed atm
    return R_FAILED_NOT_IMPLEMENTED;
}

result_t DCU_InitSPI1(DCU_spi_options options)
{
    return InitSPI1(options);
}

result_t DCU_InitSPI2(DCU_spi_options options)
{
    return InitSPI2(options);
}

uint16 DCU_SPIDoTransfer(DCU_spi_t device, uint16 data)
{
    uint16 result = 0;

    switch(device)
    {
        case DCU_SPI1:
            result = TransferSPI1(data);
            break;
        case DCU_SPI2:
            result = TransferSPI2(data);
            break;
    }

    return result;
}

result_t DCU_SPIWrite(DCU_spi_t device, uint16 data)
{
    result_t result = R_FAILED_PARAMETER_ERROR;
    
    switch(device)
    {
        case DCU_SPI1:
            TransferSPI1(data);            
            result = R_SUCCESS;
            break;
        case DCU_SPI2:
            TransferSPI2(data);        
            result = R_SUCCESS;
            break;
    }

    return result;
}

uint16 DCU_SPIGetRead(DCU_spi_t device)
{
    uint16 result = 0;
    
    switch(device)
    {
        case DCU_SPI1:            
            result = ReadSPI1();
            break;
        case DCU_SPI2:
            result = ReadSPI2();
            break;
    }

    return result;
}

result_t DCU_SetAnalogDMACallback(MB_CALLBACK_FUNC callback_func)
{
    result_t result = R_FAILED_NO_RESOURCES_LEFT;

    if(dcuVar.modelBoard.dma_callback_func == NULLPTR)
    {
        dcuVar.modelBoard.dma_callback_func = callback_func;
        result = R_SUCCESS;
    }

    return result;
}

#endif

