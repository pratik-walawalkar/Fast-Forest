/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-09-19
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_HWID_H
#define DCU_FF12_HWID_H

#include "DCU_FF12.h"

void        InitHWID               (void);
uint8       ReadHWID               (void);
result_t    SelfTestHWID           (void);

#endif //DCU_FF12_HWID_H
