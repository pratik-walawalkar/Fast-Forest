/**
 * @file Vehicle_Status.h
 * @author Fabian Bally (fabian.bally@fast-forest.de)
 * @brief Vehicle Status Enum - used globally between BMS, PE, DB an others
 * @version 0.2
 * @date 2019-11-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

enum VehicleState
{
    GroundedLowVoltage, 
    WishTractiveSystem, 
    TractiveSystem, 
    WishReadyToDrive, 
    ReadyToDrive
};