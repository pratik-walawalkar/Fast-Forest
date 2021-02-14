/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-09-19
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_HWID.h"
#include "DCU_FF12_GPIO.h"


DCU_portpin hwIdPin0 = { DCU_PORT_G, 12 };
DCU_portpin hwIdPin1 = { DCU_PORT_G, 13 };
DCU_portpin hwIdPin2 = { DCU_PORT_G, 14 };
DCU_portpin hwIdPin3 = { DCU_PORT_G, 15 };


void InitHWID(void)
{
    DCU_InitDigitalInput(hwIdPin0, DCU_NO_PULLUP_DOWN);
    DCU_InitDigitalInput(hwIdPin1, DCU_NO_PULLUP_DOWN);
    DCU_InitDigitalInput(hwIdPin2, DCU_NO_PULLUP_DOWN);
    DCU_InitDigitalInput(hwIdPin3, DCU_NO_PULLUP_DOWN);
}

uint8 ReadHWID(void)
{
    uint8 id = 0;
    id |= DCU_DigitalGet(hwIdPin0);
    id |= DCU_DigitalGet(hwIdPin1) << 1;
    id |= DCU_DigitalGet(hwIdPin2) << 2;
    id |= DCU_DigitalGet(hwIdPin3) << 3;
    
    return id;
}


result_t SelfTestHWID(void)
{
    result_t result = R_SUCCESS;

    uint8 id = ReadHWID();
    if(id != dcuVar.modelBoard.mb_hardware_id)
    {
        result = R_FAILED;
    }
    
    return result;
}
