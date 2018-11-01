################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cmu.c \
../src/gpio.c \
../src/i2c.c \
../src/letimer.c \
../src/tempsensor.c \
../src/timer.c 

OBJS += \
./src/cmu.o \
./src/gpio.o \
./src/i2c.o \
./src/letimer.o \
./src/tempsensor.o \
./src/timer.o 

C_DEPS += \
./src/cmu.d \
./src/gpio.d \
./src/i2c.d \
./src/letimer.d \
./src/tempsensor.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/cmu.o: ../src/cmu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-D__HEAP_SIZE=0xD00' '-D__StackLimit=0x20000000' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\CMSIS\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\bsp" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader\api" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\halconfig" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\common\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\halconfig\inc\hal-config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\chip\efr32" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\app\bluetooth\common\stack_bridge" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/dmd" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//util/silicon_labs/silabs_core/graphics" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\lcdGraphics" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/cmu.d" -MT"src/cmu.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gpio.o: ../src/gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-D__HEAP_SIZE=0xD00' '-D__StackLimit=0x20000000' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\CMSIS\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\bsp" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader\api" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\halconfig" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\common\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\halconfig\inc\hal-config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\chip\efr32" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\app\bluetooth\common\stack_bridge" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/dmd" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//util/silicon_labs/silabs_core/graphics" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\lcdGraphics" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/gpio.d" -MT"src/gpio.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/i2c.o: ../src/i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-D__HEAP_SIZE=0xD00' '-D__StackLimit=0x20000000' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\CMSIS\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\bsp" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader\api" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\halconfig" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\common\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\halconfig\inc\hal-config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\chip\efr32" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\app\bluetooth\common\stack_bridge" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/dmd" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//util/silicon_labs/silabs_core/graphics" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\lcdGraphics" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/i2c.d" -MT"src/i2c.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/letimer.o: ../src/letimer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-D__HEAP_SIZE=0xD00' '-D__StackLimit=0x20000000' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\CMSIS\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\bsp" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader\api" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\halconfig" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\common\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\halconfig\inc\hal-config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\chip\efr32" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\app\bluetooth\common\stack_bridge" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/dmd" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//util/silicon_labs/silabs_core/graphics" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\lcdGraphics" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/letimer.d" -MT"src/letimer.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/tempsensor.o: ../src/tempsensor.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-D__HEAP_SIZE=0xD00' '-D__StackLimit=0x20000000' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\CMSIS\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\bsp" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader\api" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\halconfig" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\common\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\halconfig\inc\hal-config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\chip\efr32" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\app\bluetooth\common\stack_bridge" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/dmd" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//util/silicon_labs/silabs_core/graphics" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\lcdGraphics" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/tempsensor.d" -MT"src/tempsensor.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/timer.o: ../src/timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-D__HEAP_SIZE=0xD00' '-D__StackLimit=0x20000000' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\CMSIS\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emlib\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\bsp" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader\api" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\common\halconfig" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\common\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\halconfig\inc\hal-config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\radio\rail_lib\chip\efr32" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\app\bluetooth\common\stack_bridge" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\bootloader" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/dmd" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//util/silicon_labs/silabs_core/graphics" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\lcdGraphics" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\sleep" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE LCD\platform\emdrv\uartdrv" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/timer.d" -MT"src/timer.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


