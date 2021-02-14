/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_ADC.h"
#include "DCU_FF12_Timer.h"
#include "DCU_FF12_Application.h"
#include "DCU_FF12_GPIO.h"
#include "FastGen_Config.h"
#include <p33EP512MU810.h>


unsigned short Adc1BufferA[MAX_ANALOG_FEEDBACKS];        // Max ADC Buffer size
unsigned short Adc1BufferB[MAX_ANALOG_FEEDBACKS];        // Max ADC Buffer size
unsigned short* Adc1CurrentBuffer;
uint8 dmaBuffer = 0;
unsigned short Adc1LowPassBuffer[MAX_ANALOG_FEEDBACKS*MAX_ANALOG_LOWPASS_SIZE];
uint16 adcActiveChannelCount = 0;
int lowPassCursor = 0;
int channelCursor = 0;
uint16 tempAdcValue = 0;
uint8 tempLowPassCounter = 0;


void InitADC(void)
{
    adcActiveChannelCount = GetActiveAnalogInputsCount();
    CreateAdcBufferToAnalogFbIndexMap();
    
    AD1CON1bits.FORM      = 0;                              // Data Output Format: Signed Fraction (Q15 format)
    AD1CON1bits.SSRC      = 2;                              // Sample Clock Source: GP timer starts conversion
    AD1CON1bits.ASAM      = 1;                              // Sampling begins immediately after conversion
    AD1CON1bits.AD12B     = 1;                              // 12-bit ADC operation
    AD1CON1bits.SIMSAM    = 0;                              // Samples multiple channels sequentially
    AD1CON2bits.BUFM      = 0;
    AD1CON2bits.CSCNA     = 1;                              // Scan CH0+ Input Selections during Sample A bit
    AD1CON2bits.CHPS      = 0;                              // Converts CH0
    AD1CON2bits.VCFG      = 0;                              // VREFH = AVDD = 3,3V / VREFL = Avss = GND
    AD1CON3bits.ADRC      = 0;                              // ADC clock is derived from systems clock
    AD1CON3bits.ADCS      = 63;                             // ADC conversion clock  (ADCS<7:0> + 1) * TCY = TAD
    AD1CON4bits.ADDMAEN   = 1;

    //AD1CHS0: Analog-to-Digital Input Select Register
    AD1CHS0bits.CH0SA     = 0;                              // MUXA +ve input selection (AIN0) for CH0
    AD1CHS0bits.CH0NA     = 0;                              // MUXA -ve input selection (VREF-) for CH0

    //AD1CHS123: Analog-to-Digital Input Select Register
    AD1CHS123bits.CH123SA = 0;                              // MUXA +ve input selection (AIN0) for CH1
    AD1CHS123bits.CH123NA = 0;                              // MUXA -ve input selection (VREF-) for CH1

    AD1CSSH = GetMaskForDefinedAnalogInputsHigh();
    AD1CSSL = GetMaskForDefinedAnalogInputsLow();

    InitTimer3ForDMA();
    InitDMA5ForADC();

    AD1CON1bits.ADDMABM   = 0;                              // DMA buffers are built in scatter/gather mode
    AD1CON2bits.SMPI      = ((adcActiveChannelCount- 1) & 0x0F);          // ADC buffers (-1)
    AD1CON4bits.DMABL     = 0;                              // Each buffer contains 1 words

    IFS0bits.AD1IF        = 0;                              // Clear Analog-to-Digital Interrupt Flag bit
    IEC0bits.AD1IE        = 0;                              // Do Not Enable Analog-to-Digital interrupt
    AD1CON1bits.ADON      = 1;                              // Turn on the ADC
}

void InitDMA5ForADC(void)
{
    DMA5CONbits.AMODE = 0;                                  // Configure DMA for Peripheral Indirect mode
    DMA5CONbits.MODE  = 2;                                  // Configure DMA for Continuous Ping-Pong mode
    DMA5PAD           = (volatile unsigned int)&ADC1BUF0;   // Point DMA to ADC1BUF0
    DMA5CNT           = ((adcActiveChannelCount- 1) & 0x0F);           // DMA request (1 buffer, each with 1 words)  (-1)
    DMA5REQ           = 13;                                 // Select ADC1 as DMA request source

    DMA5STAL          = (unsigned int)&Adc1BufferA;
    DMA5STAH          = 0x0000;
    DMA5STBL          = (unsigned int)&Adc1BufferB;
    DMA5STBH          = 0x0000;

    IFS3bits.DMA5IF   = 0;                                  // Clear the DMA Interrupt Flag bit
    IEC3bits.DMA5IE   = 1;                                  // Set the DMA Interrupt Enable bit
    DMA5CONbits.CHEN  = 1;                                  // Enable DMA
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA5Interrupt(void)
{
    if(dmaBuffer == 0)
    {
        Adc1CurrentBuffer = Adc1BufferA;
    }
    else
    {
        Adc1CurrentBuffer = Adc1BufferB;
    }
    
    for(channelCursor = 0; channelCursor < adcActiveChannelCount; channelCursor++)
    {
        if(GetAnalogFbValueUsesLowPass(channelCursor))
        {
            Adc1LowPassBuffer[channelCursor * MAX_ANALOG_LOWPASS_SIZE + lowPassCursor] = Adc1CurrentBuffer[channelCursor];
            tempAdcValue = 0;
            for(tempLowPassCounter = 0; 
                    tempLowPassCounter < FASTGEN_CONFIG_CP_DCU_ADC_LowPass &&
                    tempLowPassCounter < MAX_ANALOG_LOWPASS_SIZE; tempLowPassCounter++)
            {
                tempAdcValue += Adc1LowPassBuffer[channelCursor * MAX_ANALOG_LOWPASS_SIZE + tempLowPassCounter];
            }
            tempAdcValue = (tempAdcValue / FASTGEN_CONFIG_CP_DCU_ADC_LowPass);
            SetAnalogFbValue(channelCursor, tempAdcValue);
        }
        else
        {
            SetAnalogFbValue(channelCursor, Adc1CurrentBuffer[channelCursor]);
        }
    }
    
    lowPassCursor++;
    if(lowPassCursor >= FASTGEN_CONFIG_CP_DCU_ADC_LowPass ||
       lowPassCursor >= MAX_ANALOG_LOWPASS_SIZE)
    {
        lowPassCursor = 0;
    }
    
    dmaBuffer ^= 1;

    ApplicationADCInterrupt();

    IFS3bits.DMA5IF = 0;
}