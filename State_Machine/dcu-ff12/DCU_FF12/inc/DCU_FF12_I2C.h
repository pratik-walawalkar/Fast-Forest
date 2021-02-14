/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_I2C_H
#define DCU_FF12_I2C_H

#include "DCU_FF12.h"

result_t      InitI2C1          (DCU_i2c_options options);
void          WaitForI2C1Idle   (void);
void          StartI2C1         (void);
void          StopI2C1          (void);
void          RestartI2C1       (void);
void          SendI2C1Ack       (void);
void          SendI2C1NonAck    (void);
unsigned char GetI2C1Ack        (void);
void          WriteI2C1         (unsigned char);
unsigned char ReadI2C1          (void);


result_t      InitI2C2          (void);
void          WaitForI2C2Idle   (void);
void          StartI2C2         (void);
void          StopI2C2          (void);
void          RestartI2C2       (void);
void          SendI2C2Ack       (void);
void          SendI2C2NonAck    (void);
unsigned char GetI2C2Ack        (void);
void          WriteI2C2         (unsigned char);
unsigned char ReadI2C2          (void);


#endif //DCU_FF12_I2C_H
