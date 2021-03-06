#This file is generated by VisualGDB.
#It contains GCC settings automatically derived from the board support package (BSP).
#DO NOT EDIT MANUALLY. THE FILE WILL BE OVERWRITTEN. 
#Use VisualGDB Project Properties dialog or modify Makefile or per-configuration .mak files instead.

#VisualGDB provides BSP_ROOT via environment when running Make. The line below will only be active if GNU Make is started manually.
BSP_ROOT ?= C:/Users/Anthony/AppData/Local/VisualGDB/EmbeddedBSPs/arm-eabi/com.sysprogs.arm.stm32
TOOLCHAIN_ROOT := C:/gcc/4.8-2014q2

#Embedded toolchain
CC := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc.exe
CXX := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-g++.exe
LD := $(CXX)
AR := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-ar.exe
OBJCOPY := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy.exe

#Additional flags
PREPROCESSOR_MACROS += ARM_MATH_CM4 stm32_flash_layout STM32F427IG STM32F427xx __FPU_PRESENT
INCLUDE_DIRS += . ../Features ../Configuration/Inc ../Drivers/STM32F4xxxx-HAL/CMSIS/Device/ST/STM32F4xx/Include ../Drivers/STM32F4xxxx-HAL/CMSIS/Include ../Drivers/STM32F4xxxx-HAL/CMSIS/RTOS ../Drivers/STM32F4xxxx-HAL/STM32F4xx_HAL_Driver/Inc
LIBRARY_DIRS += 
LIBRARY_NAMES += 
ADDITIONAL_LINKER_INPUTS += 
MACOS_FRAMEWORKS += 
LINUX_PACKAGES += 

CFLAGS += -mcpu=cortex-m4 -mthumb
CXXFLAGS += -mcpu=cortex-m4 -mthumb
ASFLAGS += -mcpu=cortex-m4 -mthumb
LDFLAGS += -mcpu=cortex-m4 -mthumb  -T../Drivers/STM32F4xxxx-HAL/LinkerScripts/STM32F427xG_flash.lds
COMMONFLAGS +=  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c99 -Werror

