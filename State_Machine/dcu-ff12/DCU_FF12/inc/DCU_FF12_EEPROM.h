/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_EEPROM_H
#define DCU_FF12_EEPROM_H

#include "DCU_FF12.h"

#define EEPROM_ADDRESS 0xA7

void          InitEEPROM                        (void);
result_t      SelfTestEEPROMCommunication     (void);
result_t      SelfTestEEPROMHWID               (void);
void          CheckForSWVersionChangesEEPROM (void);
void          WriteEEPROM                       (uint8 highAdd, uint8 lowAdd, uint8 data);
uint8         ReadEEPROM                        (uint8 highAdd, uint8 lowAdd);
void          GetAllInfosFromEEPROM            (void);
void          SaveAllInfosToEEPROM              (void);
void          CleanAllInfosFromEEPROM          (void);
void          GetAllParametersFromEEPROM       (void);
void          GetAllCalibrationsFromEEPROM     (void);
void          CleanAllParametersFromEEPROM     (void);
void          CleanAllCalibrationsFromEEPROM   (void);
void          SaveNewParameterToEEPROM         (void);
void          SaveNewCalibrationToEEPROM       (void);
void          WriteParameterToEEPROM            (uint16 parameterId);
void          WriteCalibrationToEEPROM          (uint16 calibrationId);
void          ReadParameterFromEEPROM           (uint16 parameterId);
void          ReadCalibrationFromEEPROM         (uint16 calibrationId);

extern unsigned short EEPROM_Parameter[];
extern unsigned short EEPROM_Calibration[];

#endif //DCU_FF12_EEPROM_H
