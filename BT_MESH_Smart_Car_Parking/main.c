/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs BT Mesh Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko BT Mesh C application.
 * The application starts unprovisioned Beaconing after boot
 ***************************************************************************************************
 * <b> (C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include <gecko_configuration.h>
#include <mesh_sizes.h>

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#include <em_gpio.h>

/* Device initialization header */
#include "hal-config.h"

/* Device LCD header */
#include "lcd_driver.h"

/*UART header*/
#include "retargetserial.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "mesh_generic_model_capi_types.h"
#include "mesh_lighting_model_capi_types.h"
#include "mesh_lib.h"
#include "em_core.h"
#include <mesh_sizes.h>

#include "main.h"
#include "gpio.h"
#include "letimer.h"


/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

// bluetooth stack heap
#define MAX_CONNECTIONS 2

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS) + BTMESH_HEAP_SIZE + 1760];

// Bluetooth advertisement set configuration
//
// At minimum the following is required:
// * One advertisement set for Bluetooth LE stack (handle number 0)
// * One advertisement set for Mesh data (handle number 1)
// * One advertisement set for Mesh unprovisioned beacons (handle number 2)
// * One advertisement set for Mesh unprovisioned URI (handle number 3)
// * N advertisement sets for Mesh GATT service advertisements
// (one for each network key, handle numbers 4 .. N+3)
//
#define MAX_ADVERTISERS (4 + MESH_CFG_MAX_NETKEYS)

// bluetooth stack configuration
extern const struct bg_gattdb_def bg_gattdb_data;

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;

const gecko_configuration_t config =
{
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.max_advertisers = MAX_ADVERTISERS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap) - BTMESH_HEAP_SIZE,
  .bluetooth.sleep_clock_accuracy = 100,
  .gattdb = &bg_gattdb_data,
  .btmesh_heap_size = BTMESH_HEAP_SIZE,
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .max_timers = 16,
};

static void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);
void mesh_native_bgapi_init(void);
bool mesh_bgapi_listener(struct gecko_cmd_packet *evt);



void light_node_init()
{
//	 Initialize mesh lib
	mesh_lib_init(malloc, free, 8);

	//Initialize Friend functionality
	printf("LPN mode initialization\r\n");
	result = gecko_cmd_mesh_lpn_init()->result;
	if (result)
	{
		printf("LPN init failed 0x%x\r\n", result);
	}

	memset(&light_states, 0, sizeof(struct light_states));
	if (load_ps_data())
	{
		printf("lightbulb_state_load() failed, using defaults\r\n");
//		goto publish;
	}
}



static int load_ps_data()
{
	struct gecko_msg_flash_ps_load_rsp_t* ps_load;
	ps_load = gecko_cmd_flash_ps_load(PSKEY);

	if (ps_load->result)
	{
		memset(&light_states, 0, sizeof(struct light_states));
		light_states.light_onstate = false;
		light_states.motion_t = MOTION_T;
		light_states.proximity_t = PROXIMITY_T;
		return 1;
	}
	memcpy(&light_states, ps_load->value.data, ps_load->value.len);
	return 0;
}



void initiate_factory_reset()
{
printf ("Initiate Factory Reset\r\n");
LCD_write("Factory Reset",LCD_ROW_BTADDR2);

if (conn_handle != 0xFF)
{
	gecko_cmd_le_connection_close(conn_handle);
}
//Remove all the PS data before performing a factory reset
gecko_cmd_flash_ps_erase_all();

//Give a delay before rebooting to successfully delete all PS data
gecko_cmd_hardware_set_soft_timer(2 * 32768, SOFT_TIMER_FACTORY_RESET, 1);
}



int main()
{
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();

  flag1 = 1;


  gpio_cmu_init();
  letimer_cmu_init();
  gpio_irq_init();
  led_init();
  push_buttons_init();


  RETARGET_SerialInit();
  RETARGET_SerialCrLf(true);

  gecko_stack_init(&config);
  gecko_bgapi_class_dfu_init();
  gecko_bgapi_class_system_init();
  gecko_bgapi_class_le_gap_init();
  gecko_bgapi_class_le_connection_init();
  gecko_bgapi_class_gatt_init();
  gecko_bgapi_class_gatt_server_init();
  gecko_bgapi_class_endpoint_init();
  gecko_bgapi_class_hardware_init();
  gecko_bgapi_class_flash_init();
  gecko_bgapi_class_test_init();
  gecko_bgapi_class_sm_init();
  mesh_native_bgapi_init();
  gecko_initCoexHAL();
  gecko_cmd_flash_ps_erase_all();

  while (1) {

//	  pin_read = GPIO_PinInGet(MOTION_PORT,MOTION_PIN);
//	  if (!(pin_read))
//	  {
//		  GPIO_PinOutSet(LED0_PORT, LED0_PIN);
//	  }
//	  else
//	  {
//		  GPIO_PinOutToggle(LED0_PORT, LED0_PIN);
//	  }

	  struct gecko_cmd_packet *evt = gecko_wait_event();
	  bool pass = mesh_bgapi_listener(evt);
	  if (pass) {
      handle_gecko_event(BGLIB_MSG_ID(evt->header), evt);
    }
  }
}

