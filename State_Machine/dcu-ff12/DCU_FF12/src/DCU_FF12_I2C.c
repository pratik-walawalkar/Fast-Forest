/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_I2C.h"
#include <p33EP512MU810.h>


result_t InitI2C1(DCU_i2c_options options)
{
    if((options & DCU_I2C_OPTIONS_USE_ALTERNATIVE_PINS_1) > 0)
    {
        //ToDo: may be need in the future
        return R_FAILED_NOT_IMPLEMENTED;
    }

    static uint8 temp = 0;
    I2C1BRG           = 0x18B;       // baudrate: 40Mhz Fcy with 100kHz I2C clock

    I2C1CONbits.I2CEN  = 0;          // disable I2C mode
    if((options & DCU_I2C_OPTIONS_USE_SLEW_RATE_CONTROL) <= 0)
    {
        I2C1CONbits.DISSLW = 1;          // disable slew rate control
    }
    IFS1bits.MI2C1IF   = 0;          // clear interrupt
    I2C1CONbits.I2CEN  = 1;          // enable I2C mode
    temp               = I2C1RCV;    // read buffer to clear buffer full

    return R_SUCCESS;
}

void WaitForI2C1Idle(void)
{
    while (I2C1STATbits.TRSTAT);    // Wait for bus idle
}

void StartI2C1(void)
{
    I2C1CONbits.SEN = 1;            // generate start condition
    while (I2C1CONbits.SEN);        // wait for start
}

void StopI2C1(void)
{
    I2C1CONbits.PEN = 1;            // generate stop condition
    while (I2C1CONbits.PEN);        // Wait for stop
}

void RestartI2C1(void)
{
    I2C1CONbits.RSEN = 1;           // generate Restart
    while (I2C1CONbits.RSEN);       // wait for restart
}

void SendI2C1Ack(void)
{
    I2C1CONbits.ACKDT = 0;			// set for ACk
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN);		// wait for ACK to complete
}

void SendI2C1NonAck(void)
{
    I2C1CONbits.ACKDT = 1;			// set for NotACk
    I2C1CONbits.ACKEN = 1;
    while(I2C1CONbits.ACKEN);		// wait for ACK to complete
    I2C1CONbits.ACKDT = 0;			// set for NotACk
}

unsigned char GetI2C1Ack(void)
{
    return (!I2C1STATbits.ACKSTAT);
}

void WriteI2C1(unsigned char data)
{
    I2C1TRN = data;					// set tx buffer
    while (I2C1STATbits.TBF);		// wait for transmission
}

unsigned char ReadI2C1(void)
{
    I2C1CONbits.RCEN = 1;			// enable master receive
    __builtin_nop();
    while (!I2C1STATbits.RBF);      // wait for transmission
    return(I2C1RCV);				// return buffer
}


result_t InitI2C2(void)
{
    static uint8 temp = 0;
    I2C2BRG           = 0x18B;       // baudrate: 40Mhz Fcy with 100kHz I2C clock

    I2C2CONbits.I2CEN  = 0;          // disable I2C mode
    I2C2CONbits.DISSLW = 1;          // disable slew rate control
    IFS3bits.MI2C2IF   = 0;          // clear interrupt
    I2C2CONbits.I2CEN  = 1;          // enable I2C mode
    temp               = I2C2RCV;    // read buffer to clear buffer full

    return R_SUCCESS;
}

void WaitForI2C2Idle(void)
{
    while (I2C2STATbits.TRSTAT);    // Wait for bus idle
}

void StartI2C2(void)
{
    I2C2CONbits.SEN = 1;            // generate start condition
    while (I2C2CONbits.SEN);        // wait for start
}

void StopI2C2(void)
{
    I2C2CONbits.PEN = 1;            // generate stop condition
    while (I2C2CONbits.PEN);        // Wait for stop
}

void RestartI2C2(void)
{
    I2C2CONbits.RSEN = 1;           // generate Restart
    while (I2C2CONbits.RSEN);       // wait for restart
}

void SendI2C2Ack(void)
{
    I2C2CONbits.ACKDT = 0;			// set for ACk
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);		// wait for ACK to complete
}

void SendI2C2NonAck(void)
{
    I2C2CONbits.ACKDT = 1;			// set for NotACk
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);		// wait for ACK to complete
    I2C2CONbits.ACKDT = 0;			// set for NotACk
}

unsigned char GetI2C2Ack(void)
{
    return (!I2C2STATbits.ACKSTAT);
}

void WriteI2C2(unsigned char data)
{
    I2C2TRN = data;					// set tx buffer
    while (I2C2STATbits.TBF);		// wait for transmission
}

unsigned char ReadI2C2(void)
{
    I2C2CONbits.RCEN = 1;			// enable master receive
    __builtin_nop();
    while (!I2C2STATbits.RBF);      // wait for transmission
    return(I2C2RCV);				// return buffer
}