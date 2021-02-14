/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.2
 * @date 2020-05-19
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_DUMMY_SOCKETCAN_H
#define DCU_DUMMY_SOCKETCAN_H

#include <Global_Types.h>
#include "DCU_TypeDefs.h"
#include <linux/can.h>

extern result_t SCAN_Init(void);

extern result_t SCAN_SendMsg(DCU_can_t canInterface, struct can_frame *msg);

#endif //DCU_DUMMY_SOCKETCAN_H
