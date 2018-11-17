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


#define LED0_PORT			gpioPortF
#define LED0_PIN			(4)
#define LED0_DEFAULT		false

#define LED1_PORT			gpioPortF
#define LED1_PIN			(5)
#define LED1_DEFAULT		false

#define MOTION_PORT			gpioPortA
#define MOTION_PIN			3
#define MOTION_DEFAULT		true

#define RISING_EDGE			(0)
#define FALLING_EDGE 		(1)
#define INTERRUPT_ENABLE	(1)
#define MOTION_EVENT        (0x02)

void gpio_cmu_init(void);
void gpio_irq_init(void);
void led_init(void);
void GPIO_ODD_IRQHandler(void);
void push_buttons_init(void);

#endif /* GPIO_H_ */
