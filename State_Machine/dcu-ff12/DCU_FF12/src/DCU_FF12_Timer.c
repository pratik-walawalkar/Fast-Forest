/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_Timer.h"
#include <p33EP512MU810.h>

#include "DCU_FF12_Oscillator.h"
#include <libpic30.h>


void InitTimer1As1ms(void)
{
    T1CON           = 0;        //clear configuration register
    T1CONbits.TCS   = 0;        //Internal peripheral clock
    T1CONbits.TSYNC = 0;        //1=Sycnorn, 0=Asycnorn
    T1CONbits.TCKPS = 1;        //Prescaler: 0=1; 1=8; 2=64; 3=256
    T1CONbits.TGATE = 0;        //disable gated time accumulation
    T1CONbits.TSIDL = 0;        //Continue module operation in Idle mode
    TMR1            = 0;        //clear count register
    PR1             = 5000;     //Periode | PRx = [(fclk/prescaler)*T]
    IFS0bits.T1IF   = 0;        //reset interrupt flag
    IPC0bits.T1IP   = 5;        //interrupt priority
    IEC0bits.T1IE   = 1;        //enable interrupt
    T1CONbits.TON   = 1;        //1=0n 0=off
}

void InitTimer2As5us(void)
{
    T2CON           = 0;        //clear configuration register
    T2CONbits.TCS   = 0;        //Internal peripheral clock
    T2CONbits.T32   = 0;        //16-bit Timer
    T2CONbits.TCKPS = 1;        //Prescaler: 0=1; 1=8; 2=64; 3=256
    T2CONbits.TGATE = 0;        //disable gated time accumulation
    T2CONbits.TSIDL = 0;        //Continue module operation in Idle mode
    TMR2            = 0;        //clear count register
    PR2             = 25;       //Periode | PRx = [(fclk/prescaler)*T]
    IFS0bits.T2IF   = 0;        //reset interrupt flag
    IPC1bits.T2IP   = 6;        //interrupt priority
    IEC0bits.T2IE   = 1;        //enable interrupt
    T2CONbits.TON   = 1;        //1=0n 0=off
}

void InitTimer3ForDMA(void)
{
    TMR3            = 0x0000;
    PR3             = 8334;     //Trigger ADC1 every 1,6668 ms
    T3CONbits.TCKPS = 1;        //Prescaler: 0=1; 1=8; 2=64; 3=256
    IFS0bits.T3IF   = 0;        //reset interrupt flag
    IEC0bits.T3IE   = 0;        //disable interrupt
    T3CONbits.TON   = 1;        //1=0n 0=off
}

void Wait_ms(uint16 ms)
{
    __delay_ms(ms);
}