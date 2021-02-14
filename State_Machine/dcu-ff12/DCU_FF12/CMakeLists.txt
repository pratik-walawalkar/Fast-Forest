target_sources(${PROJECT_NAME}
        PUBLIC inc/DCU_FF12.h
        PUBLIC src/DCU_FF12.c
        PUBLIC inc/DCU_FF12_ADC.h
        PUBLIC src/DCU_FF12_ADC.c
        PUBLIC inc/DCU_FF12_Application.h
        PUBLIC src/DCU_FF12_Application.c
        PUBLIC inc/DCU_FF12_CAN.h
        PUBLIC src/DCU_FF12_CAN.c
        PUBLIC inc/DCU_FF12_Compiler.h
        PUBLIC inc/DCU_FF12_Configuration.h
        PUBLIC src/DCU_FF12_Configuration.c
        PUBLIC inc/DCU_FF12_EEPROM.h
        PUBLIC src/DCU_FF12_EEPROM.c
        PUBLIC inc/DCU_FF12_GPIO.h
        PUBLIC src/DCU_FF12_GPIO.c
        PUBLIC inc/DCU_FF12_HWID.h
        PUBLIC src/DCU_FF12_HWID.c
        PUBLIC inc/DCU_FF12_I2C.h
        PUBLIC src/DCU_FF12_I2C.c
        PUBLIC inc/DCU_FF12_Oscillator.h
        PUBLIC src/DCU_FF12_Oscillator.c
        PUBLIC inc/DCU_FF12_Scheduler.h
        PUBLIC src/DCU_FF12_Scheduler.c
        PUBLIC inc/DCU_FF12_SoftStart.h
        PUBLIC src/DCU_FF12_SoftStart.c
        PUBLIC inc/DCU_FF12_SPI.h
        PUBLIC src/DCU_FF12_SPI.c
        PUBLIC inc/DCU_FF12_Status.h
        PUBLIC src/DCU_FF12_Status.c
        PUBLIC inc/DCU_FF12_Timer.h
        PUBLIC src/DCU_FF12_Timer.c
        PUBLIC inc/DCU_FF12_VoltageSensing.h
        PUBLIC src/DCU_FF12_VoltageSensing.c
)

target_include_directories(${PROJECT_NAME}
        PUBLIC inc
)