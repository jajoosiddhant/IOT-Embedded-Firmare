/*
 * tempsensor.c
 *
 *  Created on: Sep 19, 2018
 *      Author: user1
 */

#include "infrastructure.h"
#include "native_gecko.h"
#include "lcd_driver.h"
#include "src/tempsensor.h"
#include "src/main.h"

#define UINT32_TO_FLT(data) (((float)((int32_t)( data) & 0x00FFFFFFU)) * (float)pow(10, ((int32_t)(data) >> 24)))


void tempsense ()
{
	char temp_lcd[16];
	uint32_t out= i2c_driver();

	uint8_t tempbuff[10]; /* Stores the temperature data in the Health Thermometer (HTM) format. */
	  uint8_t flags = 0x00;   /* HTM flags set as 0 for Celsius.*/
	  uint8_t *p = tempbuff; /*Pointer to HTM temperature buffer needed for converting values to bitstream. */

	  /* Convert flags to bitstream */
	  		  UINT8_TO_BITSTREAM(p, flags);

		float tempcelsius = (float)((175.72 * out)/65536)-46.85 ;
		if ( tempcelsius > threshold_temp )
		{
			GPIO_PinOutClear( LED1_port , LED1_pin );
		}
		else
		{
			GPIO_PinOutSet( LED1_port , LED1_pin );
		}


		  /* Convert sensor data to correct temperature format */
		      uint32_t bttemp = (uint32_t)FLT_TO_UINT32(tempcelsius*1000, -3);

		      UINT32_TO_BITSTREAM(p, bttemp);
		      if (a==1)
		      {
		      gecko_cmd_gatt_server_send_characteristic_notification(
		            0xFF, gattdb_temperature_measurement, 5, tempbuff);
		      }
		      gecko_cmd_le_connection_get_rssi(con);

		     // double converted_temp_lcd = (double) UINT32_TO_FLT(out);
		      sprintf(temp_lcd,"Temperature:%2.2f ",tempcelsius);
		      LCD_write(temp_lcd,LCD_ROW_CLIENTADDR);

}
