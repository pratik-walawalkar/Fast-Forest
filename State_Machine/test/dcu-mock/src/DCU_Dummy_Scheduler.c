/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.2
 * @date 2020-06-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_Dummy_Scheduler.h"
#include "DCU_Dummy_Application.h"
#include "timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/can.h>



void RunDummyInterruptHandler(void)
{
    tick_t last1msInterrupt, freq1ms;
    uint16 interruptCounters[4] = { 0 };

    timer_lib_initialize();
    freq1ms  = timer_ticks_per_second() / 1000;
    last1msInterrupt = timer_current();

    while(TRUE)
    {
        if(timer_elapsed_ticks(last1msInterrupt) > freq1ms)
        {
            last1msInterrupt = timer_current();
            interruptCounters[0]++;
            Application1ms();

            if(interruptCounters[0] >= 10)
            {
                interruptCounters[0] = 0;
                interruptCounters[1]++;
                Application10ms();
            }
            if(interruptCounters[1] >= 10)
            {
                interruptCounters[1] = 0;
                interruptCounters[2]++;
                Application100ms();
            }
            if(interruptCounters[2] >= 5)
            {
                interruptCounters[2] = 0;
                interruptCounters[3]++;
                Application500ms();
            }
            if(interruptCounters[3] >= 2)
            {
                interruptCounters[3] = 0;
                Application1s();
            }
        }
        ApplicationIdle();
    }

    timer_lib_shutdown();
}