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

#define ANALOG_PIN_COUNT (32)


analogFbConfig_t configuredAnalogFbs[MAX_ANALOG_FEEDBACKS];
uint8 analogPinMap[ANALOG_PIN_COUNT] = 
{
    0x10 | 0x0,           // AN0  : PORTB | PIN0
    0x10 | 0x1,           // AN1  : PORTB | PIN1
    0x10 | 0x2,           // AN2  : PORTB | PIN2
    0x10 | 0x3,           // AN3  : PORTB | PIN3
    0x10 | 0x4,           // AN4  : PORTB | PIN4
    0x10 | 0x5,           // AN5  : PORTB | PIN5
    0x10 | 0x6,           // AN6  : PORTB | PIN6
    0x10 | 0x7,           // AN7  : PORTB | PIN7
    0x10 | 0x8,           // AN8  : PORTB | PIN8
    0x10 | 0x9,           // AN9  : PORTB | PIN9
    0x10 | 0xA,           // AN10 : PORTB | PIN10
    0x10 | 0xB,           // AN11 : PORTB | PIN11
    0x10 | 0xC,           // AN12 : PORTB | PIN12
    0x10 | 0xD,           // AN13 : PORTB | PIN13
    0x10 | 0xE,           // AN14 : PORTB | PIN14
    0x10 | 0xF,           // AN15 : PORTB | PIN15
    
    0x20 | 0x1,           // AN16 : PORTC | PIN1
    0x20 | 0x2,           // AN17 : PORTC | PIN2
    0x20 | 0x3,           // AN18 : PORTC | PIN3
    0x20 | 0x4,           // AN19 : PORTC | PIN4
    
    0x40 | 0x8,           // AN20 : PORTE | PIN8
    0x40 | 0x9,           // AN21 : PORTE | PIN9
    
    0x00 | 0x6,           // AN22 : PORTA | PIN6
    0x00 | 0x7,           // AN23 : PORTA | PIN7
    
    0x40 | 0x0,           // AN24 : PORTE | PIN0
    0x40 | 0x1,           // AN25 : PORTE | PIN1
    0x40 | 0x2,           // AN26 : PORTE | PIN2
    0x40 | 0x3,           // AN27 : PORTE | PIN3
    0x40 | 0x4,           // AN28 : PORTE | PIN4
    0x40 | 0x5,           // AN29 : PORTE | PIN5
    0x40 | 0x6,           // AN30 : PORTE | PIN6
    0x40 | 0x7,           // AN31 : PORTE | PIN7
};
uint8 adcBufferToAnalogFbIndexMap[MAX_ANALOG_FEEDBACKS];
boolean adcConfigLocked = FALSE;

void InitPinsAsDefault(void)
{
    int i;

    for(i = 0; i < MAX_ANALOG_FEEDBACKS; i++)
    {
        configuredAnalogFbs[i].pVariable = NULLPTR;
        adcBufferToAnalogFbIndexMap[i] = 0xFF;
    }

    //Default init all pins as input and analog, except CAN / oscillator related pins

    //Port 1: 0 - 7, 9, 10, 14, 15
    //Port B: 0 - 15
    //Port C: 12 - 15 (Oscillator)
    //Port D: 0 - 11
    //Port E: 0 - 7
    //Port F: 0 - 6 (CAN1_TX: 0, CAN1_RX: 1, CAN2_TX: 2, CAN2_RX: 3)
    //Port G: 2 - 3, 6 - 9

    //latches
    LATA  = 0x0000;
    LATB  = 0x0000;
    LATC  = 0x0000;
    LATD  = 0x0000;
    LATE  = 0x0000;
    LATF  = 0x0000;
    LATG  = 0x0000;

    //tri-state (input = 1)
    TRISA = 0xFFFF;
    TRISB = 0xFFFF;
    TRISC = 0x7000;
    TRISD = 0x0FFF;
    TRISE = 0x00FF;
    TRISF = 0x0005; // 0x0022;
    TRISG = 0x03CC;

    //analog select (analog = 1)
    ANSELA = 0xFFFF;
    ANSELB = 0xFFFF;
    ANSELC = 0x0000;
    ANSELD = 0x0FFF;
    ANSELE = 0x00FF;
    //ANSELF = 0x004C;
    ANSELG = 0x03CC;
}

