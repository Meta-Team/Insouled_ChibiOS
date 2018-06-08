set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.8)
project(Insouled_ChibiOS)

### Use make to build target STLink
add_custom_target(STLink COMMAND make upload-stlink -j -C ${Insouled_ChibiOS_SOURCE_DIR}/dev
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

add_custom_target(JLink COMMAND make upload-jlink -j -C ${Insouled_ChibiOS_SOURCE_DIR}/dev
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

### Append new source files to dev/dev.mk

### Following commands are only used to allow CLion work properly

include_directories(dev dev/control dev/info_interaction)
file(GLOB_RECURSE USER_SRC "dev/*.c" "dev/control/*.c" "dev/info_interaction/*.c")

# include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32f4xx.mk
file(GLOB_RECURSE STARTUP_SRC "os/common/startup/ARMCMx/compilers/GCC/crt1.c" "os/common/startup/ARMCMx/compilers/GCC/vectors.c")
include_directories(os/common/startup/ARMCMx/compilers/GCC)
include_directories(os/common/startup/ARMCMx/devices/STM32F4xx)
include_directories(os/common/ext/CMSIS/include)
include_directories(os/common/ext/CMSIS/ST/STM32F4xx)

SET(LINKER_SCRIPT os/common/startup/ARMCMx/compilers/GCC/ld/STM32F429xI.ld)

# include $(CHIBIOS)/os/hal/hal.mk
file(GLOB_RECURSE HAL_SRC "os/hal/src/*.c")
include_directories(os/hal/include)

#include $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
file(GLOB_RECURSE PLATFORM_SRC
        "os/hal/ports/common/ARMCMx/nvic.c          "
        "os/hal/ports/STM32/STM32F4xx/hal_lld.c     "
        "os/hal/ports/STM32/STM32F4xx/hal_ext_lld_isr.c "
        "os/hal/ports/STM32/LLD/ADCv2/hal_adc_lld.c     "
        "os/hal/ports/STM32/LLD/CANv1/hal_can_lld.c     "
        "os/hal/ports/STM32/LLD/EXTIv1/hal_ext_lld.c    "
        "os/hal/ports/STM32/LLD/DACv1/hal_dac_lld.c     "
        "os/hal/ports/STM32/LLD/DMAv2/stm32_dma.c   "
        "os/hal/ports/STM32/LLD/GPIOv2/hal_pal_lld.c    "
        "os/hal/ports/STM32/LLD/I2Cv1/hal_i2c_lld.c     "
        "os/hal/ports/STM32/LLD/MACv1/hal_mac_lld.c     "
        "os/hal/ports/STM32/LLD/OTGv1/hal_usb_lld.c     "
        "os/hal/ports/STM32/LLD/RTCv2/hal_rtc_lld.c     "
        "os/hal/ports/STM32/LLD/SDIOv1/hal_sdc_lld.c    "
        "os/hal/ports/STM32/LLD/SPIv1/hal_i2s_lld.c     "
        "os/hal/ports/STM32/LLD/SPIv1/hal_spi_lld.c     "
        "os/hal/ports/STM32/LLD/TIMv1/hal_gpt_lld.c     "
        "os/hal/ports/STM32/LLD/TIMv1/hal_icu_lld.c     "
        "os/hal/ports/STM32/LLD/TIMv1/hal_pwm_lld.c     "
        "os/hal/ports/STM32/LLD/TIMv1/hal_st_lld.c      "
        "os/hal/ports/STM32/LLD/USARTv1/hal_serial_lld.c"
        "os/hal/ports/STM32/LLD/USARTv1/hal_uart_lld.c  "
        "os/hal/ports/STM32/LLD/xWDGv1/hal_wdg_lld.c    "
        )
include_directories(
        os/hal/ports/common/ARMCMx
        os/hal/ports/STM32/STM32F4xx
        os/hal/ports/STM32/LLD/ADCv2
        os/hal/ports/STM32/LLD/CANv1
        os/hal/ports/STM32/LLD/DACv1
        os/hal/ports/STM32/LLD/DMAv2
        os/hal/ports/STM32/LLD/EXTIv1
        os/hal/ports/STM32/LLD/GPIOv2
        os/hal/ports/STM32/LLD/I2Cv1
        os/hal/ports/STM32/LLD/MACv1
        os/hal/ports/STM32/LLD/OTGv1
        os/hal/ports/STM32/LLD/RTCv2
        os/hal/ports/STM32/LLD/SDIOv1
        os/hal/ports/STM32/LLD/SPIv1
        os/hal/ports/STM32/LLD/TIMv1
        os/hal/ports/STM32/LLD/USARTv1
        os/hal/ports/STM32/LLD/xWDGv1
        os/license
        os/common/oslib/include
)

# include $(CHIBIOS)/board/board.mk
file(GLOB_RECURSE BOARD_SRC "board/board.c")
include_directories(board)

# include $(CHIBIOS)/os/hal/osal/rt/osal.mk
file(GLOB_RECURSE OSAL_SRC "os/hal/osal/rt/osal.c")
include_directories(os/hal/osal/rt)

# include $(CHIBIOS)/os/rt/rt.mk
file(GLOB_RECURSE RTOS_SRC "os/rt/src/*.c")
include_directories(os/rt/include)

# include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk
file(GLOB_RECURSE PORT_SRC "os/common/ports/ARMCMx/chcore.c" "os/common/ports/ARMCMx/chcore_v7m.c")
include_directories(os/common/ports/ARMCMx os/common/ports/ARMCMx/compilers/GCC)

# include Shell
file(GLOB_RECURSE SHELL_SRC "os/various/shell.c" "os/various/evtimer.c" "os/various/syscalls.c"
        "os/hal/lib/streams/memstreams.c" "os/hal/lib/streams/chprintf.c")
include_directories(os/various os/hal/lib/streams)

add_library(USER ${USER_SRC} dev/control/arm.c)
add_library(STARTUP ${STARTUP_SRC} os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.s)
add_library(HAL ${HAL_SRC})
add_library(PLATFORM ${PLATFORM_SRC})
add_library(BOARD ${BOARD_SRC})
add_library(OSAL ${OSAL_SRC})
add_library(RTOS ${RTOS_SRC})
add_library(PORT ${PORT_SRC} os/common/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.s)
add_library(SHELL ${SHELL_SRC})