/*
 * tempsensor.h
 *
 *  Created on: Sep 19, 2018
 *      Author: user1
 */

#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#define threshold_temp 15		// Set Threshold Temperature


#endif /* TEMPSENSOR_H_ */

#include "em_i2c.h"
#include "i2cspm.h"
#include "tempsens.h"
#include "src/gpio.h"
#include "src/i2c.h"


void tempsense (void);

