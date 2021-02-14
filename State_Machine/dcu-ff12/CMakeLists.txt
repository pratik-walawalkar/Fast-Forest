
target_sources(${PROJECT_NAME}
        PUBLIC inc/DCU_cfg.h
        PUBLIC inc/DCU_TypeDefs.h
        PUBLIC inc/DCU.h
)

target_include_directories(${PROJECT_NAME}
        PUBLIC inc
)

add_subdirectory(DCU_FF12)