void InitPPS(void)
{
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));      //Unlock RPINRx/RPORx

    RPOR7bits.RP96R   = 0b001110;                    //CAN1TX to RP96 = Pin 87
    RPINR26bits.C1RXR = 0b1100001;                   //CAN1RX to RP97 = Pin 88
    RPOR8bits.RP99R   = 0b001111;                    //CAN2TX to RP99 = Pin 51
    RPINR26bits.C2RXR = 0b1100010;                   //CAN2RX to RP98 = Pin 52

    __builtin_write_OSCCONL(OSCCON | (1<<6));       //Lock RPINRx/RPORx
}

result_t InitDigitalInput(DCU_portpin portPin, DCU_pin_pull_config_t pullCfg)
{
    result_t result = R_SUCCESS;

    //ToDo: check for allowed pins
    
    switch(portPin.port)
    {
        case DCU_PORT_A:
            ANSELA &= ~(1UL << portPin.pin);     // unset pin bit
            TRISA |= 1UL << portPin.pin;         // set pin bit
            break;
        case DCU_PORT_B:
            ANSELB &= ~(1UL << portPin.pin);     // unset pin bit
            TRISB |= 1UL << portPin.pin;         // set pin bit
            break;
        case DCU_PORT_C:
            ANSELC &= ~(1UL << portPin.pin);     // unset pin bit
            TRISC |= 1UL << portPin.pin;         // set pin bit
            break;
        case DCU_PORT_D:
            ANSELD &= ~(1UL << portPin.pin);     // unset pin bit
            TRISD |= 1UL << portPin.pin;         // set pin bit
            break;
        case DCU_PORT_E:
            ANSELE &= ~(1UL << portPin.pin);     // unset pin bit
            TRISE |= 1UL << portPin.pin;         // set pin bit
            break;
        case DCU_PORT_F:
          //  ANSELF &= ~(1UL << portPin.pin);     // unset pin bit
            TRISF |= 1UL << portPin.pin;         // set pin bit
            break;
        case DCU_PORT_G:
            ANSELG &= ~(1UL << portPin.pin);     // unset pin bit
            TRISG |= 1UL << portPin.pin;         // set pin bit
            break;
        default:
            result = R_FAILED_PARAMETER_ERROR;
            break;
    }

    return result;
}

result_t InitDigitalOutput(DCU_portpin portPin, DCU_pin_digital_value_t initialValue)
{
    result_t result = R_SUCCESS;
    if(initialValue > 1)
    {
        result = R_FAILED_PARAMETER_ERROR;
    }
    else
    {
        
        //ToDo: check for allowed pins
        
        switch (portPin.port)
        {
            case DCU_PORT_A:
                LATA |= initialValue << portPin.pin;         // set pin bit
                ANSELA &= ~(1UL << portPin.pin);             // unset pin bit
                TRISA &= ~(1UL << portPin.pin);              // unset pin bit
                break;
            case DCU_PORT_B:
                LATB |= initialValue << portPin.pin;         // set pin bit
                ANSELB &= ~(1UL << portPin.pin);             // unset pin bit
                TRISB &= ~(1UL << portPin.pin);              // unset pin bit
                break;
            case DCU_PORT_C:
                LATC |= initialValue << portPin.pin;         // set pin bit
                ANSELC &= ~(1UL << portPin.pin);             // unset pin bit
                TRISC &= ~(1UL << portPin.pin);              // unset pin bit
                break;
            case DCU_PORT_D:
                LATD |= initialValue << portPin.pin;         // set pin bit
                ANSELD &= ~(1UL << portPin.pin);             // unset pin bit
                TRISD &= ~(1UL << portPin.pin);              // unset pin bit
                break;
            case DCU_PORT_E:
                LATE |= initialValue << portPin.pin;         // set pin bit
                ANSELE &= ~(1UL << portPin.pin);             // unset pin bit
                TRISE &= ~(1UL << portPin.pin);              // unset pin bit
                break;
            case DCU_PORT_F:
                LATF |= initialValue << portPin.pin;         // set pin bit
              //  ANSELF &= ~(1UL << portPin.pin);             // unset pin bit
                TRISF &= ~(1UL << portPin.pin);              // unset pin bit
                break;
            case DCU_PORT_G:
                LATG |= initialValue << portPin.pin;         // set pin bit
                ANSELG &= ~(1UL << portPin.pin);             // unset pin bit
                TRISG &= ~(1UL << portPin.pin);              // unset pin bit
                break;
            default:
                result = R_FAILED_PARAMETER_ERROR;
                break;
        }
    }

    return result;
}