static void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{
	switch (evt_id) {
	case gecko_evt_dfu_boot_id:
		//gecko_cmd_le_gap_set_advertising_timing(0, 1000*adv_interval_ms/625, 1000*adv_interval_ms/625, 0, 0);
		gecko_cmd_le_gap_set_mode(2, 2);
		break;
	case gecko_evt_system_boot_id:


		LCD_init("LPN");
		printf("Boot Id\r\n");
		//		 check pushbutton state at startup. If either PB0 or PB1 is held down then do factory reset
		if (GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN) == 0 || GPIO_PinInGet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN) == 0)
		{
			initiate_factory_reset();
		}
		else
		{

			//gecko_cmd_hardware_set_soft_timer(32768*10, SOFT_TIMER_1, 1);
			struct gecko_msg_system_get_bt_address_rsp_t *bt_addr = gecko_cmd_system_get_bt_address();
			bd_addr *client_address =(bd_addr*)&bt_addr->address;
			sprintf(string_disp,"%x:%x:%x:%x:%x:%x", client_address->addr[5],client_address->addr[4],client_address->addr[3],client_address->addr[2],client_address->addr[1],client_address->addr[0]);
			LCD_write(string_disp,LCD_ROW_BTADDR1);

			// Initialize Mesh stack in Node operation mode, wait for initialized event
			result = gecko_cmd_mesh_node_init_oob(PUBLICKEY, AUTH_METHOD, OUTPUT_ACTION, OUTPUT_SIZE, INPUT_ACTION, INPUT_SIZE,
					LOCATION_BITMASK)->result;

			/*If node initialization not successful*/
			if (result)
			{
				printf ("Mesh Node Init Failed %x\r\n", result);
				sprintf (string_disp, "Mesh Node Init Failed 0x%x", result);
				LCD_write(string_disp, LCD_ROW_BTADDR2);
			}
		}
		break;


	case gecko_evt_hardware_soft_timer_id:
		switch (evt->data.evt_hardware_soft_timer.handle)
		{
		case SOFT_TIMER_1:
			printf("Software Timer Implementation Successful");
			break;


		case SOFT_TIMER_FACTORY_RESET:
			printf("Factory Reset Successful");
			gecko_cmd_system_reset(0);
			break;

		default:
			break;
		}
		break;


		case gecko_evt_mesh_node_initialized_id:

			printf ("Node Initialized\r\n");
			LCD_write("Node Initialized",LCD_ROW_BTADDR2);
			gecko_cmd_mesh_generic_server_init();

			if (evt->data.evt_mesh_node_initialized.provisioned)
			{
				printf ("Node provisioned\r\n");
				printf("address:%x, ivi:%ld\r\n", evt->data.evt_mesh_node_initialized.address, evt->data.evt_mesh_node_initialized.ivi);
				LCD_write("Node provisoned",LCD_ROW_BTADDR2);

				light_node_init();
				/*
    		result = gecko_cmd_mesh_lpn_init()->result;
    		{
    			printf ("LPN Node Init Failed %x\r\n", result);
    			sprintf (string_disp, "LPN Node Init Failed 0x%x", result);
    			LCD_write(string_disp, LCD_ROW_BTADDR2);
    		}
				 */
			}
			else
			{
				printf ("Provisioning...\r\n");
				LCD_write("Provisioning...",LCD_ROW_BTADDR2);

				// The Node is now initialized, start unprovisioned Beaconing using PB-Adv Bearer
				gecko_cmd_mesh_node_start_unprov_beaconing(BEARER);
			}
			break;

		case gecko_evt_mesh_node_provisioning_started_id:

			printf ("Provisioning Started\r\n");
			LCD_write("Provisioning Started",LCD_ROW_BTADDR2);

			break;

		case gecko_evt_le_connection_opened_id:
			printf("evt:gecko_evt_le_connection_opened_id\r\n");
			conn_handle = evt->data.evt_le_connection_opened.connection;
			break;

		case gecko_evt_le_connection_parameters_id:
			printf("evt:gecko_evt_le_connection_parameters_id\r\n");
			break;

		case gecko_evt_mesh_node_key_added_id:

			netkey_type = evt->data.evt_mesh_node_key_added.type;
			key_index = evt->data.evt_mesh_node_key_added.index;
			uint16_t netkey_index = evt->data.evt_mesh_node_key_added.netkey_index;
			printf ("Received New %s Key with Index %d\r\n" ,
					netkey_type ==0 ? "Network" : "Application" , netkey_index );
			struct gecko_msg_mesh_node_display_output_oob_evt_t *disp_oob =
					(struct gecko_msg_mesh_node_display_output_oob_evt_t *)&(evt->data);

			break;


		case gecko_evt_mesh_node_display_output_oob_id:
		{
			struct gecko_msg_mesh_node_display_output_oob_evt_t *pOOB = (struct gecko_msg_mesh_node_display_output_oob_evt_t *)&(evt->data);
			printf("gecko_msg_mesh_node_display_output_oob_evt_t: action %d, size %d\r\n", pOOB->output_action, pOOB->output_size);
			for(int i=0;i<pOOB->data.len;i++)
			{
				printf("%2.2x ", pOOB->data.data[i]);
			}
			printf("\r\n");
		}
		break;
		/*
    case gecko_evt_mesh_node_display_output_oob_id:

    	printf("Display Passkey\r\n");
    	//if (disp_oob->output_action == OUTPUT_ACTION)
    	//{
    		printf("Data Size %d\n", disp_oob->output_size);
    		for(int i=0;i < disp_oob->data.len;i++)
    		{
    			printf("%x", disp_oob->data.data[i]);
    		}
    		sprintf(string_disp,"%x%x%x%x", disp_oob->data.data[3],disp_oob->data.data[2],disp_oob->data.data[1]
																		  ,disp_oob->data.data[0]);
    		LCD_write(string_disp,LCD_ROW_PASSKEY);

    	//}
    	break;

		 */
		case gecko_evt_mesh_node_provisioning_failed_id:
			printf ("Provisioning Failed\r\n");
			LCD_write("Provisioning Failed",LCD_ROW_BTADDR2);

			//gecko_cmd_mesh_node_start_unprov_beaconing(BEARER);
			break;

		case gecko_evt_mesh_node_provisioned_id:

			printf ("Provisioning Successful\r\n");
			LCD_write("Provisioning S",LCD_ROW_BTADDR2);
			uint32_t ivindex = evt->data.evt_mesh_node_provisioned.iv_index;
			uint16_t unicast_address = evt->data.evt_mesh_node_provisioned.address;
			//LCD_write("",LCD_ROW_PASSKEY);
			light_node_init();
			break;

		case gecko_evt_le_connection_closed_id:
			/* Check if need to boot to dfu mode */
			if (boot_to_dfu) {
				/* Enter to DFU OTA mode */
				gecko_cmd_system_reset(2);
			}
			printf("gecko_evt_le_connection_closed_id\r\n");
			conn_handle = 0xFF;
			break;


		case gecko_evt_system_external_signal_id:

			if (evt->data.evt_system_external_signal.extsignals & MOTION_EVENT)
			{
				CORE_ATOMIC_IRQ_DISABLE();
				EXT_EVENT &= ~MOTION_EVENT;
				CORE_ATOMIC_IRQ_ENABLE();
				if (flag1==1)
				{
				GPIO_PinOutToggle(LED0_PORT,LED0_PIN);
				GPIO_PinOutToggle(LED1_PORT,LED1_PIN);
				flag1 = 0;
				}

				LETIMER_Reset(LETIMER0);
				letimer_init();
				letimer_start();

				printf("\r\nMOTON DETECTED\r\n");
				printf("TIMER STARTED");
				/* Configure and Enable GPIO Interrupt For MOTION SENSOR*/
				GPIO_IntConfig(MOTION_PORT,MOTION_PIN,RISING_EDGE,FALLING_EDGE,INTERRUPT_ENABLE);

			}

			if (evt->data.evt_system_external_signal.extsignals & LETIMER_EVENT)
			{
				letimer_stop();
				CORE_ATOMIC_IRQ_DISABLE();
				EXT_EVENT &= ~LETIMER_EVENT;
				CORE_ATOMIC_IRQ_ENABLE();
				GPIO_PinOutClear(LED0_PORT,LED0_PIN);
				GPIO_PinOutClear(LED1_PORT,LED1_PIN);

				flag1 = 1;
				printf("\r\nTIMER FINISHED\r\n");
			}

			break;

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