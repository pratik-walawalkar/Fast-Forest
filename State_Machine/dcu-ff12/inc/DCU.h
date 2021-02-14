/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief 
 * @version 0.5
 * @date 2019-11-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef DCU_H
#define DCU_H

#include <Global_Types.h>
#include "DCU_TypeDefs.h"
#include "DCU_cfg.h"

#define DCU_MAJOR_VERSION 0
#define DCU_MINOR_VERSION 4

/**
 * @brief Entry point for DCU
 * 
 * @param init_func the MB_Init function pointer
 * @param selftest_func the MB_Init selftest pointer
 * @param app_5us_func the MB_Application5us function pointer (if == nullptr timer2 will not be used)
 * @param app_10ms_func the MB_Application10ms function pointer 
 * @param app_100ms_func the MB_Application100ms function pointer 
 * @param app_500ms_func the MB_Application500ms function pointer 
 * @param app_1s_func the MB_Application1s function pointer 
 * @param mb_major_version the MB major version number
 * @param mb_minor_version the MB minor version number
 */
extern void DCU_Run(MB_INIT_FUNC init_func,
                MB_SELFTEST_FUNC selftest_func,
                MB_APP_FUNC app_5us_func,
                MB_APP_FUNC app_1ms_func,
                MB_APP_FUNC app_10ms_func,
                MB_APP_FUNC app_100ms_func,
                MB_APP_FUNC app_500ms_func,
                MB_APP_FUNC app_1s_func,
                uint8 mb_major_version,
                uint8 mb_minor_version,
                uint8 mb_hardware_id);


/**
 * @brief Setups a digital input
 * 
 * @param portPin µC port and pin
 * @param pullCfg configures used pullup or pulldown
 * @return result_t 
 */
extern result_t DCU_InitDigitalInput(DCU_portpin portPin,
                                DCU_pin_pull_config_t pullCfg);


/**
 * @brief Setups a digital output
 * 
 * @param portPin µC port and pin
 * @param initialValue the initial pin value (DCU_PIN_SET or DCU_PIN_UNSET)
 * @return result_t 
 */
extern result_t DCU_InitDigitalOutput(DCU_portpin portPin,
                                DCU_pin_digital_value_t initialValue);


/**
 * @brief reads the value of a digital pin
 * 
 * @param portPin µC port and pin
 * @return DCU_pin_digital_value_t current pin value
 */
extern DCU_pin_digital_value_t DCU_DigitalGet(DCU_portpin portPin);


/**
 * @brief Sets or unset a digital pin
 * 
 * @param portPin µC port and pin
 * @param value DCU_PIN_SET or DCU_PIN_UNSET
 * @return result_t 
 */
extern result_t DCU_DigitalSet(DCU_portpin portPin,
                        DCU_pin_digital_value_t value);


/**
 * @brief Setups an analog input
 * 
 * @param portPin µC port and pin
 * @param useLowPass if global low pass filter should be used
 * @param pVar the variable into which every new analog value should be written
 * @return result_t 
 */
extern result_t DCU_InitAnalogInput(DCU_portpin portPin,
                                boolean useLowPass,
                                DCU_analog_read_ref_t pVar);


/**
 * @brief Setups a callback for the ADC DMA interrupt, used for e.g. external adc multiplexer
 *
 * @param callback_func the callback function
 * @return result_t
 */
extern result_t DCU_SetAnalogDMACallback(MB_CALLBACK_FUNC callback_func);


/**
 * @brief Setups a pwm output
 * 
 * @param portPin µC port and pin
 * @param frequency the PWM frequency
 * @param options additional option flags
 * @return result_t 
 */
extern result_t DCU_InitPWMOutput(DCU_portpin portPin,
                            DCU_pwm_freq_t frequency,
                            DCU_pwm_options options);


/**
 * @brief Changes the dutycycle of a pwm output
 * 
 * @param portPin µC port and pin
 * @param dutycyle the new dutycycle
 * @return result_t 
 */