result_t SetDigitalPin(DCU_portpin portPin, DCU_pin_digital_value_t value)
{
    result_t result = R_SUCCESS;
    if(value > 1)
    {
        result = R_FAILED_PARAMETER_ERROR;
    }
    else
    {
        //ToDo: check for allowed pins
        
        switch (portPin.port)
        {
            case DCU_PORT_A:
                LATA ^= (-value ^ PORTA) & (1UL << portPin.pin);
                break;
            case DCU_PORT_B:
                LATB ^= (-value ^ PORTB) & (1UL << portPin.pin);
                break;
            case DCU_PORT_C:
                LATC ^= (-value ^ PORTC) & (1UL << portPin.pin);
                break;
            case DCU_PORT_D:
                LATD ^= (-value ^ PORTD) & (1UL << portPin.pin);
                break;
            case DCU_PORT_E:
                LATE ^= (-value ^ PORTE) & (1UL << portPin.pin);
                break;
            case DCU_PORT_F:
                LATF ^= (-value ^ PORTF) & (1UL << portPin.pin);
                break;
            case DCU_PORT_G:
                LATG ^= (-value ^ PORTG) & (1UL << portPin.pin);
                break;
            default:
                result = R_FAILED_PARAMETER_ERROR;
                break;
        }
    }

    return result;
}

DCU_pin_digital_value_t GetDigitalPin(DCU_portpin portPin)
{
    uint16 result = 0;
    
    
    //ToDo: check for allowed pins

    switch (portPin.port)
    {
        case DCU_PORT_A:
            result = (PORTA >> portPin.pin) & 1U;
            break;
        case DCU_PORT_B:
            result = (PORTB >> portPin.pin) & 1U;
            break;
        case DCU_PORT_C:
            result = (PORTC >> portPin.pin) & 1U;
            break;
        case DCU_PORT_D:
            result = (PORTD >> portPin.pin) & 1U;
            break;
        case DCU_PORT_E:
            result = (PORTE >> portPin.pin) & 1U;
            break;
        case DCU_PORT_F:
            result = (PORTF >> portPin.pin) & 1U;
            break;
        case DCU_PORT_G:
            result = (PORTG >> portPin.pin) & 1U;
            break;
    }

    return result;
}

result_t InitAnalogInput(DCU_portpin portPin, boolean useLowPass, DCU_analog_read_ref_t pVar)
{
    result_t result = R_FAILED_NO_RESOURCES_LEFT;
    int i;
        
    if(adcConfigLocked || GetAnalogPinIndexForPin(portPin) == 0xFF)
    {
        result = R_FAILED_PARAMETER_ERROR;
    }
    else
    {
        for(i = 0; i < MAX_ANALOG_FEEDBACKS; i++)
        {
            if(configuredAnalogFbs[i].pVariable == NULLPTR)
            {
                configuredAnalogFbs[i].portPin.port = portPin.port;
                configuredAnalogFbs[i].portPin.pin = portPin.pin;
                configuredAnalogFbs[i].useLowPass = useLowPass;
                configuredAnalogFbs[i].pVariable = pVar;

                switch(portPin.port)
                {
                    case DCU_PORT_A:
                        ANSELA |= 1UL << portPin.pin;        // set pin bit
                        TRISA |= 1UL << portPin.pin;         // set pin bit
                        break;
                    case DCU_PORT_B:
                        ANSELB |= 1UL << portPin.pin;        // set pin bit
                        TRISB |= 1UL << portPin.pin;         // set pin bit
                        break;
                    case DCU_PORT_C:
                        ANSELC |= 1UL << portPin.pin;        // set pin bit
                        TRISC |= 1UL << portPin.pin;         // set pin bit
                        break;
                    case DCU_PORT_D:
                        ANSELD |= 1UL << portPin.pin;        // set pin bit
                        TRISD |= 1UL << portPin.pin;         // set pin bit
                        break;
                    case DCU_PORT_E:
                        ANSELE |= 1UL << portPin.pin;        // set pin bit
                        TRISE |= 1UL << portPin.pin;         // set pin bit
                        break;             
                    case DCU_PORT_G:
                        ANSELG |= 1UL << portPin.pin;        // set pin bit
                        TRISG |= 1UL << portPin.pin;         // set pin bit
                        break;
                }

                result = R_SUCCESS;
                break;
            }
        }
    }        

    return result;
}

