/**
 * @file FF12_Onboarding_main.h
 * @author Fabian Bally (fabian.bally@fast-forest.de)
 * @brief 
 * @version 0.1
 * @date 2020-11-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <Global_Types.h>
#include "DCU.h"

#define MB_MAJOR_VERSION 0
#define MB_MINOR_VERSION 1

#ifdef __cplusplus
extern "C" {
#endif

extern result_t MB_Init(void);
extern result_t MB_PerformSelfTests(void);
extern void MB_Application5us(void);
extern void MB_Application1ms(void);
extern void MB_Application10ms(void);
extern void MB_Application100ms(void);
extern void MB_Application500ms(void);
extern void MB_Application1s(void);

#ifdef __cplusplus
}
#endif

