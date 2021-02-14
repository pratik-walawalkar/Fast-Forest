/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.2
 * @date 2020-05-19
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DCU_Dummy_SocketCAN.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>


result_t InitCANInterface(int *pCanSocket, char *interfaceName);

int can1Socket;
int can2Socket;


result_t SCAN_Init(void)
{
    result_t result;

    result = InitCANInterface(&can1Socket, "can0");
    result |= InitCANInterface(&can2Socket, "can1");

    return result;
}

result_t InitCANInterface(int *pCanSocket, char *interfaceName)
{
    struct ifreq ifr;
    struct sockaddr_can addr;

    if (((*pCanSocket) = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        return R_FAILED;
    }

    strcpy(ifr.ifr_name, interfaceName);
    ioctl(*pCanSocket, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(*pCanSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        return R_FAILED;
    }

    return R_SUCCESS;
}

result_t SCAN_SendMsg(DCU_can_t canInterface, struct can_frame *msg)
{
    int canSocket;
    if(canInterface == DCU_CAN1)
        canSocket = can1Socket;
    else if(canInterface == DCU_CAN2)
        canSocket = can2Socket;
    else
        return R_FAILED_PARAMETER_ERROR;

    if (write(canSocket, msg, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        return R_FAILED;
    }

    return R_SUCCESS;
}