/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.2
 * @date 2020-06-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_Dummy_Application.h"
#include "DCU_Dummy_SocketCAN.h"
#include "FastGen_CAN_Var.h"
#include "FastGen_CAN_Functions.h"
#include "FastGen_Config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/can.h>



result_t Init()
{
    SCAN_Init();

    return R_SUCCESS;
}

result_t SelfTest()
{
    return R_SUCCESS;
}

void Application1ms(void)
{
    if(dcuVar.modelBoard.app_1ms_func != NULLPTR)
        dcuVar.modelBoard.app_1ms_func();
}

void Application10ms(void)
{
    can_var.send.ST_DCU_StatusLed = STATUS_OK;
    FASTGEN_Send_CAN2_Status();

    if(dcuVar.modelBoard.app_10ms_func != NULLPTR)
        dcuVar.modelBoard.app_10ms_func();
}

void Application100ms(void)
{
    FASTGEN_Send_CAN2_Error();

    // struct can_frame frame;
    // frame.can_id = 0x555;
    //  frame.can_dlc = 5;
    // sprintf(frame.data, "Hello");

    // SCAN_SendMsg(DCU_CAN1, &frame);

    if(dcuVar.modelBoard.app_100ms_func != NULLPTR)
        dcuVar.modelBoard.app_100ms_func();
}

void Application500ms(void)
{
    can_var.send.ST_DCU_StatusLedGreen = !can_var.send.ST_DCU_StatusLedGreen;
    can_var.send.ST_DCU_StatusLedRed = DCU_PIN_UNSET;
    can_var.send.FB_DCU_StatusLedGreen = can_var.send.ST_DCU_StatusLedGreen;
    can_var.send.FB_DCU_StatusLedRed = can_var.send.ST_DCU_StatusLedRed;

    FASTGEN_Send_CAN2_Warning();

    if(dcuVar.modelBoard.app_500ms_func != NULLPTR)
        dcuVar.modelBoard.app_500ms_func();
}

void Application1s(void)
{
    if(can_var.receive.Flag_config && FASTGEN_CONFIG_CC_EEPROM_clean_Parameter == 1)
    {
        FASTGEN_CONFIG_INIT_Parameter();
        can_var.receive.Flag_config = 0;
    }

    if(can_var.receive.Flag_config && FASTGEN_CONFIG_CC_EEPROM_clean_Calibration == 1)
    {
        FASTGEN_CONFIG_INIT_Calibration();
        can_var.receive.Flag_config = 0;
    }

    if(dcuVar.modelBoard.app_1s_func != NULLPTR)
        dcuVar.modelBoard.app_1s_func();
}

void ApplicationIdle(void)
{
    //ToDo: cross-platform usleep
}