/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_TYPEDEFS_H
#define DCU_TYPEDEFS_H

#include <Global_Types.h>


typedef result_t (*MB_CALLBACK_FUNC)(void);
typedef result_t (*MB_INIT_FUNC)(void);
typedef result_t (*MB_SELFTEST_FUNC)(void);
typedef void (*MB_APP_FUNC)(void);

typedef uint8                   DCU_port_t;
#define DCU_PORT_A              0
#define DCU_PORT_B              1
#define DCU_PORT_C              2
#define DCU_PORT_D              3
#define DCU_PORT_E              4
#define DCU_PORT_F              5
#define DCU_PORT_G              6

typedef uint8                   DCU_pin_t;
typedef struct
{
    DCU_port_t port;
    DCU_pin_t pin;
} DCU_portpin;

typedef uint8                   DCU_pin_pull_config_t;
#define DCU_NO_PULLUP_DOWN      0
#define DCU_PULLUP              1
#define DCU_PULLDOWN            2

typedef uint8                   DCU_pin_digital_value_t;
#define DCU_PIN_UNSET           0
#define DCU_PIN_SET             1

typedef uint16                  DCU_pwm_freq_t;
typedef uint16                  DCU_pwm_dutycycle_t;

typedef uint16                  DCU_pwm_options;
#define DCU_PWM_OPTIONS_NONE    0

typedef uint16*                 DCU_analog_read_ref_t;

typedef uint8                   DCU_can_signal_error_t;
typedef uint8                   DCU_can_signal_feedback_digital_t;
typedef uint16                  DCU_can_signal_feedback_analog_t;
typedef uint16                  DCU_can_signal_parameter_t;

//TODO:
//typedef uint16                  CAN_SIGNAL_VALUE_UINT8
//typedef uint16                  CAN_SIGNAL_VALUE_UINT16
//typedef uint16                  CAN_SIGNAL_VALUE_UINT32

typedef uint16                                      DCU_i2c_options;
#define DCU_I2C_OPTIONS_NONE                        0
#define DCU_I2C_OPTIONS_USE_SLEW_RATE_CONTROL       1
#define DCU_I2C_OPTIONS_USE_ALTERNATIVE_PINS_1      2

typedef uint8                                       DCU_i2c_t;
#define DCU_I2C1                                    1
#define DCU_I2C2                                    2

typedef uint16                                      DCU_spi_options;
#define DCU_SPI_OPTIONS_NONE                        0
#define DCU_SPI_OPTIONS_USE_ALTERNATIVE_PINS_1      1

typedef uint8                                       DCU_spi_t;
#define DCU_SPI1                                    1
#define DCU_SPI2                                    2

typedef uint8                                       DCU_can_t;
#define DCU_CAN1                                    1
#define DCU_CAN2                                    2
#define DCU_CAN3                                    3
#define DCU_CAN4                                    4

typedef struct
{
    MB_INIT_FUNC initFunc;
    MB_SELFTEST_FUNC selftestFunc;
    MB_APP_FUNC app_5us_func;
    MB_APP_FUNC app_1ms_func;
    MB_APP_FUNC app_10ms_func;
    MB_APP_FUNC app_100ms_func;
    MB_APP_FUNC app_500ms_func;
    MB_APP_FUNC app_1s_func;
    uint8 mb_major_version;
    uint8 mb_minor_version;
    uint8 mb_hardware_id;
    MB_CALLBACK_FUNC dma_callback_func;
} modelBoard_t;

typedef struct
{
    modelBoard_t modelBoard;
    unsigned char flag_1ms;
    unsigned char status;
    boolean addNewParameterToEEPROM;
    boolean addNewCalibrationToEEPROM;
} dcu_var_t;

typedef struct
{
    DCU_portpin portPin;
    boolean useLowPass;
    DCU_analog_read_ref_t pVariable;
} analogFbConfig_t;

#endif //DCU_TYPEDEFS_H
