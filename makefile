# We try to detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
    TARGET_TEST_EXTENSION=.exe
    LD_WRAP=true
else
    TARGET_TEST_EXTENSION=.out
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LD_WRAP=true
    endif
    ifeq ($(UNAME_S),Darwin)
        LD_WRAP=false
    endif
endif

# Location of dependencies
UNITY_ROOT = ../Unity
RTOS_ROOT = ../FreeRTOS
STM32CUBE_ROOT = ../STM32Cube_F4

TARGET_BASE = lab5_sysid
TARGET_ELF = bin/$(TARGET_BASE).elf
TARGET_HEX = bin/$(TARGET_BASE).hex

TARGET_TEST_BASE = test_$(TARGET_BASE)
TARGET_TEST = test/bin/$(TARGET_TEST_BASE)$(TARGET_TEST_EXTENSION)

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld 
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump

DEFS = -DSTM32F411xE

OPTIMIZE = -Os

# Fixes the 'exit()' compile error due to existing system environment 'exit()' function
EXIT_FIX = --specs=nosys.specs

# Use Newlib-nano runtime library - produces much smaller code by using libraries optimised for embedded systems when including functions such as printf()
USE_NANO = \
	--specs=nano.specs \
	-u _printf_float \
	-u _scanf_float

WRAP_PRINTF = -Wl,-wrap,printf -fno-builtin-printf
	
MCU = cortex-m4
MCFLAGS = -mcpu=$(MCU) -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork
MATH_STM = -lm

CFLAGS  = -std=c99
CFLAGS += $(DEFS)
CFLAGS += -Wno-comment
CFLAGS += -Wno-parentheses-equality
CFLAGS += -Wno-int-to-pointer-cast

SRC_COMMON = \
	src/cmd_line_buffer.c \
	src/cmd_parser.c \
	src/controller.c \
	src/light.c \
	src/encoder.c \
	src/potentiometer.c \
	src/led_rgb.c \
	src/button.c \
	src/dimmer_task.c \
	src/heartbeat_task.c \
	src/stm32f4xx_hal_msp.c \
	src/sin_table.c \
	src/log_data.c \
	src/stepper_task.c


SRC_STM = \
	$(SRC_COMMON) \
	$(SRC_RTOS) \
	src/main.c \
	src/uart.c \
	src/system_stm32f4xx.c \
	src/stm32f4xx_it.c \
	src/startup_stm32f411xe.s \
	src/cmd_task.c


# $(STM32CUBE_ROOT)/Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_sin_f32.c


SRC_TEST = \
	$(UNITY_ROOT)/src/unity.c \
	$(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
	$(SRC_COMMON) \
	mock/stm32f4xx_hal_gpio.c \
	mock/stm32f4xx_hal_tim.c \
	mock/stm32f4xx_hal_cortex.c \
	mock/stm32f4xx_hal_rcc.c \
	mock/stm32f4xx_hal_adc.c \
	mock/cmsis_os.c \
	test/src/all_tests.c \
	test/src/test_led_rgb.c \
	test/src/test_encoder.c \
	test/src/test_potentiometer.c \
	test/src/test_light.c \
	test/src/test_button.c \
	test/src/test_cmd_line_buffer.c \
	test/src/iospy.c \
	test/src/test_iospy.c \
	test/src/test_cmd_process.c \
	test/src/test_cmd_parse.c \
	test/src/test_cmd_enc.c \
	test/src/test_cmd_pot.c \
	test/src/test_cmd_light.c \
	test/src/test_cmd_dimmer.c \
	test/src/test_cmd_heartbeat.c \
	test/src/test_cmd_xy.c \
	test/src/test_controller.c \
	test/src/test_cmd_controller.c \
	test/src/test_sin_table.c


SRC_STM_CUBE = \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
	$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c


SRC_RTOS = \
	$(RTOS_ROOT)/croutine.c \
	$(RTOS_ROOT)/event_groups.c \
	$(RTOS_ROOT)/list.c \
	$(RTOS_ROOT)/port.c \
	$(RTOS_ROOT)/queue.c \
	$(RTOS_ROOT)/tasks.c \
	$(RTOS_ROOT)/timers.c \
	$(RTOS_ROOT)/heap_useNewlib.c \
	$(RTOS_ROOT)/cmsis_os.c


INC_RTOS = \
	-I../FreeRTOS/

INC_COMMON = \
	-Isrc

INC_STM_CUBE = \
	-I$(STM32CUBE_ROOT)/Drivers/STM32F4xx_HAL_Driver/Inc/ \
	-I$(STM32CUBE_ROOT)/Drivers/CMSIS/Include/ \
	-I$(STM32CUBE_ROOT)/Drivers/CMSIS/Device/ST/STM32F4xx/Include/


INC_STM = \
	$(INC_COMMON) \
	$(INC_STM_CUBE)


INC_TEST = \
	$(INC_COMMON) \
	-I$(UNITY_ROOT)/src \
	-I$(UNITY_ROOT)/extras/fixture/src \
	-Imock

MISC_TEST = -DUNITY_FLOAT_PRECISION=0.0001f
ifeq ($(LD_WRAP),true)
	MISC_TEST += -Wl,-wrap,cmd_parse
else
	MISC_TEST += -DNO_LD_WRAP
endif

all: clean default

test: clean_test test_only
	- ./$(TARGET_TEST) -v

test_only:
	gcc -g -O0 $(CFLAGS) $(INC_TEST) $(SRC_TEST) -o $(TARGET_TEST) $(MISC_TEST)

clean_test:
	rm -f $(TARGET_TEST)

default: test_only target_only
	- ./$(TARGET_TEST)

target_only:
	$(CC) $(CFLAGS) $(WRAP_PRINTF) $(OPTIMIZE) -Wl,-T,STM32F411RETx_FLASH.ld $(EXIT_FIX) $(MCFLAGS) $(USE_NANO) $(INC_STM) $(INC_RTOS) $(SRC_STM) $(SRC_STM_CUBE) $(STARTUP) $(MATH_STM) -o $(TARGET_ELF) 
	$(CP) -O ihex $(TARGET_ELF) $(TARGET_HEX)
	arm-none-eabi-size $(TARGET_ELF)

clean_stm:
	rm -f $(TARGET_ELF) $(TARGET_HEX)

clean: clean_test clean_stm

program: clean_stm target_only
	openocd -f interface/stlink-v2-1.cfg -f target/stm32f4x.cfg -c "program $(TARGET_HEX) verify reset exit"
