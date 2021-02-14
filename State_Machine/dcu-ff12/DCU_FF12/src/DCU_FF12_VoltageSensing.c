/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-09-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_VoltageSensing.h"
#include "FastGen_CAN_Functions.h"
#include "FastGen_CAN_Var.h"
#include "FastGen_Config.h"


DCU_portpin analogInput24VFB = { DCU_PORT_A, 7 };
uint16 rawInput24VFB = 0;
uint16 calcInput24VFB = 0;
float vcc24VFactor;

void InitDCUVoltageSensing(void)
{
    vcc24VFactor = ((((float32)FASTGEN_CONFIG_CV_DCU_Vcc24V_calibration) / 24) * 0.05);
    DCU_InitAnalogInput(analogInput24VFB, TRUE, &rawInput24VFB);
}

void CheckDCUVoltages(void)
{
    if(FASTGEN_CONFIG_CP_DCU_Vcc24V == 1)
    {
        calcInput24VFB = (uint16)(rawInput24VFB / vcc24VFactor);
        can_var.send.FB_DCU_Vcc24V = (uint8)(calcInput24VFB - 360);
        
        if (calcInput24VFB * 50 < FASTGEN_CONFIG_CP_DCU_Vcc24V_LowerLimit)
        {
            can_var.send.EW_DCU_Vcc24V = 1;
        }
        else if (calcInput24VFB * 50 > FASTGEN_CONFIG_CP_DCU_Vcc24V_UpperLimit)
        {
            can_var.send.EW_DCU_Vcc24V = 1;
        }
        else
        {
            can_var.send.EW_DCU_Vcc24V = 0;
        }
    }
    else
    {
        can_var.send.FB_DCU_Vcc24V = 0;
        can_var.send.EW_DCU_Vcc24V = 0;
    }
}

void DoDCUVoltagesCalibration(void)
{
    if (FASTGEN_CONFIG_CC_DCU_Vcc24V_calibration == 1)
    {
        FASTGEN_CONFIG_SET_CV_DCU_Vcc24V_calibration(rawInput24VFB);

        dcuVar.addNewCalibrationToEEPROM = 1;
        FASTGEN_CONFIG_SET_CC_DCU_Vcc24V_calibration(0);
    }
}
