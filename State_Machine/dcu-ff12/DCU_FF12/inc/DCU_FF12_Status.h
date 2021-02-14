/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_STATUS_H
#define DCU_FF12_STATUS_H

#include "DCU_FF12.h"

void        InitStatusAndLEDs          (void);
void        CheckStatusAndUpdateLEDs   (void);
void        CheckStatusLedFeedbacks    (void);
result_t    SelfTestStatusLEDs          (void);
void        SetFaultStatus               (void);

#endif //DCU_FF12_STATUS_H
