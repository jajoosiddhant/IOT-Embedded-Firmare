/*
 * tempsensor.c
 *
 *  Created on: Sep 19, 2018
 *      Author: user1
 */

#include "src/tempsensor.h"

void tempsense ()
{
	uint16_t out= i2c_driver();

		float tempcelsius = ((175.72 * out)/65536)-46.85 ;
		if ( tempcelsius > threshold_temp )
		{
			GPIO_PinOutClear( LED1_port , LED1_pin );
		}
		else
		{
			GPIO_PinOutSet( LED1_port , LED1_pin );
		}


}


