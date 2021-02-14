/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_GPIO.h"
#include <p33EP512MU810.h>

void InitOscillator(void)
{
    uint16 a = 0;
    uint8 b = 0;

    a = OSCCONbits.COSC;
    b = 1;

    //Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
    PLLFBD             = 38;                        // M=40
    CLKDIVbits.PLLPOST = 0;                         // N1=2
    CLKDIVbits.PLLPRE  = 2;                         // N2=4
    OSCTUN             = 0;                         // Tune FRC oscillator, if FRC is used

    RCONbits.SWDTEN    = 0;                         //Disable Watchdog

    __builtin_write_OSCCONH(0x23);                  //Init clock switch
    __builtin_write_OSCCONL(0x01);                  //Start clock switch

    while(OSCCONbits.COSC != 0b011);                //Wait for clock switch
    while(OSCCONbits.LOCK != 1);                    //Wait for lock
}