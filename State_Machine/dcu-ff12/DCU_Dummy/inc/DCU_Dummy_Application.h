/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.2
 * @date 2020-06-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_DUMMY_APPLICATION_H
#define DCU_DUMMY_APPLICATION_H

#include <Global_Types.h>
#include "DCU_x86_Dummy.h"


result_t Init                   (void);
result_t SelfTest               (void);
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

#endif //DCU_DUMMY_APPLICATION_H
