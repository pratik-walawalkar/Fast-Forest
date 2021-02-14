target_include_directories(${PROJECT_NAME} PUBLIC inc)

target_sources(${PROJECT_NAME}  PUBLIC inc/DCU.h
                                PUBLIC inc/DCU_cfg.h
                                PUBLIC inc/DCU_FF12_GPIO.h
                                PUBLIC inc/DCU_TypeDefs.h
                                PUBLIC src/DCU_MOCK_GPIO.c
                                PUBLIC inc/DCU_FF12_SPI.h
                                PUBLIC src/DCU_MOCK_SPI.c)