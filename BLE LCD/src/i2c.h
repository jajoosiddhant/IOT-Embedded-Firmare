/*
 * i2c.h
 *
 *  Created on: Sep 18, 2018
 *      Author: user1
 */

#include "main.h"
#include "em_i2c.h"
#include "src/gpio.h"
#include "src/letimer.h"

#ifndef I2C_H_
#define I2C_H_

int temp;

#endif /* I2C_H_ */

void i2c_init(void);
void i2c_disable (void);
uint16_t i2c_driver (void);
void I2C0_IRQHandler (void);
void sensor_enable(void);


