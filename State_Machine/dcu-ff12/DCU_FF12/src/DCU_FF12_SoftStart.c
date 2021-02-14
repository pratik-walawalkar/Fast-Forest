/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-09-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_SoftStart.h"
#include "DCU_FF12_GPIO.h"


DCU_portpin softStartPin = { DCU_PORT_F, 8 };


void InitAndActivateSoftStartOverride(void)
{
    DCU_InitDigitalOutput(softStartPin, DCU_PIN_SET);
}

void DeactivateSoftStartOverride(void)
{
    DCU_DigitalSet(softStartPin, DCU_PIN_UNSET);
}
