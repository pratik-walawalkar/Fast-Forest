/**
 * @file Global_Types.h
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief Global used types and defines
 * @version 0.2
 * @date 2019-11-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef GLOBAL_TYPES_H
#define GLOBAL_TYPES_H

// BASE TYPES

typedef unsigned char           boolean;
typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned long           uint32;
typedef unsigned long long      uint64;
typedef   signed char           int8;
typedef   signed short          int16;
typedef   signed long           int32;
typedef   signed long long      int64;
typedef          float          float32;
typedef          long double    float64;

#define NULLPTR                 0

// GLOBAL TYPES

typedef int16                           result_t;
#define R_SUCCESS                       0x00
#define R_FAILED                        0x01
#define R_FAILED_PARAMETER_ERROR        0x02
#define R_FAILED_NOT_IMPLEMENTED        0x04
#define R_FAILED_NO_RESOURCES_LEFT      0x08
#define R_FAILED_NO_HW_SUPPORT          0x10
#define R_FAILED_SELFTEST_NON_CRITICAL  0x20
 
typedef uint8                       status_t;
#define STATUS_OK                   0
#define STATUS_WARNING              1
#define STATUS_ERROR                2
#define STATUS_CRITICAL             3

// GLOBAL DEFINES

#ifndef TRUE
    #define TRUE                1
#endif 

#ifndef FALSE
    #define FALSE               0
#endif 

#define MASK16_BIT_0            ((uint16)0x0001)
#define MASK16_BIT_1            ((uint16)0x0002)
#define MASK16_BIT_2            ((uint16)0x0004)
#define MASK16_BIT_3            ((uint16)0x0008)
#define MASK16_BIT_4            ((uint16)0x0010)
#define MASK16_BIT_5            ((uint16)0x0020)
#define MASK16_BIT_6            ((uint16)0x0040)
#define MASK16_BIT_7            ((uint16)0x0080)
#define MASK16_BIT_8            ((uint16)0x0100)
#define MASK16_BIT_9            ((uint16)0x0200)
#define MASK16_BIT_10           ((uint16)0x0400)
#define MASK16_BIT_11           ((uint16)0x0800)
#define MASK16_BIT_12           ((uint16)0x1000)
#define MASK16_BIT_13           ((uint16)0x2000)
#define MASK16_BIT_14           ((uint16)0x4000)
#define MASK16_BIT_15           ((uint16)0x8000)

#define MASK8_BIT_0             ((uint8)0x01)
#define MASK8_BIT_1             ((uint8)0x02)
#define MASK8_BIT_2             ((uint8)0x04)
#define MASK8_BIT_3             ((uint8)0x08)
#define MASK8_BIT_4             ((uint8)0x10)
#define MASK8_BIT_5             ((uint8)0x20)
#define MASK8_BIT_6             ((uint8)0x40)
#define MASK8_BIT_7             ((uint8)0x80)


#endif // GLOBAL_TYPES_H