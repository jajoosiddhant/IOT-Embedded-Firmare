/*
 * gpio.h
 *
 *  Created on: Nov 16, 2018
 *      Author: user1
 */


#ifndef GPIO_H_
#define GPIO_H_

#include "em_gpio.h"
#include "em_cmu.h"
#include "hal-config.h"
#include "native_gecko.h"
#include "em_core.h"

/*
 * Defining SDA and SCL lines
 */
#define SDAPORT 			gpioPortC
#define SDAPIN				(11)
#define SCLPORT 			gpioPortC
#define SCLPIN				(10)
#define SENSORPORT 			gpioPortD
#define SENSORPIN			(15)

/*
 * Defininf LEDS,SENSOR port and pins
 */
#define LED0_PORT			gpioPortF
#define LED0_PIN			(4)
#define LED0_DEFAULT		false
#define LED1_PORT			gpioPortF
#define LED1_PIN			(5)
#define LED1_DEFAULT		false
#define MOTION_PORT			gpioPortA
#define MOTION_PIN			(3)
#define MOTION_DEFAULT		true
#define PROXIMITY_PORT		gpioPortD
#define PROXIMITY_PIN		(10)
#define PROXIMITY_DEFAULT	true

/*
 * Parameters for configuring gpio interrupts for sensors
 */
#define RISING_EDGE			(0)
#define FALLING_EDGE 		(1)
#define INTERRUPT_ENABLE	(1)

/*
 * Scheduler Event Flags
 */
#define MOTION_EVENT        (0x02)
#define PROXIMITY_EVENT		(0X04)

/**
 *  State of the LEDs is updated by calling LED_set_state().
 *  The new state is passed as parameter, possible values are defined below.
 */
#define LED_STATE_OFF    	0   /* light off (both LEDs turned off)   */
#define LED_STATE_ON     	1   /* light on (both LEDs turned on)     */
#define LED_STATE_RESTORE	2   /* light on (LED0 turned on , LED1 blinking )     */
#define LED_STATE_PROV   	3   /* provisioning (LEDs blinking)       */


void gpio_cmu_init(void);
void gpio_irq_init(void);
void led_init(void);
void GPIO_ODD_IRQHandler(void);
void GPIO_EVEN_IRQHandler(void);
void push_buttons_init(void);
void LED_state(int state);

#endif /* GPIO_H_ */
