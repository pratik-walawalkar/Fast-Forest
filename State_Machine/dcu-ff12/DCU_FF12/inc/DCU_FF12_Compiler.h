/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_COMPILER_H
#define DCU_FF12_COMPILER_H

#if (__XC16_VERSION < 1034)
    #error "Please use XC16 1.50 Compiler or newer"
#endif


#if __XC16_VERSION >= 1050
// FOSC
    #pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
    #pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
    #pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow only one reconfiguration)
    #pragma config FCKSM = CSECMD           // enables clock switching mode

    // FOSCSEL
    #pragma config FNOSC = PRI              // Oscillator Source Selection (Primary Oscillator (XT, HS, EC))
    #pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

    // FWDT
    #pragma config FWDTEN = OFF             // Disable always on watchdog, now controlled by software
    #pragma config WDTPRE = PR32            // Select 5bit Prescaler 1:32
    #pragma config WDTPOST = PS2048           // Select 1:2048 Postscaler => watchdog timeout 2048 ms

    // FICD
    #pragma config JTAGEN = OFF
    #pragma config ICS = PGD2

    // FPOR
    #pragma config ALTI2C1 = OFF
#endif

#endif //DCU_FF12_COMPILER_H
