################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BG_LCD/lcdGraphics/dmd/dmd_display.c 

OBJS += \
./BG_LCD/lcdGraphics/dmd/dmd_display.o 

C_DEPS += \
./BG_LCD/lcdGraphics/dmd/dmd_display.d 


# Each subdirectory must supply rules for building sources it contributes
BG_LCD/lcdGraphics/dmd/dmd_display.o: ../BG_LCD/lcdGraphics/dmd/dmd_display.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFR32BG13P632F512GM48=1' '-D__HEAP_SIZE=0x1200' '-DMESH_LIB_NATIVE=1' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x1000' -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\hardware\kit\common\drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\Device\SiliconLabs\EFR32BG13P\Source\GCC" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\CMSIS\Include" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\emlib\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\emlib\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\protocol\bluetooth\bt_mesh\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\protocol\bluetooth\bt_mesh\inc\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\hardware\kit\common\halconfig" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\protocol\bluetooth\bt_mesh\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\protocol\bluetooth\bt_mesh\inc\soc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\Device\SiliconLabs\EFR32BG13P\Source" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\hardware\kit\common\bsp" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\radio\rail_lib\common" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\emdrv\sleep\src" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\emdrv\uartdrv\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\bootloader\api" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\emdrv\sleep\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\hardware\kit\EFR32BG13_BRD4104A\config" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\radio\rail_lib\chip\efr32" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\emdrv\common\inc" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\platform\halconfig\inc\hal-config" -I"C:/SiliconLabsnew/developer/sdks/blemesh/v1.3//platform/middleware/glib/glib" -I"C:/SiliconLabsnew/developer/sdks/blemesh/v1.3//platform/middleware/glib" -I"C:/SiliconLabsnew/developer/sdks/blemesh/v1.3//platform/middleware/glib/dmd" -I"C:/SiliconLabsnew/developer/sdks/blemesh/v1.3//util/silicon_labs/silabs_core/graphics" -I"C:/SiliconLabsnew/developer/sdks/blemesh/v1.3//hardware/kit/common/drivers" -I"C:\Users\user1\SimplicityStudio\v4_workspace\soc-btmesh-empty\BG_LCD" -I/soc-btmesh-empty/hardware/kit/common/halconfig/halconfig.h -Os -fno-builtin -flto -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"BG_LCD/lcdGraphics/dmd/dmd_display.d" -MT"BG_LCD/lcdGraphics/dmd/dmd_display.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


