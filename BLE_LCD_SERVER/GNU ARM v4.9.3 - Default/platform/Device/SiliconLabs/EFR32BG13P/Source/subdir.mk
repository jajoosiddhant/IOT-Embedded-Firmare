################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.c 

OBJS += \
./platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.o 

C_DEPS += \
./platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.d 


# Each subdirectory must supply rules for building sources it contributes
platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.o: ../platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-D__HEAP_SIZE=0xD00' '-D__StackLimit=0x20000000' '-DEFR32BG13P632F512GM48=1' -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\CMSIS\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\radio\rail_lib\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emlib\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emlib\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\hardware\kit\common\bsp" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\hardware\kit\common\drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\bootloader\api" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emdrv\sleep\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\protocol\bluetooth\ble_stack\inc\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\hardware\kit\common\halconfig" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emdrv\common\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\protocol\bluetooth\ble_stack\inc\soc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\halconfig\inc\hal-config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emdrv\sleep\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\radio\rail_lib\chip\efr32" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\app\bluetooth\common\stack_bridge" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\bootloader" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//platform/middleware/glib/dmd" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//util/silicon_labs/silabs_core/graphics" -I"C:/SiliconLabsnew/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\lcdGraphics" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emdrv\sleep" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emdrv\uartdrv" -I"C:\Users\user1\SimplicityStudio\v4_workspace\BLE_LCD_SERVER\platform\emdrv\uartdrv\inc" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.d" -MT"platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


