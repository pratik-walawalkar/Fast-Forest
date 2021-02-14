/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_FF12_Status.h"
#include "DCU_FF12_GPIO.h"
#include "FastGen_CAN_Var.h"
#include "FastGen_Config.h"
#include "DCU_FF12_Timer.h"
#include "DCU.h"
#include <p33EP512MU810.h>


DCU_portpin statusLEDGreenPin   = { DCU_PORT_A, 9 };
DCU_portpin statusLEDRedPin     = { DCU_PORT_G, 0 };
DCU_portpin statusLEDGreenFBPin = { DCU_PORT_A, 10 };
DCU_portpin statusLEDRedFBPin   = { DCU_PORT_G, 1 };
uint8_t statusLedCounter = 0;


void InitStatusAndLEDs(void)
{
    DCU_InitDigitalOutput(statusLEDGreenPin, DCU_PIN_UNSET);
    DCU_InitDigitalOutput(statusLEDRedPin, DCU_PIN_UNSET);
    DCU_InitDigitalInput(statusLEDGreenFBPin, DCU_NO_PULLUP_DOWN);
    DCU_InitDigitalInput(statusLEDRedFBPin, DCU_NO_PULLUP_DOWN);

    dcuVar.status = STATUS_OK;
}

void SetStatusLedGreen(DCU_pin_digital_value_t value)
{        
    DCU_DigitalSet(statusLEDGreenPin, value);
    can_var.send.ST_DCU_StatusLedGreen = value & 0x1;
}

void SetStatusLedRed(DCU_pin_digital_value_t value)
{    
    DCU_DigitalSet(statusLEDRedPin, value);
    can_var.send.ST_DCU_StatusLedRed = value & 0x1;
}

result_t GetHighestCanError()
{
    if(FASTGEN_ANY_ACTIVE_CRITICALS)
        return STATUS_CRITICAL;
    if(FASTGEN_ANY_ACTIVE_ERRORS)
        return STATUS_ERROR;
    if(FASTGEN_ANY_ACTIVE_WARNINGS)
        return STATUS_WARNING;    
    
    return STATUS_OK;    
}

void CheckStatusAndUpdateLEDs(void)
{
    CheckStatusLedFeedbacks();
    dcuVar.status = GetHighestCanError();
    can_var.send.ST_DCU_StatusLed = dcuVar.status;

    if(FASTGEN_CONFIG_CP_DCU_StatusLED > 0)
    {
        if(statusLedCounter == 9)
            statusLedCounter = 0;
        else
            statusLedCounter++;

        switch(dcuVar.status)
        {
            case STATUS_OK:
                SetStatusLedGreen(statusLedCounter < 5);
                SetStatusLedRed(DCU_PIN_UNSET);
                break;
            case STATUS_WARNING:
                SetStatusLedGreen(statusLedCounter < 5);
                SetStatusLedRed(DCU_DigitalGet(statusLEDGreenPin));
                break;
            case STATUS_ERROR:
                SetStatusLedGreen(DCU_PIN_UNSET);
                SetStatusLedRed(statusLedCounter < 5);
                break;
            case STATUS_CRITICAL:
                SetStatusLedGreen(DCU_PIN_UNSET);
                SetStatusLedRed((statusLedCounter%2) > 0);
                break;
        }
    }
    else
    {
        SetStatusLedGreen(DCU_PIN_UNSET);
        SetStatusLedRed(DCU_PIN_UNSET);
    }
}

void CheckStatusLedFeedbacks()
{
    if(FASTGEN_CONFIG_CP_DCU_StatusLED > 0)
    {
        can_var.send.FB_DCU_StatusLedGreen = !DCU_DigitalGet(statusLEDGreenFBPin); // FB is inverted
        can_var.send.FB_DCU_StatusLedRed = !DCU_DigitalGet(statusLEDRedFBPin); // FB is inverted
        if(can_var.send.FB_DCU_StatusLedGreen != can_var.send.ST_DCU_StatusLedGreen) 
        {
            can_var.send.EE_DCU_StatusLedGreen = 1;
        }
        if(can_var.send.FB_DCU_StatusLedRed != can_var.send.ST_DCU_StatusLedRed) 
        {
            can_var.send.EE_DCU_StatusLedRed = 1;
        }
    }
    else
    {
        can_var.send.EE_DCU_StatusLedGreen = 0;
        can_var.send.EE_DCU_StatusLedRed = 0;
    }
}

result_t SelfTestStatusLEDs(void)
{
    result_t result = R_SUCCESS;
    if(FASTGEN_CONFIG_CP_DCU_StatusLED == 0)
    {
        return result;   
    }
    
    DCU_pin_digital_value_t LEDgreen = DCU_PIN_UNSET;
    DCU_pin_digital_value_t LEDred = DCU_PIN_UNSET;
    
    DCU_DigitalSet(statusLEDGreenPin, DCU_PIN_SET);
    Wait_ms(5); 
    LEDgreen = !DCU_DigitalGet(statusLEDGreenFBPin); // FB is inverted

    if(LEDgreen != DCU_PIN_SET) 
    {
        result |= R_FAILED;
        can_var.send.EE_DCU_StatusLedGreen = 1;
    }
    
    DCU_DigitalSet(statusLEDGreenPin, DCU_PIN_UNSET);
    Wait_ms(5);    
    LEDgreen = !DCU_DigitalGet(statusLEDGreenFBPin); // FB is inverted

    if(LEDgreen != DCU_PIN_UNSET) 
    {
        result |= R_FAILED;
        can_var.send.EE_DCU_StatusLedGreen = 1;
    }
    
    DCU_DigitalSet(statusLEDRedPin, DCU_PIN_SET);
    Wait_ms(5);   
    LEDred = !DCU_DigitalGet(statusLEDRedFBPin); // FB is inverted

    if(LEDred != DCU_PIN_SET) 
    {
        result |= R_FAILED;
        can_var.send.EE_DCU_StatusLedRed = 1;
    }
    
    DCU_DigitalSet(statusLEDRedPin, DCU_PIN_UNSET);
    Wait_ms(5);   
    LEDred = !DCU_DigitalGet(statusLEDRedFBPin); // FB is inverted

    if(LEDred != DCU_PIN_UNSET) 
    {
        result |= R_FAILED;
        can_var.send.EE_DCU_StatusLedRed = 1;
    }
    
    return result;
}


void SetFaultStatus()
{
    DCU_DigitalSet(statusLEDRedPin, DCU_PIN_SET);
    can_var.send.EC_DCU_EcuNonOperational = 1;
}
