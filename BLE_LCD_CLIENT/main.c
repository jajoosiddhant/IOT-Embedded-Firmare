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

#include <math.h>
#include "retargetserial.h"
#include "src/main.h"
#include "src/gpio.h"
#include "src/cmu.h"
#include "em_letimer.h"
#include "lcd_driver.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************
const uint8_t thermometer_Service[2] = { 0x09, 0x18 };
const uint8_t thermometer_characteristic[2] = { 0x1c, 0x2a };

//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************
typedef struct{
uint8_t conn_handle;
uint8_t properties;
uint8_t temperature [5];
uint16_t characteristics;
uint16_t descriptors;
uint32_t service_handle;
}att_values;

typedef enum{
disc_services,
disc_characteristics,
disc_descriptors,
enable_indications,
finished
}command;


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

	/*Initialize stack*/
	gecko_init(&config);

	RETARGET_SerialInit();
	RETARGET_SerialCrLf(true);


	ext_event=0;
	att_values att_values;
	command command;



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
        	LCD_init("BLE CLIENT");

        	struct gecko_msg_system_get_bt_address_rsp_t *bt_addr = gecko_cmd_system_get_bt_address();
        	bd_addr *client_address =(bd_addr*)&bt_addr->address;
        	sprintf(client_address_string,"%x:%x:%x:%x:%x:%x", client_address->addr[5],client_address->addr[4],client_address->addr[3],client_address->addr[2],client_address->addr[1],client_address->addr[0]);
        	LCD_write(client_address_string,LCD_ROW_BTADDR1);

        	/*Siddhant board address*/
        	bd_addr server_address_connect = {.addr={0xb6,0x80,0xf1,0x57,0x0b,0x00}};
        	/*Sarthak board address*/
        	//bd_addr server_address_connect = {.addr={0x6e,0xf2,0xb5,0x57,0x0b,0x00}};
        	/*Satya Bluetooth Address*/
        	//bd_addr server_address_connect = {.addr={0xd3,0x7e,0xa9,0x9f,0xfd,0x90}};
        	/*Ayush Bluetooth Address*/
        	//bd_addr server_address_connect = {.addr={0xea,0x2f,0xef,0x57,0x0b,0x00}};

          /*Delete all previous bondings*/
          gecko_cmd_sm_delete_bondings();

          LCD_write("Scanning..", LCD_ROW_CONNECTION);

          /* Configure Security*/
          gecko_cmd_sm_configure(security_configure_flag,sm_io_capability_displayyesno);

          /*Set into Bondable Mode*/
          gecko_cmd_sm_set_bondable_mode(1);

          gecko_cmd_le_gap_set_discovery_timing(s_phy,scanning_interval,scanning_window);

          gecko_cmd_le_gap_set_discovery_type(s_phy, scanning_type);

          gecko_cmd_le_gap_connect(server_address_connect,le_gap_address_type_public, sc_phy );
          break;



        /* This event is generated when a connected client has either
         * 1) changed a Characteristic Client Configuration, meaning that they have enabled
         * or disabled Notifications or Indications, or
         * 2) sent a confirmation upon a successful reception of the indication. */
        case gecko_evt_gatt_server_characteristic_status_id:

          if ((evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement)
              && (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01)) {
            if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 0x02) {

          }
          }
          break;

        case gecko_evt_le_connection_opened_id:

        	/*Configure Connection Parameters*/
        	gecko_cmd_le_connection_set_parameters(evt->data.evt_le_connection_opened.connection, con_interval_min,con_interval_max,con_latency,con_timeout);
        	con = evt->data.evt_le_connection_opened.connection;
        	LCD_write( "CONNECTED" , LCD_ROW_CONNECTION );

        	bd_addr server_address = evt->data.evt_le_connection_opened.address;
        	sprintf(server_address_string,"X:X:X:X:%x:%x",server_address.addr[1],server_address.addr[0]);
        	LCD_write(server_address_string,LCD_ROW_BTADDR2);

        	break;


        case gecko_evt_sm_confirm_bonding_id:

        	bond_con = evt->data.evt_sm_confirm_bonding.connection;
        	LCD_write( "PRESS PB1 TO PAIR" , LCD_ROW_ACTION );

        	/*Press PB1 to confirm Pairing*/
        	/*GPIO Interrupt called on pressing PB1*/

        	break;

        case gecko_evt_sm_confirm_passkey_id:

        	sprintf(pass_word,"%lu",evt->data.evt_sm_passkey_display.passkey);
        	LCD_write( pass_word , LCD_ROW_PASSKEY );
        	LCD_write( "Press PB0 to confirm" , LCD_ROW_ACTION );

        	/*Confirm passkey by pressing the PB1 button*/
        	/*GPIO interrupt called on pressing PB1 button*/

        	break;

       case gecko_evt_sm_bonded_id:

    	   LCD_write( "BONDED" , LCD_ROW_MAX );

    	   gecko_cmd_gatt_discover_primary_services_by_uuid(evt->data.evt_sm_bonded.connection,2
    			   	   	   	   	   	   	   	   	   	   	   	   ,(const uint8_t*)thermometer_Service);
    	   command = disc_characteristics;

    	   break;

       case gecko_evt_sm_bonding_failed_id:
    	   LCD_write( "BONDING FAILED" , LCD_ROW_MAX );
    	   LCD_write( "" , LCD_ROW_PASSKEY );
    	   LCD_write( "" , LCD_ROW_ACTION );

    	   gecko_cmd_le_connection_close(evt->data.evt_sm_bonding_failed.connection);

    	   break;

       case gecko_evt_gatt_service_id:

    	   att_values.conn_handle = evt->data.evt_gatt_service.connection;
    	   att_values.service_handle = evt->data.evt_gatt_service.service;

      		break;

       case gecko_evt_gatt_characteristic_id:

    	   att_values.conn_handle = evt->data.evt_gatt_characteristic.connection;
    	   att_values.characteristics = evt->data.evt_gatt_characteristic.characteristic;
    	   att_values.properties = evt->data.evt_gatt_characteristic.properties;

    	   break;


       case gecko_evt_gatt_descriptor_id:

    	   att_values.conn_handle = evt->data.evt_gatt_descriptor.connection;
    	   att_values.descriptors = evt-> data.evt_gatt_descriptor.descriptor;

    	   break;

       case gecko_evt_gatt_characteristic_value_id:

    	   if (evt->data.evt_gatt_characteristic_value.att_opcode==29)
    	   {
    		   gecko_cmd_gatt_send_characteristic_confirmation(att_values.conn_handle);
    		   uint8_t *server_temp = &evt->data.evt_gatt_characteristic_value.value.data[0];
    		  // uint8array server_temp = & evt->data.evt_gatt_characteristic_value.value;

    		   memcpy(att_values.temperature, server_temp, 5);

    		   uint32_t t = *((uint32_t *)&att_values.temperature[1]);
    		   float server_temp_degree = (float)UINT32_TO_FLT(t);

    		   if ( server_temp_degree > threshold_temp )
    		   		{
    		   			GPIO_PinOutClear( LED1_port , LED1_pin );
    		   		}
    		   		else
    		   		{
    		   			GPIO_PinOutSet( LED1_port , LED1_pin );
    		   		}

    		   sprintf(server_temp_degree_string,"Temperature:%2.2f ",server_temp_degree);
    		   LCD_write( server_temp_degree_string , LCD_ROW_PASSKEY );
    	   }
    	   break;

       case gecko_evt_gatt_procedure_completed_id:
    	   if (command == disc_characteristics && evt->data.evt_gatt_procedure_completed.result==0)
    	   {
    		   gecko_cmd_gatt_discover_characteristics_by_uuid(evt->data.evt_gatt_procedure_completed.connection,
    				   	   	   	   	   	   	   	   	   	   	   att_values.service_handle,
															   2,(const uint8_t*)thermometer_characteristic);
    		   command = disc_descriptors;

    		   break;
    	   }

    	   if (command == disc_descriptors && evt->data.evt_gatt_procedure_completed.result==0)
    	   {
    		   gecko_cmd_gatt_discover_descriptors(evt->data.evt_gatt_procedure_completed.connection,
    				   	   	   	   	   	   	   	   	   att_values.characteristics);
    		   command = enable_indications;

    		   break;
    	   }

    	   if (command == enable_indications && evt->data.evt_gatt_procedure_completed.result==0)
    	   {
    		   gecko_cmd_gatt_set_characteristic_notification(evt->data.evt_gatt_procedure_completed.connection,
    				   	   	   	   	   	   	   	   	   	   	   att_values.characteristics,
    				   	   	   	   	   	   	   	   	   	   	   enable_indication_flag);
    		   command = finished;

    		   break;
    	   }

    	   if (command == finished && evt->data.evt_gatt_procedure_completed.result==0)
    	   {
    		   break;
    	   }
    	   break;


        case gecko_evt_system_external_signal_id:

        	if (evt->data.evt_system_external_signal.extsignals & gpio_PB1_event)
        	{
        		CORE_ATOMIC_IRQ_DISABLE();
        		ext_event &= ~gpio_PB1_event;
        		CORE_ATOMIC_IRQ_ENABLE();

        		gecko_cmd_sm_bonding_confirm(bond_con, accept_bond);
        		LCD_write( "" , LCD_ROW_ACTION );
        		LCD_write( "PAIRED" , LCD_ROW_TEMPVALUE );

        		/* Configure and Enable GPIO Interrupt For Push Button 0*/
        		GPIO_IntConfig(PB0_port,PB0_pin,rising_edge,falling_edge,interrupt_enable);
        		/* Configure and Enable GPIO Interrupt For Push Button 1*/
        		GPIO_IntConfig(PB1_port,PB1_pin,rising_edge,falling_edge,interrupt_enable);


        	}

           	if (evt->data.evt_system_external_signal.extsignals & gpio_PB0_event)
           	{
           		CORE_ATOMIC_IRQ_DISABLE();
           		ext_event &= ~gpio_PB0_event;
           		CORE_ATOMIC_IRQ_ENABLE();
           		gecko_cmd_sm_passkey_confirm( bond_con, 1);
           		LCD_write("PASSKEY CONFIRMED",LCD_ROW_ACTION );
           		LCD_write( "" , LCD_ROW_PASSKEY );

           		/* Configure and Enable GPIO Interrupt For Push Button 0*/
           		GPIO_IntConfig(PB0_port,PB0_pin,rising_edge,falling_edge,interrupt_enable);
           		/* Configure and Enable GPIO Interrupt For Push Button 1*/
           		GPIO_IntConfig(PB1_port,PB1_pin,rising_edge,falling_edge,interrupt_enable);
        	}

        	break;


        case gecko_evt_le_connection_closed_id:
        	/* Check if need to boot to dfu mode */
        	LCD_write( "DISCONNECTED" , LCD_ROW_CONNECTION );
        	gecko_cmd_system_set_tx_power(0);
        	gecko_cmd_le_gap_connect(server_address_connect,le_gap_address_type_public, sc_phy );

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
