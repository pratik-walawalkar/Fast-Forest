/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_GPIO_H
#define DCU_FF12_GPIO_H

#include "DCU_FF12.h"

void                        InitPinsAsDefault                  (void);
void                        InitPPS                             (void);
result_t                    InitDigitalInput                   (DCU_portpin portPin,DCU_pin_pull_config_t pullCfg);
result_t                    InitDigitalOutput                  (DCU_portpin portPin, DCU_pin_digital_value_t initialValue);
result_t                    InitAnalogInput                    (DCU_portpin portPin, boolean useLowPass, DCU_analog_read_ref_t pVar);
result_t                    SetDigitalPin                      (DCU_portpin portPin,DCU_pin_digital_value_t value);
DCU_pin_digital_value_t     GetDigitalPin                      (DCU_portpin portPin);
uint8                       GetAnalogPinIndexForPin           (DCU_portpin portPin);
uint16                      GetMaskForDefinedAnalogInputsLow (void);
uint16                      GetMaskForDefinedAnalogInputsHigh(void);
uint8                       GetActiveAnalogInputsCount       (void);
void                        CreateAdcBufferToAnalogFbIndexMap   (void);
void                        SetAnalogFbValue                   (int index, uint16 value);
boolean                     GetAnalogFbValueUsesLowPass       (int index);
#endif //DCU_FF12_GPIO_H
