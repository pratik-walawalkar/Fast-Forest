/**
 * @file DCU_cfg.h
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief Used to configure the DCU module
 * @version 0.2
 * @date 2019-11-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */


#ifndef DCU_CFG_H
#define DCU_CFG_H

//#define USE_X86_DUMMY

#define DCU_SEND_STATUS_10MS                //if set, sends the STATUS CAN msg every 10ms instead of all 100ms

#define MAX_ANALOG_FEEDBACKS                16 // ( <= 16 )
#define MAX_ANALOG_LOWPASS_SIZE             10



#endif // DCU_CFG_H