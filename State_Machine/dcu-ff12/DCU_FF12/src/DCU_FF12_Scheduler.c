/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_Scheduler.h"
#include "DCU_FF12_Application.h"
#include "DCU_FF12_Timer.h"
#include <p33EP512MU810.h>


volatile uint16 Flag_1ms;
uint16 Counter1ms  = 0;
uint16 Counter10ms  = 0;
uint16 Counter100ms = 0;
uint16 Counter500ms = 0;

void RunScheduler(void)
{
    if(dcuVar.modelBoard.app_5us_func != NULLPTR)
        InitTimer2As5us();

    while (TRUE)
    {
        if(Flag_1ms == 1)
        {
            Flag_1ms = 0;
            Counter1ms++;
            Application1ms();
        }
        if(Counter1ms >= 10)
        {
            Counter1ms = 0;
            Counter10ms++;
            Application10ms();
        }
        if(Counter10ms >= 10)
        {
            Counter10ms = 0;
            Counter100ms++;
            Application100ms();
        }
        if(Counter100ms >= 5)
        {
            Counter100ms = 0;
            Counter500ms++;
            Application500ms();
        }
        if(Counter500ms >= 2)
        {
            Counter500ms = 0;
            Application1s();
        }
        ApplicationIdle();
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _DefaultInterrupt(void)
{
    IFS0=0; IFS1=0; IFS2=0; INTCON1=0; INTCON2=0;
}

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
    Flag_1ms = 1;
    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void)
{
    Application5us();
    IFS0bits.T2IF = 0;
}