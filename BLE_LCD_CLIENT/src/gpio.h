//***********************************************************************************
// Include files
//***********************************************************************************
#include "main.h"
#include "em_gpio.h"
#include "native_gecko.h"
#include "lcd_driver.h"
#include "em_core.h"

//***********************************************************************************
// defined files
//***********************************************************************************

/*Defining SDA and SCL lines*/
#define sdaport gpioPortC
#define sdapin 11
#define sclport gpioPortC
#define sclpin 10
#define sensorport gpioPortD
#define sensorpin 15

/* LED0 pin is*/
#define	LED0_port gpioPortF
#define LED0_pin 4
#define LED0_default	false 	// off

/* LED1 pin is*/
#define LED1_port gpioPortF
#define LED1_pin 5
#define LED1_default	false	// off

/*Push Button 1*/
#define PB1_port	 gpioPortF
#define PB1_pin 	 7

/*Push Button 1*/
#define PB0_port 	gpioPortF
#define PB0_pin 	6

/*GPIO Interrupt Config*/
#define rising_edge 		false
#define falling_edge 		true
#define interrupt_enable 	true
#define gpio_PB1_event 		0x04
#define gpio_PB0_event 		0x08


//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_init(void);
void GPIO_ODD_IRQHandler(void);
void GPIO_EVEN_IRQHandler(void);

