/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_ADC_H
#define DCU_FF12_ADC_H

#include "DCU_FF12.h"

void InitADC        (void);
void InitDMA5ForADC (void);
void RunADCLowPass  (void);

#endif //DCU_FF12_ADC_H