uint16 GetMaskForDefinedAnalogInputsHigh(void)
{
    uint16 result = 0;
    int i;
    uint8 analogPinIndex = 0;
    
    for(i = 0; i < MAX_ANALOG_FEEDBACKS; i++)
    {
        if(configuredAnalogFbs[i].pVariable != NULLPTR)
        {
            analogPinIndex = GetAnalogPinIndexForPin(configuredAnalogFbs[i].portPin);
            if(analogPinIndex >= 16 && analogPinIndex < 32)
            {
                result |= 1UL << (analogPinIndex-16);
            }
            
        }
    }
    
    return result;
}

uint16 GetMaskForDefinedAnalogInputsLow(void)
{
    uint16 result = 0;
    int i;
    uint8 analogPinIndex = 0;
    
    for(i = 0; i < MAX_ANALOG_FEEDBACKS; i++)
    {
        if(configuredAnalogFbs[i].pVariable != NULLPTR)
        {            
            analogPinIndex = GetAnalogPinIndexForPin(configuredAnalogFbs[i].portPin);
            if(analogPinIndex >= 0 && analogPinIndex < 16)
            {
                result |= 1UL << (analogPinIndex);
            }
            
        }
    }
    
    return result;
}

uint8 GetAnalogPinIndexForPin(DCU_portpin portPin)
{
    uint8 result = 0xFF; // invalid / not found
    uint8 toFind = ((portPin.port << 8) & 0xF0) | (portPin.pin & 0x0F);
    int i;
    
    for(i = 0; i < ANALOG_PIN_COUNT; i++)
    {
        if(analogPinMap[i] == toFind)
        {
            result = i;
            break;
        }
    }
    
    return result;
}

uint8 GetActiveAnalogInputsCount(void)
{
    uint8 result = 0;
    int i;
    
    for(i = 0; i < MAX_ANALOG_FEEDBACKS; i++)
    {
        if(configuredAnalogFbs[i].pVariable != NULLPTR)
        {            
            result++;            
        }
    }
    
    return result;
}

void CreateAdcBufferToAnalogFbIndexMap(void)
{
    int i, j;
    uint8 minimumLastIndex = 0x00;
    uint8 minimumIndex;
    uint8 currentIndex;
    
    for(j = 0; j < MAX_ANALOG_FEEDBACKS; j++)
    {
        minimumIndex = 0xFF;
        adcBufferToAnalogFbIndexMap[j] = 0xFF; // invalid
        for(i = 0; i < MAX_ANALOG_FEEDBACKS; i++)
        {
            if(configuredAnalogFbs[i].pVariable != NULLPTR)
            {       
                currentIndex = GetAnalogPinIndexForPin(configuredAnalogFbs[i].portPin);
                if(currentIndex < minimumIndex && minimumLastIndex < currentIndex)
                {
                    minimumIndex = currentIndex;
                    adcBufferToAnalogFbIndexMap[j] = i;
                }
            }
        }
        
        minimumLastIndex = minimumIndex;
    }
    
    adcConfigLocked = TRUE;
}

void SetAnalogFbValue(int index, uint16 value)
{
    if(adcBufferToAnalogFbIndexMap[index] != 0xFF && configuredAnalogFbs[adcBufferToAnalogFbIndexMap[index]].pVariable != NULLPTR)
    {
        (*configuredAnalogFbs[adcBufferToAnalogFbIndexMap[index]].pVariable) = value;
    }
}

boolean GetAnalogFbValueUsesLowPass(int index)
{
    boolean result = FALSE;
    
    if(adcBufferToAnalogFbIndexMap[index] != 0xFF && configuredAnalogFbs[adcBufferToAnalogFbIndexMap[index]].pVariable != NULLPTR)
    {
        result = configuredAnalogFbs[adcBufferToAnalogFbIndexMap[index]].useLowPass;
    }
    
    return result;
}