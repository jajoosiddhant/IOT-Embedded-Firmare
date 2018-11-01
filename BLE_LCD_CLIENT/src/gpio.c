//***********************************************************************************
// Include files
//***********************************************************************************
#include "src/gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************
void gpio_init(void){


	/*Configure Push Button 1*/
	GPIO_PinModeSet( PB1_port , PB1_pin , gpioModeInputPull , 1 );

	/*Configure Push Button 0*/
	GPIO_PinModeSet( PB0_port , PB0_pin , gpioModeInputPull , 1 );

	/* Set LED ports to be standard output drive with default off (cleared)*/
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);


	/*Clear all the Gpio interrupts Flags*/
	GPIO->IFC = 0x00000000;

	/* Configure and Enable GPIO Interrupt For Push Button 1*/
	GPIO_IntConfig(PB1_port,PB1_pin,rising_edge,falling_edge,interrupt_enable);

	/* Configure and Enable GPIO Interrupt For Push Button 0*/
	//GPIO_IntConfig(PB0_port,PB0_pin,rising_edge,falling_edge,interrupt_enable);

	/*Enable NVIC interrupt*/
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

	/*Enable NVIC interrupt*/
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}


void GPIO_ODD_IRQHandler(void)
{
	/*Disable all interrupts*/
		CORE_ATOMIC_IRQ_DISABLE();

		/*Clear all Interrupt flags*/
		GPIO->IFC = 0x00000000;

		/*Update external event Scheduler Values*/
		ext_event |= gpio_PB1_event;

		gecko_external_signal(ext_event);

		/*Disable all Interrupts*/
		GPIO->IEN = 0x00000000;

		/*Disable all interrupts*/
		CORE_ATOMIC_IRQ_ENABLE();
}

void GPIO_EVEN_IRQHandler(void)
{
	/*Disable all interrupts*/
		CORE_ATOMIC_IRQ_DISABLE();

		/*Clear all Interrupt flags*/
		GPIO->IFC = 0x00000000;

		/*Update external event Scheduler Values*/
		ext_event |= gpio_PB0_event;

		gecko_external_signal(ext_event);

		/*Disable all Interrupts*/
		GPIO->IEN = 0x00000000;

		/*Disable all interrupts*/
		CORE_ATOMIC_IRQ_ENABLE();
}


