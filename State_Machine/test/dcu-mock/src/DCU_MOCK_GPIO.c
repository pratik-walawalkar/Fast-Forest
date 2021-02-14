#include "DCU_FF12_GPIO.h"

uint16 PORTA;
uint16 PORTB;
uint16 PORTC;
uint16 PORTD;
uint16 PORTE;
uint16 PORTF;
uint16 PORTG;

uint16 LATA;
uint16 LATB;
uint16 LATC;
uint16 LATD;
uint16 LATE;
uint16 LATF;
uint16 LATG;

result_t SetDigitalPin(DCU_portpin portPin, DCU_pin_digital_value_t value)
{
    result_t result = R_SUCCESS;
    if (value > 1)
    {
        result = R_FAILED_PARAMETER_ERROR;
    }
    else
    {
        // ToDo: check for allowed pins

        switch (portPin.port)
        {
        case DCU_PORT_A:
            LATA ^= (-value ^ LATA) & (1UL << portPin.pin);
            break;
        case DCU_PORT_B:
            LATB ^= (-value ^ LATB) & (1UL << portPin.pin);
            break;
        case DCU_PORT_C:
            LATC ^= (-value ^ LATC) & (1UL << portPin.pin);
            break;
        case DCU_PORT_D:
            LATD ^= (-value ^ LATD) & (1UL << portPin.pin);
            break;
        case DCU_PORT_E:
            LATE ^= (-value ^ LATE) & (1UL << portPin.pin);
            break;
        case DCU_PORT_F:
            LATF ^= (-value ^ LATF) & (1UL << portPin.pin);
            break;
        case DCU_PORT_G:
            LATG ^= (-value ^ LATG) & (1UL << portPin.pin);
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

    // ToDo: check for allowed pins

    switch (portPin.port)
    {
    case DCU_PORT_A:
        result = (LATA >> portPin.pin) & 1U;
        break;
    case DCU_PORT_B:
        result = (LATB >> portPin.pin) & 1U;
        break;
    case DCU_PORT_C:
        result = (LATC >> portPin.pin) & 1U;
        break;
    case DCU_PORT_D:
        result = (LATD >> portPin.pin) & 1U;
        break;
    case DCU_PORT_E:
        result = (LATE >> portPin.pin) & 1U;
        break;
    case DCU_PORT_F:
        result = (LATF >> portPin.pin) & 1U;
        break;
    case DCU_PORT_G:
        result = (LATG >> portPin.pin) & 1U;
        break;
    }

    return result;
}