extern result_t DCU_PWMSetDutyCylce(DCU_portpin portPin,
                                DCU_pwm_dutycycle_t dutycycle); 


/**
 * @brief Inits the I2C not used for eeprom
 * 
 * @param options Flags used to set slew rate control, frequency and alternative pins
 * @return result_t 
 */
extern result_t DCU_InitAdditionalI2C(DCU_i2c_options options);


/**
 * @brief Starts a I2C transfer
 * 
 * @param device I2C1 or I2C2
 * @return result_t 
 */
extern result_t DCU_I2CStart(DCU_i2c_t device);


/**
 * @brief Stops a I2C transfer
 * 
 * @param device I2C1 or I2C2
 * @return result_t 
 */
extern result_t DCU_I2CStop(DCU_i2c_t device);


/**
 * @brief Restarts a I2C bus
 *
 * @param device I2C1 or I2C2
 * @return result_t
 */
extern result_t DCU_I2CRestart(DCU_i2c_t device);


/**
 * @brief Writes a byte to the I2C bus
 * 
 * @param device I2C1 or I2C2
 * @param data one byte of data
 * @return result_t 
 */
extern result_t DCU_I2CWrite(DCU_i2c_t device,
                        uint8 data);


/**
 * @brief Read a byte from the I2C bus
 * 
 * @param device I2C1 or I2C2
 * @return the read byte 
 */
extern uint8 DCU_I2CRead(DCU_i2c_t device);


/**
 * @brief Scans the I2C bus
 * 
 * @param device I2C1 or I2C2
 * @param addresses a buffer where the found addresses will be written to
 * @param maxCount buffer len
 * @return result_t 
 */
extern result_t DCU_I2CScanBus(DCU_i2c_t device,
                        uint8 *addresses, 
                        uint8 maxCount);


/**
 * @brief Returns the ACK
 *
 * @param device I2C1 or I2C2
 * @return result_t
 */
extern uint8 DCU_I2CGetAck(DCU_i2c_t device);


/**
 * @brief Sends an ACK to the I2C bus
 *
 * @param device I2C1 or I2C2
 * @return result_t
 */
extern result_t DCU_I2CSendAck(DCU_i2c_t device);


/**
 * @brief Sends an NonACK to the I2C bus
 *
 * @param device I2C1 or I2C2
 * @return result_t
 */
extern result_t DCU_I2CSendNonAck(DCU_i2c_t device);


/**
 * @brief Returns any I2C errors
 * 
 * @param device I2C1 or I2C2
 * @return result_t 
 */
extern result_t DCU_I2CGetErrors(DCU_i2c_t device);


/**
 * @brief Inits the SPI bus 1
 * 
 * @param options Flags used to set additional parameter, frequency and alternative pins
 * @return result_t 
 */
extern result_t DCU_InitSPI1(DCU_spi_options options);


/**
 * @brief Inits the SPI bus 2
 * 
 * @param options Flags used to set additional parameter, frequency and alternative pins
 * @return result_t 
 */
extern result_t DCU_InitSPI2(DCU_spi_options options);


/**
 * @brief Writes data to a SPI slave and returns the received answer
 * 
 * @param device SPI1 or SPI2
 * @param data the data to be send
 * @return the received data
 */
extern uint16 DCU_SPIDoTransfer(DCU_spi_t device,
                            uint16 data);


/**
 * @brief Just writes to the SPI bus
 * 
 * @param device SPI1 or SPI2 
 * @param data the data to be send 
 * @return result_t 
 */
extern result_t DCU_SPIWrite(DCU_spi_t device,
                        uint16 data);


/**
 * @brief Returns the content of the SPI input register
 * 
 * @param device SPI1 or SPI2 
 * @return uint16 
 */
extern uint16 DCU_SPIGetRead(DCU_spi_t device);


#endif // DCU_H