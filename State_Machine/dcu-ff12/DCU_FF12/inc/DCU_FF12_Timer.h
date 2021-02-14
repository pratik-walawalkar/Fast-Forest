/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_TIMER_H
#define DCU_FF12_TIMER_H

#include "DCU_FF12.h"

void InitTimer1As1ms(void);
void InitTimer2As5us(void);
void InitTimer3ForDMA(void);
void Wait_ms(uint16 ms);

#endif //DCU_FF12_TIMER_H
