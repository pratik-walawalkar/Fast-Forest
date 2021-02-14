/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-09-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_VOLTAGESENSING_H
#define DCU_FF12_VOLTAGESENSING_H

#include "DCU_FF12.h"

void InitDCUVoltageSensing     (void);
void CheckDCUVoltages          (void);
void DoDCUVoltagesCalibration (void);

#endif //DCU_FF12_VOLTAGESENSING_H
