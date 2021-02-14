/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_APPLICATION_H
#define DCU_FF12_APPLICATION_H

#include "DCU_FF12.h"

result_t Init                   (void);
result_t SelfTest               (void);
void PostDCUSelfTest           (void);
void PostMBInit                 (void);
void Reset                      (void);
void FaultLoop                  (void);
void Application5us             (void);
void Application1ms             (void);
void Application10ms            (void);
void Application100ms           (void);
void Application500ms           (void);
void Application1s              (void);
void ApplicationIdle            (void);
void ApplicationADCInterrupt    (void);

#endif //DCU_FF12_APPLICATION_H
