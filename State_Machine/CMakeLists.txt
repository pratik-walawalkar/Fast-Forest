set(CMAKE_TOOLCHAIN_FILE cmake-microchip/toolchain.cmake CACHE INTERNAL "")
set(MICROCHIP_MCU dsPIC33EP512MU810 CACHE INTERNAL "MICROCHIP_MCU" FORCE)
set(MICROCHIP_XC16_PATH /opt/microchip/xc16/v1.50)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)


add_compile_options(
    "-mcpu=33EP512MU810"
)
string(APPEND CMAKE_C_LINK_FLAGS
    " -mcpu=33EP512MU810"
    " -Wl,--script,p33EP512MU810.gld"
)

cmake_minimum_required(VERSION 3.16)

add_definitions(-D__dsPIC33EP512MU810__)

project(pe-ff12 C)

add_executable(${PROJECT_NAME} main.c ${MICROCHIP_SOURCES} ${FASTGEN_SRC})

add_subdirectory(dcu-ff12)
add_subdirectory(ecu.lib)
add_subdirectory(FastGen)
add_subdirectory(MB)
add_subdirectory(microchip)


