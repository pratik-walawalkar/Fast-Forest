/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-09-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_CAN.h"
#include "FastGen_CAN_Functions.h"
#include "FastGen_CAN_Var.h"
#include "FastGen_Config.h"

void InitCAN(void)
{
    FASTGEN_INIT_CAN1();
    FASTGEN_INIT_CAN2();
    
    FASTGEN_CONFIG_INIT_Command();
    FASTGEN_CONFIG_INIT_Information();
    
    FASTGEN_CONFIG_SET_CI_MB_HWID(dcuVar.modelBoard.mb_hardware_id);
    FASTGEN_CONFIG_SET_CI_DCU_SW_major_version(DCU_MAJOR_VERSION);
    FASTGEN_CONFIG_SET_CI_DCU_SW_minor_version(DCU_MINOR_VERSION);
    FASTGEN_CONFIG_SET_CI_MB_SW_major_version(dcuVar.modelBoard.mb_major_version);
    FASTGEN_CONFIG_SET_CI_MB_SW_minor_version(dcuVar.modelBoard.mb_minor_version);
}