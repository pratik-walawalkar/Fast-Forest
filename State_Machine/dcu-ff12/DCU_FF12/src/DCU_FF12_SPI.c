/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_SPI.h"
#include "DCU_FF12_GPIO.h"
#include <p33EP512MU810.h>


result_t InitSPI1(DCU_spi_options options)
{
    if((options & DCU_SPI_OPTIONS_USE_ALTERNATIVE_PINS_1) > 0)
    {
        //ToDo: may be need in the future
        return R_FAILED_NOT_IMPLEMENTED;
    }
    SPI1STAT = 0x0;                             // disable the SPI module
    SPI1CON1 = 0x0161;                          // FRAMEN = 0, SPIFSD = 0, DISSDO = 0, MODE16 = 0; SMP = 0; CKP = 1; CKE = 1; SSEN = 0; MSTEN = 1; SPRE = 0b000, PPRE = 0b01
    SPI1CON1bits.CKE = 0x01;
    SPI1CON1bits.CKP = 0x00;
    SPI1STAT = 0x8000;                          // enable the SPI module

    return R_SUCCESS;
}

short TransferSPI1(short data)
{
    short temp;
    temp = SPI1BUF;                             // dummy read of the SPI1BUF register to clear the SPIRBF flag
    SPI1BUF = data;                             // write the data out to the SPI peripheral
    while (!SPI1STATbits.SPIRBF);               // wait for the data to be sent out

    return (short)(SPI1BUF & 0xFFFF);
}

result_t InitSPI2(DCU_spi_options options)
{
    if((options & DCU_SPI_OPTIONS_USE_ALTERNATIVE_PINS_1) > 0)
    {
        //ToDo: may be need in the future
        return R_FAILED_NOT_IMPLEMENTED;
    }
    SPI2STAT = 0x0;                             // disable the SPI module
    SPI2CON1 = 0x0161;                          // FRAMEN = 0, SPIFSD = 0, DISSDO = 0, MODE16 = 0; SMP = 0; CKP = 1; CKE = 1; SSEN = 0; MSTEN = 1; SPRE = 0b000, PPRE = 0b01
    SPI2CON1bits.CKE = 0x01;
    SPI2CON1bits.CKP = 0x00;
    SPI2STAT = 0x8000;                          // enable the SPI module

    return R_SUCCESS;
}

short TransferSPI2(short data)
{
    short temp;
    temp = SPI2BUF;                             // dummy read of the SPI1BUF register to clear the SPIRBF flag
    SPI2BUF = data;                             // write the data out to the SPI peripheral
    while (!SPI2STATbits.SPIRBF);               // wait for the data to be sent out

    return (short)(SPI2BUF & 0xFFFF);
}

short ReadSPI1(void)
{    
    return (short)(SPI1BUF & 0xFFFF);
}

short ReadSPI2(void)
{    
    return (short)(SPI2BUF & 0xFFFF);
}