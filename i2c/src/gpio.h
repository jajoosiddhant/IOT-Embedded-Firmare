//***********************************************************************************
// Include files
//***********************************************************************************
#include "main.h"
#include "em_gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************

//Defining SDA and SCL lines
#define sdaport gpioPortC
#define sdapin 11
#define sclport gpioPortC
#define sclpin 10
#define sensorport gpioPortD
#define sensorpin 15

// LED0 pin is
#define	LED0_port gpioPortF
#define LED0_pin 4
#define LED0_default	false 	// off

// LED1 pin is
#define LED1_port gpioPortF
#define LED1_pin 5
#define LED1_default	false	// off

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_init(void);
void GPIO_EVEN_IRQHandler(void);
void GPIO_ODD_IRQHandler(void);

