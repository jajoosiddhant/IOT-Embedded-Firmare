/***************************************************************************//**
 * @file sleep.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/


/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* Board headers */
#include "init_mcu.h"
//#include "init_mcu.c"
#include "init_board.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header  */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;


//***********************************************************************************
// Include files
//***********************************************************************************

#include "src/i2c.h"
#include "src/main.h"
#include "src/gpio.h"
#include "src/cmu.h"
#include "src/letimer.h"
#include "src/tempsensor.h"
#include "src/timer.h"
#include "em_letimer.h"
#include "lcd_driver.h"

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


//***********************************************************************************
// main
//***********************************************************************************

/**
 * @brief  Main function
 */
int main(void)
{
	/* Initialize device */
	initMcu();

	/* Initialize board */
	initBoard();

	/* Initialize clocks*/
	cmu_init();

	/* Initialize GPIO */
	gpio_init();

	/* Initialize LETIMER0*/
	letimer_init();

	i2c_init();

	/* Initialize TIMER0*/
	time_init();

	/*Initialize stack*/
	gecko_init(&config);


	a=0;
	ext_event=0;

  while (1) {
      /* Event pointer for handling events */
      struct gecko_cmd_packet* evt;

      /* Check for stack event. */
      evt = gecko_wait_event();

      /* Handle events */
      switch (BGLIB_MSG_ID(evt->header)) {
        /* This boot event is generated when the system boots up after reset.
         * Do not call any stack commands before receiving the boot event.
         * Here the system is set to start advertising immediately after boot procedure. */
        case gecko_evt_system_boot_id:

        	gecko_cmd_system_set_tx_power(0);
        	LCD_init("BLE SERVER");

        	struct gecko_msg_system_get_bt_address_rsp_t *bt_addr = gecko_cmd_system_get_bt_address();
        	bd_addr *server_address = &bt_addr->address;

        	sprintf(address_string,"%x:%x:%x:%x:%x:%x", server_address->addr[5],server_address->addr[4],server_address->addr[3],server_address->addr[2],server_address->addr[1],server_address->addr[0]);
        	//LCD_write("BLUETOOTH ADDRESS",LCD_ROW_BTADDR1);
        	LCD_write(address_string,LCD_ROW_BTADDR1);

          /*Delete all previous bondings*/
          gecko_cmd_sm_delete_bondings();

          /* Configure Security*/
          gecko_cmd_sm_configure(security_configure_flag,sm_io_capability_displayyesno);

          /*Set into Bondable Mode*/
          gecko_cmd_sm_set_bondable_mode(1);

          gecko_cmd_le_gap_set_advertise_timing(adv_handle, adv_interval_min, adv_interval_max, adv_timing_duration, adv_maxevents);

          /* Start general advertising and enable connections. */
          gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
          break;



        /* This event is generated when a connected client has either
         * 1) changed a Characteristic Client Configuration, meaning that they have enabled
         * or disabled Notifications or Indications, or
         * 2) sent a confirmation upon a successful reception of the indication. */
        case gecko_evt_gatt_server_characteristic_status_id:

          if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement)
              && (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01)) {
            if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x02) {

            	 //gecko_cmd_le_connection_get_rssi(evt->data.evt_gatt_server_characteristic_status.connection);

          }
          }
          break;

        case gecko_evt_le_connection_opened_id:

        	/*Configure Connection Parameters*/
        	gecko_cmd_le_connection_set_parameters(evt->data.evt_le_connection_opened.connection, con_interval_min,con_interval_max,con_latency,con_timeout);
        	con = evt->data.evt_le_connection_opened.connection;
        	LCD_write( "CONNECTED" , LCD_ROW_BTADDR2 );


        	gecko_cmd_sm_increase_security(con);


        	break;

        /*case gecko_evt_sm_passkey_display_id:


        	sprintf(pass_word,"%d",evt->data.evt_sm_passkey_display);
        	LCD_write( pass_word , LCD_ROW_PASSKEY );
        	LCD_write( pass_word , LCD_ROW_PASSKEY );

        	break; */

        case gecko_evt_sm_confirm_passkey_id:

        	bond_con = evt->data.evt_sm_confirm_bonding.connection;
        	sprintf(pass_word,"%lu",evt->data.evt_sm_passkey_display.passkey);
        	LCD_write( pass_word , LCD_ROW_PASSKEY );
        	LCD_write( "Press PB1 to confirm" , LCD_ROW_ACTION );

        	/*Confirm passkey by pressing the PB1 button*/
        	/*GPIO interrupt called on pressing PB1 button*/

        	break;

       case gecko_evt_sm_bonded_id:

        	a=1;  						/*Value set to 1 to only send temperature if bonded*/
    	   LCD_write( "BONDED" , LCD_ROW_MAX );

        	break;

       case gecko_evt_sm_bonding_failed_id:
    	   LCD_write( "BONDING FAILED" , LCD_ROW_MAX );
    	   break;

        /*case gecko_evt_sm_confirm_bonding_id:
        	gecko_cmd_sm_passkey_confirm(evt->data.evt_sm_confirm_bonding.connection, 1);

        	        	LCD_write( "CONNECTED" , LCD_ROW_CLIENTADDR );

		gecko_cmd_sm_bonding_confirm(con , 1);
		LCD_write( "CONNECTED" , LCD_ROW_CLIENTADDR );

        break;*/


        case gecko_evt_le_connection_closed_id:
          /* Check if need to boot to dfu mode */
        	LCD_write( "DISCONNECTED" , LCD_ROW_BTADDR2 );
        	gecko_cmd_system_set_tx_power(0);
          if (boot_to_dfu) {
            /* Enter to DFU OTA mode */
            gecko_cmd_system_reset(2);

          	} else {

            /* Stop timer in case client disconnected before indications were turned off */
            gecko_cmd_hardware_set_soft_timer(0, 0, 0);
            /* Restart advertising after client has disconnected */
            gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
          }

          break;

        case gecko_evt_system_external_signal_id:

        	if (evt->data.evt_system_external_signal.extsignals & letimer_event)
        	{
        		CORE_ATOMIC_IRQ_DISABLE();
        		ext_event &= ~letimer_event;
        		CORE_ATOMIC_IRQ_ENABLE();

        		i2c_init();
        		sensor_enable();
        		//for (int i=0;i<250000;i++);
        		tempsense();
        		i2c_disable();

        	}


        	if (evt->data.evt_system_external_signal.extsignals & timer_event)
        	{
        		CORE_ATOMIC_IRQ_DISABLE();
        		ext_event &= ~timer_event;
        		CORE_ATOMIC_IRQ_ENABLE();

        		/*Set the Counter Value*/
        		TIMER_CounterSet(TIMER0,0x00);
        		//tempsense();
    		    //i2c_disable();



        	}

        	if (evt->data.evt_system_external_signal.extsignals & gpio_event)
        	{
        		CORE_ATOMIC_IRQ_DISABLE();
        		ext_event &= ~gpio_event;
        		CORE_ATOMIC_IRQ_ENABLE();

        		/*Confirm Passkey*/
        		gecko_cmd_sm_passkey_confirm( bond_con, 1);
        		LCD_write("PASSKEY CONFIRMED",LCD_ROW_ACTION );
        		LCD_write( "" , LCD_ROW_PASSKEY );

        	}

        	break;

        case gecko_evt_le_connection_rssi_id:

        	txpowe = 0;
        	rvalue = evt->data.evt_le_connection_rssi.rssi;
        	if (rvalue > -35 )
        	{
        		txpowe = -260;	//setting TX power to min (-26 dbm) when rssi more than -35 dB
       	  	}
        	else if (rvalue > -45 && rvalue <=(-35))
        	{
        		txpowe = -200;
        	}
        	else if (rvalue > -55 && rvalue <=(-45))
        	{
        		txpowe = -150;
        	}
        	else if (rvalue >-65 && rvalue<= (-55))
        	{
        		txpowe = -50;
        	}else if (rvalue >-75 && rvalue <=(-65))
        	{
        		txpowe = 0;
        	}
        	else if (rvalue >-85 && rvalue <= (-75))
        	{
        		txpowe = 50;
        	}else
        	{
        		txpowe = 80; //setting TX power to max (8 dbm) when rssi less than -85 dB
        	}


        	gecko_cmd_system_halt(1);
        	gecko_cmd_system_set_tx_power(txpowe);
        	gecko_cmd_system_halt(0);
        	break;



        /* Events related to OTA upgrading
           ----------------------------------------------------------------------------- */

        /* Checks if the user-type OTA Control Characteristic was written.
         * If written, boots the device into Device Firmware Upgrade (DFU) mode. */
        case gecko_evt_gatt_server_user_write_request_id:
          if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
            /* Set flag to enter to OTA mode */
            boot_to_dfu = 1;
            /* Send response to Write Request */
            gecko_cmd_gatt_server_send_user_write_response(
              evt->data.evt_gatt_server_user_write_request.connection,
              gattdb_ota_control,
              bg_err_success);

            /* Close connection to enter to DFU OTA mode */
            gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
          }
          break;

        default:
          break;
      }
    }
  }
/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
