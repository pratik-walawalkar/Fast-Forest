


#include "Definitions.h"
#include <iostream>

// configure desired motion profile in the lines below
int m_ulProfileVelocity = 83;        //rpm profile speed
DWORD m_ulProfileAcceleration = 10;       //rpm / s, up to 1e7 would be possible
DWORD m_ulProfileDeceleration = 50;       //rpm / s
int number_of_cycles = 100;
int angle = 40;                  //degrees
int delay = angle / 6 / m_ulProfileVelocity; //delay in seconds.This determines the angle to be reached, approximately

int m_lHomePosition = 0;                 //start position
int m_lTargetPosition = 100000;          //put in a big number, bigger than the desired end position
int m_trigger = 50;             //the value after which the state is changed
int m_sensor_input = 0;         //input from the sensor
bool engine_on = true;




WORD m_usNodeId = 1;
int m_baudrate = 1000000;
int m_timeout = 500;
DWORD m_ulErrorCode = 0;
BOOL m_absolute = true;         //as opposed to relative
BOOL m_oImmediately = true;      //does not wait for execution of previous command
int timewait = 2000;         //maxtime for position reach
HANDLE m_KeyHandle = 0;

char deviceName = 'EPOS';
char protocolStackName = 'MAXO';
char interfaceName = 'USB';
char portName = 'USB0';

int main()
{
    HANDLE m_KeyHandle = VCS_OpenDevice(&deviceName, &protocolStackName, &interfaceName, &portName, &m_ulErrorCode);

    VCS_SetProtocolStackSettings(m_KeyHandle, m_baudrate, m_timeout, &m_ulErrorCode);
    VCS_ClearFault(m_KeyHandle, m_usNodeId, &m_ulErrorCode);
    VCS_ActivateProfilePositionMode(m_KeyHandle, m_usNodeId, &m_ulErrorCode);
    VCS_SetPositionProfile(m_KeyHandle, m_usNodeId, m_ulProfileVelocity, m_ulProfileAcceleration, m_ulProfileDeceleration, &m_ulErrorCode);
    VCS_SetEnableState(m_KeyHandle, m_usNodeId, &m_ulErrorCode);


    if (engine_on)
    {
        switch (m_sensor_input)
        {
        case 1:
            VCS_MoveToPosition(m_KeyHandle, m_usNodeId, m_lTargetPosition, m_absolute, m_oImmediately, &m_ulErrorCode);
            break;
        case 0:
            VCS_MoveToPosition(m_KeyHandle, m_usNodeId, m_lHomePosition, m_absolute, m_oImmediately, &m_ulErrorCode);
            break;

        default: VCS_MoveToPosition(m_KeyHandle, m_usNodeId, m_lHomePosition, m_absolute, m_oImmediately, &m_ulErrorCode);;
        }
    }
    else
    {
        VCS_MoveToPosition(m_KeyHandle, m_usNodeId, m_lHomePosition, m_absolute, m_oImmediately, &m_ulErrorCode);
        VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);
    }
}