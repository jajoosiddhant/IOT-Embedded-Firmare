/*
 * gpio.c
 *
 *  Created on: Nov 16, 2018
 *      Author: user1
 */

#include "gpio.h"
#include "letimer.h"


void gpio_cmu_init()
{
	// CLOCK Enable for CLock tree
	CMU_ClockEnable(cmuClock_LFA, true);

	CMU_ClockEnable(cmuClock_GPIO,true);
}

void push_buttons_init(void)
{
	// configure pushbutton PB0 and PB1 as inputs, with pull-up enabled
	GPIO_PinModeSet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN, gpioModeInputPull, 1);
	GPIO_PinModeSet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN, gpioModeInputPull, 1);
}

void gpio_irq_init()
{
	GPIO_PinModeSet(MOTION_PORT, MOTION_PIN, gpioModeInputPull, MOTION_DEFAULT);
	/*Clear all the Gpio interrupts Flags*/
	GPIO->IFC = 0x00000000;

	/* Configure and Enable GPIO Interrupt For MOTION SENSOR*/
	GPIO_IntConfig(MOTION_PORT,MOTION_PIN,RISING_EDGE,FALLING_EDGE,INTERRUPT_ENABLE);

	/* Configure and Enable GPIO Interrupt For Push Button 0*/
	//GPIO_IntConfig(PB0_port,PB0_pin,rising_edge,falling_edge,interrupt_enable);

	/*Enable NVIC interrupt*/
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/*Enable NVIC interrupt*/
	//NVIC_EnableIRQ(GPIO_EVEN_IRQn);

}

void led_init()
{
	GPIO_DriveStrengthSet(LED1_PORT, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED1_PORT, gpioDriveStrengthStrongAlternateStrong);

	GPIO_PinModeSet(LED0_PORT,LED0_PIN, gpioModePushPull, LED0_DEFAULT);
	GPIO_PinModeSet(LED1_PORT,LED1_PIN, gpioModePushPull, LED1_DEFAULT);

}


void GPIO_ODD_IRQHandler(void)
{
	/*Disable all interrupts*/
		CORE_ATOMIC_IRQ_DISABLE();

		/*Clear all Interrupt flags*/
		GPIO->IFC = 0x00000000;

		/*Update external event Scheduler Values*/
		EXT_EVENT |= MOTION_EVENT;

		gecko_external_signal(EXT_EVENT);

		/*Disable all Interrupts*/
		GPIO->IEN = 0x00000000;

		/*Disable all interrupts*/
		CORE_ATOMIC_IRQ_ENABLE();
}
