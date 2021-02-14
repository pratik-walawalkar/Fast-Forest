/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU.h"

#ifdef USE_X86_DUMMY
#include <stdio.h>
#include "DCU_x86_Dummy.h"
#include "DCU_Dummy_Scheduler.h"
#include "DCU_Dummy_Application.h"
#include "DCU_Dummy_SocketCAN.h"

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


            dcuVar.modelBoard.initFunc = init_func;
            dcuVar.modelBoard.selftestFunc = selftest_func;
            dcuVar.modelBoard.app_5us_func = app_5us_func;
            dcuVar.modelBoard.app_10ms_func = app_10ms_func;
            dcuVar.modelBoard.app_100ms_func = app_100ms_func;
            dcuVar.modelBoard.app_500ms_func = app_500ms_func;
            dcuVar.modelBoard.app_1s_func = app_1s_func;
            dcuVar.modelBoard.mb_major_version = mb_major_version;
            dcuVar.modelBoard.mb_minor_version = mb_minor_version;
            dcuVar.modelBoard.mb_hardware_id = mb_hardware_id;

            printf("Using DCU x86 Dummy (DCU Version: %i.%i, MB Version: %i.%i)\n",
                    DCU_MAJOR_VERSION, DCU_MINOR_VERSION, dcuVar.modelBoard.mb_major_version, dcuVar.modelBoard.mb_minor_version);

            result = Init();
            if(result != R_SUCCESS)
            {
                printf("DCU Init failed !!!\n");
                return;
            }
            printf("DCU Init OK\n");

            result = dcuVar.modelBoard.initFunc();
            if(result != R_SUCCESS)
            {
                printf("MB Init failed !!!\n");
                return;
            }
            printf("MB Init OK\n");

            result = SelfTest();
            if(result != R_SUCCESS)
            {
                printf("DCU SelfTest failed !!!\n");
                return;
            }
            printf("DCU SelfTest passed\n");

            result = dcuVar.modelBoard.selftestFunc();
            if(result != R_SUCCESS)
            {
                printf("MB SelfTest failed !!!\n");
                return;
            }
            printf("MB SelfTest passed\n");

            RunDummyInterruptHandler();
}


result_t DCU_InitDigitalInput(DCU_portpin portPin, DCU_pin_pull_config_t pullCfg) {
    return R_SUCCESS;
}

result_t DCU_InitDigitalOutput(DCU_portpin portPin, DCU_pin_digital_value_t initialValue) {
    return R_SUCCESS;
}

DCU_pin_digital_value_t DCU_DigitalGet(DCU_portpin portPin) {
    return R_SUCCESS;
}

result_t DCU_DigitalSet(DCU_portpin portPin, DCU_pin_digital_value_t value) {
    return R_SUCCESS;
}

result_t DCU_InitAnalogInput(DCU_portpin portPin, boolean useLowPass, DCU_analog_read_ref_t pVar) {
    return R_SUCCESS;
}

result_t DCU_InitPWMOutput(DCU_portpin portPin, DCU_pwm_freq_t frequency, DCU_pwm_options options) {
    return R_SUCCESS;
}

result_t DCU_PWMSetDutyCylce(DCU_portpin portPin, DCU_pwm_dutycycle_t dutycycle) {
    return R_SUCCESS;
}

result_t DCU_InitAdditionalI2C(DCU_i2c_options options) {
    return R_SUCCESS;
}

result_t DCU_I2CStart(DCU_i2c_t device) {
    return R_SUCCESS;
}

result_t DCU_I2CStop(DCU_i2c_t device) {
    return R_SUCCESS;
}

result_t DCU_I2CRestart(DCU_i2c_t device) {
    return R_SUCCESS;
}

result_t DCU_I2CWrite(DCU_i2c_t device, uint8 data) {
    return R_SUCCESS;
}

uint8 DCU_I2CRead(DCU_i2c_t device) {
    return R_SUCCESS;
}

result_t DCU_I2CScanBus(DCU_i2c_t device, uint8 *addresses, uint8 maxCount) {
    return R_SUCCESS;
}

uint8 DCU_I2CGetAck(DCU_i2c_t device) {
    return 1;
}

result_t DCU_I2CSendAck(DCU_i2c_t device) {
    return R_SUCCESS;
}

result_t DCU_I2CSendNonAck(DCU_i2c_t device) {
    return R_SUCCESS;
}

result_t DCU_I2CGetErrors(DCU_i2c_t device) {
    return R_SUCCESS;
}

result_t DCU_InitSPI1(DCU_spi_options options) {
    return R_SUCCESS;
}

result_t DCU_InitSPI2(DCU_spi_options options) {
    return R_SUCCESS;
}

uint16 DCU_SPIDoTransfer(DCU_spi_t device, uint16 data) {
    return R_SUCCESS;
}

result_t DCU_SPIWrite(DCU_spi_t device, uint16 data) {
    return R_SUCCESS;
}

result_t DCU_SPIGetRead(DCU_spi_t device) {
    return R_SUCCESS;
}

#endif