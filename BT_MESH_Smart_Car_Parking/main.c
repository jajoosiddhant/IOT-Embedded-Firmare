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
#include "bg_errorcodes.h"
#include <mesh_sizes.h>

#include "main.h"
#include "gpio.h"
#include "letimer.h"
#include "i2c.h"


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


/**
 * Initialize LPN functionality with configuration and friendship establishment
 * Also includes function calls to store and save data in persistent storage.
 * Includes a power-up routine behaviour
 * Function Call to Model initialization
 */
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

	 result = gecko_cmd_mesh_lpn_configure(2, 5 * 1000)->result;
	  if (result)
	  {
	    printf("LPN conf failed (0x%x)\r\n", result);
	    return;
	  }

	  printf("trying to find friend...\r\n");
	  result = gecko_cmd_mesh_lpn_establish_friendship(0)->result;

	  if (result != 0) {
	    printf("ret.code %x\r\n", result);
	  }

	memset(&light_states, 0, sizeof(struct light_states));
	if (load_ps_data())
	{
		printf("lightbulb_state_load() failed, using defaults\r\n");
		printf("Skipped power up routine\r\n");
		goto publish;
	}

	// Handle on power up behavior
	switch (light_states.onpowerup)
	{
	case MESH_GENERIC_ON_POWER_UP_STATE_OFF:
		printf("On power up state is OFF\r\n");
		light_states.onoff_current = MESH_GENERIC_ON_OFF_STATE_OFF;
		light_states.onoff_target = MESH_GENERIC_ON_OFF_STATE_OFF;
		LED_state(LED_STATE_OFF);
		break;

	case MESH_GENERIC_ON_POWER_UP_STATE_ON:
		printf("On power up state is ON\r\n");
		light_states.onoff_current = MESH_GENERIC_ON_OFF_STATE_ON;
		light_states.onoff_target = MESH_GENERIC_ON_OFF_STATE_ON;
		LED_state(LED_STATE_ON);
		break;

	case MESH_GENERIC_ON_POWER_UP_STATE_RESTORE:
		printf("On power up state is RESTORE\r\n");
		LED_state(LED_STATE_RESTORE);
		if (light_states.onoff_current != light_states.onoff_target)
		{
//			uint32_t transition_ms = default_transition_time();
			printf("CURRENT != TARGET\r\n");
			if (light_states.onoff_target == MESH_GENERIC_ON_OFF_STATE_OFF)
			{
				LED_state(LED1_STATE_OFF);
				LCD_write("LIGHT -> 'OFF'",LCD_ROW_CLIENTADDR);
			}
			else
			{
				LED_state(LED1_STATE_ON);
				LCD_write("LIGHT -> 'ON'",LCD_ROW_CLIENTADDR);
			}

			light_states.onoff_current = light_states.onoff_target;

		}
		else
		{
			printf("Keeping loaded state\r\n");
			printf("CURRENT == TARGET\r\n");
			if (light_states.onoff_current == MESH_GENERIC_ON_OFF_STATE_OFF)
			{
				LED_state(LED1_STATE_OFF);
				LCD_write("LIGHT -> 'OFF'",LCD_ROW_CLIENTADDR);
			}
			else
			{
				LED_state(LED1_STATE_ON);
				LCD_write("LIGHT -> 'ON'",LCD_ROW_CLIENTADDR);
			}
		}
		break;
	}

	publish:
	lightstate_save();
	models_init();
	onoff_update(primary_element);
//	uint8_t x = onoff_update_and_publish(primary_element);
//	power_onoff_update_and_publish(primary_element);

	init_complete = 1;			//Flag to service interrupts only after initialization is complete
}


// SERVER SIDE RELATED FUNCTIONS

/*
 * Initializing Different Models Used
 */
static void models_init(void)
{
  mesh_lib_generic_server_register_handler(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
                                           0,
                                           onoff_request,
                                           onoff_change);
  mesh_lib_generic_server_register_handler(MESH_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL_ID,
                                           0,
                                           power_onoff_request,
                                           power_onoff_change);
//  mesh_lib_generic_server_register_handler(MESH_GENERIC_TRANSITION_TIME_SERVER_MODEL_ID,
//                                           0,
//                                           transtime_request,
//                                           transtime_change);

}

/*
 * Function to change Server State as requested by the Client
 */
static void onoff_request(uint16_t model_id,
		uint16_t element_index,
		uint16_t client_addr,
		uint16_t server_addr,
		uint16_t appkey_index,
		const struct mesh_generic_request *request,
		uint32_t transition_ms,
		uint16_t delay_ms,
		uint8_t request_flags)
{
	printf("ON/OFF request: requested state=<%s>, transition=%lu, delay=%u\r\n",
			request->on_off ? "ON" : "OFF", transition_ms, delay_ms);

	if (light_states.onoff_current == request->on_off)
	{
		printf("Request for current state received; no op\n");
	}
	else
	{
//		transition_ms = 0;
//		delay_ms = 0;
		printf("Turning lightbulb <%s>\r\n", request->on_off ? "ON" : "OFF");
		if (transition_ms == 0 && delay_ms == 0)
		{ // Immediate change
			light_states.onoff_current = request->on_off;
			light_states.onoff_target = request->on_off;
			if (light_states.onoff_current == MESH_GENERIC_ON_OFF_STATE_OFF)
			{
				LED_state(LED1_STATE_OFF);
				LCD_write("LIGHT -> 'OFF'",LCD_ROW_CLIENTADDR);
			}
			else
			{
				LED_state(LED1_STATE_ON);
				LCD_write("LIGHT -> 'ON'",LCD_ROW_CLIENTADDR);
			}
		}
	/*
		else if (delay_ms > 0)
		{
			// a delay has been specified for the light change. Start a soft timer
			// that will trigger the change after the given delay
			// Current state remains as is for now
			light_states.onoff_target = request->on_off;
			gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(delay_ms), TIMER_ID_DELAYED_ONOFF, 1);
			// store transition parameter for later use
//			delayed_onoff_trans = transition_ms;
		}

		else
		{
			// no delay but transition time has been set.
			light_states.onoff_target = request->on_off;

			if (request->on_off == MESH_GENERIC_ON_OFF_STATE_OFF)
			{
				LEDS_SetLevel(0, transition_ms);
			}
			else
			{
				// restore last brightness
				light_states.lightness_target = lightbulb_state.lightness_last;
				LEDS_SetLevel(lightbulb_state.lightness_target, transition_ms);
			}
			// lightbulb current state will be updated when transition is complete
			gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(transition_ms), TIMER_ID_ONOFF_TRANSITION, 1);
		}
		*/
		lightstate_save();
	}

	if (request_flags & MESH_REQUEST_FLAG_RESPONSE_REQUIRED)
	{
		onoff_response(element_index, client_addr, appkey_index);
	}
	else
	{
		onoff_update(element_index);
	}
}


static void onoff_change(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
                         uint32_t remaining_ms)
{
  if (current->on_off.on != light_states.onoff_current) {
    printf("on-off state changed %u to %u\r\n", light_states.onoff_current, current->on_off.on);

    light_states.onoff_current = current->on_off.on;
    lightstate_save();
  } else {
    printf("dummy onoff change - same state as before\r\n");
  }
}

/*
 * Function to respond to client calls
 */
static errorcode_t onoff_response(uint16_t element_index,
                                  uint16_t client_addr,
                                  uint16_t appkey_index)
{
  struct mesh_generic_state current, target;

  current.kind = mesh_generic_state_on_off;
  current.on_off.on = light_states.onoff_current;

  target.kind = mesh_generic_state_on_off;
  target.on_off.on = light_states.onoff_target;

  return mesh_lib_generic_server_response(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
                                          element_index,
                                          client_addr,
                                          appkey_index,
                                          &current,
                                          &target,
                                          0,
                                          0x00);
}

/**
 * Updates the onoff data.
 */
static errorcode_t onoff_update(uint16_t element_index)
{
  struct mesh_generic_state current, target;

  current.kind = mesh_generic_state_on_off;
  current.on_off.on = light_states.onoff_current;

  target.kind = mesh_generic_state_on_off;
  target.on_off.on = light_states.onoff_target;

  return mesh_lib_generic_server_update(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
                                        element_index,
                                        &current,
                                        &target,
                                        0);
}

/**
 * Updates and publishes the onoff data to the network
 */
static errorcode_t onoff_update_and_publish(uint16_t element_index, int retrans)
{
	errorcode_t e;
	const uint32 transtime = 0; /* using zero transition time by default */

	e = onoff_update(element_index);

	struct mesh_generic_request req;
	req.kind = mesh_generic_request_on_off;
	req.on_off = light_states.onoff_current ? MESH_GENERIC_ON_OFF_STATE_ON : MESH_GENERIC_ON_OFF_STATE_OFF;

	// increment transaction ID for each request, unless it's a retransmission
  	if (retrans == 0)
  	{
  		trid++;
  	}

  	if (e == bg_err_success)
  	{
  		e = mesh_lib_generic_server_publish(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
  				element_index,
				mesh_generic_state_on_off);
  	}

  	if (e)
  	{
  		printf("gecko_cmd_mesh_generic_client_publish failed,code %x\r\n", e);
  	}
  	else
  	{
  		printf("request sent, trid = %u\r\n", trid);
  	}

    /* keep track of how many requests has been sent */
    if (request_count > 0)
    {
    	request_count--;
    }

  return e;
}


static errorcode_t power_onoff_response(uint16_t element_index,
                                        uint16_t client_addr,
                                        uint16_t appkey_index)
{
  struct mesh_generic_state current;
  current.kind = mesh_generic_state_on_power_up;
  current.on_power_up.on_power_up = light_states.onpowerup;

  return mesh_lib_generic_server_response(MESH_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL_ID,
                                          element_index,
                                          client_addr,
                                          appkey_index,
                                          &current,
                                          NULL,
                                          0,
                                          0x00);
}

static errorcode_t power_onoff_update(uint16_t element_index)
{
  struct mesh_generic_state current;
  current.kind = mesh_generic_state_on_power_up;
  current.on_power_up.on_power_up = light_states.onpowerup;

  return mesh_lib_generic_server_update(MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL_ID,
                                        element_index,
                                        &current,
                                        NULL,
                                        0);
}

static errorcode_t power_onoff_update_and_publish(uint16_t element_index)
{
  errorcode_t e;

  e = power_onoff_update(element_index);
  if (e == bg_err_success) {
    e = mesh_lib_generic_server_publish(MESH_GENERIC_POWER_ON_OFF_SERVER_MODEL_ID,
                                        element_index,
                                        mesh_generic_state_on_power_up);
  }

  return e;
}

static void power_onoff_request(uint16_t model_id,
                                uint16_t element_index,
                                uint16_t client_addr,
                                uint16_t server_addr,
                                uint16_t appkey_index,
                                const struct mesh_generic_request *request,
                                uint32_t transition_ms,
                                uint16_t delay_ms,
                                uint8_t request_flags)
{
  printf("ON POWER UP request received; state=<%s>\n",
         light_states.onpowerup == 0 ? "OFF"
         : light_states.onpowerup == 1 ? "ON"
         : "RESTORE");

  if (light_states.onpowerup == request->on_power_up) {
    printf("Request for current state received; no op\n");
  } else {
    printf("Setting onpowerup to <%s>\n",
           request->on_power_up == 0 ? "OFF"
           : request->on_power_up == 1 ? "ON"
           : "RESTORE");
    light_states.onpowerup = request->on_power_up;
    lightstate_save();
  }

  if (request_flags & MESH_REQUEST_FLAG_RESPONSE_REQUIRED) {
    power_onoff_response(element_index, client_addr, appkey_index);
  } else {
    power_onoff_update(element_index);
  }
}


static void power_onoff_change(uint16_t model_id,
                               uint16_t element_index,
                               const struct mesh_generic_state *current,
                               const struct mesh_generic_state *target,
                               uint32_t remaining_ms)
{
  // TODO
}

/*
 * Detects the Current State of the LIGHTBULB if ON or OFF and sends it to
 * the central node / friend to display the status of the LightBulb.
 */
static void send_light_state(void)
{
	if (flag1==1)
	{
		light_states.onoff_current = MESH_GENERIC_ON_OFF_STATE_ON;
		LED_state(LED1_STATE_ON);
		LCD_write("LIGHT -> 'ON'",LCD_ROW_CLIENTADDR);
//		request_count = 3;
		//For testing Purpose
//		onoff_update_and_publish(primary_element, 0); 	// 0 indicates this is first transmission
//		gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0.05), SOFT_TIMER_RETRANSMISSION, 1);
		lightstate_save();
		flag1 = 0;
	}
	printf("\r\nMOTON DETECTED\r\n");
}

/*
 * Detects the Current State of the parking Slot if occupied or vacant and sends it to
 * the central node / friend to display the status of the Parking Slot(PS).
 */
static void send_pslot_state()
{
	uint16_t dist_cm;
	uint16_t confidence;

	printf("PROXIMITY EVENT\r\n");
	dist_cm = getdistance()/10;
	confidence = getconfidencevalue();

	if (dist_cm > light_states.proximity_t && confidence < light_states.proximity_min_con)
	{
		empty_count++;
//		LCD_write("",LCD_ROW_CONNECTION);

		if (empty_count == 10)				//10 is a random value used as delay
		{
			if (empty_count>65530)
			{
				empty_count = 11;
			}
			light_states.ps_current = MESH_GENERIC_ON_OFF_STATE_OFF;	//This value denotes Parking Slot is Empty
			printf("Parking Slot Empty\r\n");
			LCD_write("SLOT EMPTY",LCD_ROW_CONNECTION);
			LED_state(LED0_STATE_OFF);
			gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0), SOFT_TIMER_PARKING_IN_PROCESS, 0);
			//The below 3 lines should be written inside the interrupt routine for Proximity Sensor
			request_count = 3;
			//For testing Purpose
			onoff_update_and_publish(primary_element, 0); 	// 0 indicates this is first transmission
			gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0.05), SOFT_TIMER_RETRANSMISSION, 1);
			lightstate_save();

		}
	}
	else if (dist_cm < light_states.proximity_t && confidence > light_states.proximity_min_con)
	{

		light_states.ps_current = MESH_GENERIC_ON_OFF_STATE_ON;	//This value denotes Parking Slot is Occupied
		if (dist_cm < light_states.min_alarm_dist && confidence > light_states.proximity_min_con)
		{
			//In order to Blink LEDS indicating Car too close to the Sensor
			gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0.25), SOFT_TIMER_PARKING_IN_PROCESS, 0);
		}
		else
		{
			//In order to stop LEDS blinking
			gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0), SOFT_TIMER_PARKING_IN_PROCESS, 0);
			LED_state(LED0_STATE_OFF);
		}
		printf("Parking Slot Occupied\r\n");
		printf("D = %dcm \t C = %d\r\n",dist_cm, confidence);
		sprintf(string_disp,"D=%dcm C=%d\r\n", dist_cm, confidence);
		LCD_write(string_disp,LCD_ROW_CONNECTION);
		if (empty_count!=0)
		{
			empty_count = 0;
			//The below 3 lines should be written inside the interrupt routine for Proximity Sensor
			request_count = 3;
			//For testing Purpose
			onoff_update_and_publish(primary_element, 0); 	// 0 indicates this is first transmission
			gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0.05), SOFT_TIMER_RETRANSMISSION, 1);
			lightstate_save();
		}
	}
	else
	{
		printf("D = %dcm \t C = %d\r\n",dist_cm, confidence);
		sprintf(string_disp,"D=%dcm C=%d\r\n", dist_cm, confidence);
		LCD_write(string_disp,LCD_ROW_CONNECTION);
	}
}

// CLIENT SIDE RELATED FUNCTIONS
/**
 * This function loads the saved states from Persistent Storage and
 * copies the data in the global variable light_states
 */
static int load_ps_data()
{
	struct gecko_msg_flash_ps_load_rsp_t* ps_load;
	ps_load = gecko_cmd_flash_ps_load(PSKEY);

	if (ps_load->result)
	{
		memset(&light_states, 0, sizeof(struct light_states));
		light_states.onoff_current = MESH_GENERIC_ON_OFF_STATE_OFF;
		light_states.onoff_target = MESH_GENERIC_ON_OFF_STATE_OFF;
		light_states.ps_current = MESH_GENERIC_ON_OFF_STATE_OFF;
		light_states.ps_target = MESH_GENERIC_ON_OFF_STATE_OFF;
		light_states.proximity_t = PROXIMITY_T;
		light_states.proximity_min_con = PROXIMITY_MIN_CON;
		light_states.min_alarm_dist = MIN_ALARM_DIST;
		light_states.transtime = 0;
		light_states.onpowerup = MESH_GENERIC_ON_POWER_UP_STATE_OFF;

		return 1;
	}
	memcpy(&light_states, ps_load->value.data, ps_load->value.len);
	return 0;
}

/**
 * this function saves the current states of light and parking slots in Persistent Storage so that
 * the data is preserved over reboots and power cycles. The state are held
 * in a global variable light_states. a PS key with ID 0x4004 is used to store
 * the whole structure.
 */
static int save_ps_data(void)
{
  struct gecko_msg_flash_ps_save_rsp_t* pSave;

  pSave = gecko_cmd_flash_ps_save(0x4004, sizeof(struct light_states), (const uint8*)&light_states);

  if (pSave->result) {
    printf("lightbulb_state_store(): PS save failed, code %x\r\n", pSave->result);
    return(-1);
  }

  return 0;
}

/**
 *  this function is called to initiate factory reset. Factory reset may be initiated
 *  by keeping one of the WSTK pushbuttons pressed during reboot. Factory reset is also
 *  performed if it is requested by the provisioner (event gecko_evt_mesh_node_reset_id)
 */
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
gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(2), SOFT_TIMER_FACTORY_RESET, 1);
}

/**
 * Get Device Bluetooth Address and display on LCD
 */
void disp_device_addr()
{
	struct gecko_msg_system_get_bt_address_rsp_t *bt_addr = gecko_cmd_system_get_bt_address();
	bd_addr *client_address =(bd_addr*)&bt_addr->address;
	sprintf(string_disp,"%x:%x:%x:%x:%x:%x", client_address->addr[5],client_address->addr[4],client_address->addr[3],client_address->addr[2],client_address->addr[1],client_address->addr[0]);
	LCD_write(string_disp,LCD_ROW_BTADDR1);
}

/*
static uint32_t default_transition_time(void)
{
  return mesh_lib_transition_time_to_ms(light_states.transtime);
}
*/

/**
 * Function calls to save and load state values
 */
static void lightstate_save(void)
{
gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(3), SOFT_TIMER_LIGHTSTATE_SAVE, 1);
}

int main()
{
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();

  /**initializing the flags to default Values
   *@flag1 - To update the state value only once in certain specified time
   *@flag2 - To  update and publish state value only once in certain specified
   *@empty_count - To publish data only once
  **/
  flag1 = 1;
  flag2 = 1;
  empty_count = 0;


  gpio_cmu_init();
  gpio_irq_init();
  letimer_cmu_init();
  letimer_init();
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
//  gecko_cmd_flash_ps_erase_all();



  gecko_bgapi_class_mesh_node_init();
  gecko_bgapi_class_mesh_proxy_init();
  gecko_bgapi_class_mesh_proxy_server_init();
  gecko_bgapi_class_mesh_proxy_client_init();
  gecko_bgapi_class_mesh_generic_client_init();
  gecko_bgapi_class_mesh_generic_server_init();
  //gecko_bgapi_class_mesh_test_init();
  gecko_bgapi_class_mesh_lpn_init();

  while (1)
  {
	  struct gecko_cmd_packet *evt = gecko_wait_event();
	  bool pass = mesh_bgapi_listener(evt);
	  if (pass)
	  {
		  handle_gecko_event(BGLIB_MSG_ID(evt->header), evt);
	  }
  }
}

static void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{
	switch (evt_id) {
	case gecko_evt_dfu_boot_id:
		gecko_cmd_le_gap_set_mode(2, 2);
		break;

	case gecko_evt_system_boot_id:
		LCD_init("LPN");
		printf("Boot Id\r\n");
		//check pushbutton state at startup. If either PB0 or PB1 is held down then do factory reset
		if (GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN) == 0 || GPIO_PinInGet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN) == 0)
		{
			initiate_factory_reset();
		}
		else
		{
			disp_device_addr();

			// Initialize Mesh stack in Node operation mode, wait for initialized event with OOB authentication Method
			result = gecko_cmd_mesh_node_init_oob(PUBLICKEY, AUTH_METHOD, OUTPUT_ACTION,
												OUTPUT_SIZE, INPUT_ACTION, INPUT_SIZE,LOCATION_BITMASK)->result;

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
		case SOFT_TIMER_FACTORY_RESET:
			printf("Factory Reset Successful\r\n");
			gecko_cmd_system_reset(0);
			break;

		case SOFT_TIMER_PROVISIONING:
			LED_state(LED_STATE_PROV);
			break;

		case SOFT_TIMER_FIND_FRIEND:
			printf("trying to find friend...\r\n");
			result = gecko_cmd_mesh_lpn_establish_friendship(0)->result;

			if (result != 0)
			{
				printf("ret.code %x\r\n", result);
			}
			break;

		case SOFT_TIMER_LIGHTSTATE_SAVE:
			printf("Saving the latest state values\r\n");
			save_ps_data();
			load_ps_data();
			break;

		case SOFT_TIMER_RETRANSMISSION:
			printf("Retransmitting..\r\n");
			onoff_update_and_publish(primary_element,1);   // 1 as a parameter indicates this is a retransmission
			if (request_count == 0)
			{
				gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0), SOFT_TIMER_RETRANSMISSION, 0);
			}
			else
			{
				gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0.05), SOFT_TIMER_RETRANSMISSION, 1);
			}
			break;

		case SOFT_TIMER_PROXIMITY_INTERRUPT_ENABLE:
			GPIO_IntEnable(PROXIMITY_INT_REG_VALUE);
			break;

		case SOFT_TIMER_MOTION_INTERRUPT_ENABLE:
			/*To Turn off the light after 120 seconds if no motion is detected*/
			gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(WAIT_TIME_LIGHTOFF), SOFT_TIMER_MOTION_LIGHT_OFF, 1);
			/*Clear Interrupt flags*/
			int flag = GPIO_IntGet();
			flag = flag & (MOTION_INT_REG_VALUE);
			GPIO_IntClear(flag);
			GPIO_IntEnable(MOTION_INT_REG_VALUE);
			break;

		case SOFT_TIMER_MOTION_LIGHT_OFF:
			letimer_stop();
			LED_state(LED1_STATE_OFF);
			LCD_write("LIGHT -> 'OFF'",LCD_ROW_CLIENTADDR);
			flag1 = 1;
			break;

		case SOFT_TIMER_PARKING_IN_PROCESS:
			LED_state(LED0_STATE_PARKING_IN_PROCESS);
			break;

		default:
			break;
		}
		break;

		case gecko_evt_mesh_node_initialized_id:
			printf ("Node Initialized\r\n");
			LCD_write("Node Initialized",LCD_ROW_BTADDR2);

			//Initializing  Generic Server Stack
			gecko_cmd_mesh_generic_server_init();

			if (evt->data.evt_mesh_node_initialized.provisioned)
			{
				printf ("Node provisioned\r\n");
				printf("address:%x, ivi:%ld\r\n", evt->data.evt_mesh_node_initialized.address, evt->data.evt_mesh_node_initialized.ivi);
				LCD_write("Node provisioned",LCD_ROW_BTADDR2);
				primary_element = 0;			// Primary ELement Index Hardcoded
				light_node_init();

				/********************************/
//				gpio_irq_init();
				i2c_init();
				proximity_powerup_config();
				proximity_config();
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

			//In order to Blink LEDS during Provisioning
			gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0.25), SOFT_TIMER_PROVISIONING, 0);
			break;

		case gecko_evt_le_connection_opened_id:
			printf("evt:gecko_evt_le_connection_opened_id\r\n");
			num_connections++;
			conn_handle = evt->data.evt_le_connection_opened.connection;
			break;

		case gecko_evt_le_connection_parameters_id:
			printf("evt:gecko_evt_le_connection_parameters_id\r\n");
			break;

		case gecko_evt_mesh_node_model_config_changed_id:
			printf("model config changed\r\n");
			break;

		case gecko_evt_mesh_node_key_added_id:
			LCD_write("", LCD_ROW_PASSKEY);
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
			printf("%2.2x ", pOOB->data.data[pOOB->data.len-1]);
			sprintf(string_disp,"PASSKEY : %2.2x", pOOB->data.data[pOOB->data.len-1]);
			LCD_write(string_disp, LCD_ROW_PASSKEY);
			printf("\r\n");
		}
		break;

		case gecko_evt_mesh_node_provisioning_failed_id:
			printf ("Provisioning Failed\r\n");
			LCD_write("Provisioning Failed",LCD_ROW_BTADDR2);

			/* start a one-shot timer that will trigger soft reset after small delay */
			gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(2), SOFT_TIMER_FACTORY_RESET, 1);
			break;

		case gecko_evt_mesh_node_provisioned_id:
			printf ("Node Provisioned\r\n");
			LCD_write("Node Provisioned",LCD_ROW_BTADDR2);
//			uint32_t ivindex = evt->data.evt_mesh_node_provisioned.iv_index;
			uint16_t unicast_address = evt->data.evt_mesh_node_provisioned.address;
			printf("Server address : %d ", unicast_address);
			primary_element = 0;			// Primary ELement Index Hardcoded
			gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0), SOFT_TIMER_PROVISIONING, 0);
			LED_state(LED_STATE_OFF);
			light_node_init();
			printf("node provisioned, got address=%x\r\n", evt->data.evt_mesh_node_provisioned.address);

			/********************************/
//			gpio_irq_init();
			i2c_init();
			proximity_powerup_config();
			proximity_config();
			break;

		case gecko_evt_mesh_generic_server_client_request_id:
			printf("evt gecko_evt_mesh_generic_server_client_request_id\r\n");
			mesh_lib_generic_server_event_handler(evt);
			break;

		case gecko_evt_mesh_generic_server_state_changed_id:

			// uncomment following line to get debug prints for each server state changed event
			//server_state_changed(&(evt->data.evt_mesh_generic_server_state_changed));

			// pass the server state changed event to mesh lib handler that will invoke
			// the callback functions registered by application
			mesh_lib_generic_server_event_handler(evt);
			break;

		case gecko_evt_mesh_lpn_friendship_established_id:
			printf("friendship established\r\n");
			printf("Address : %x\r\n",evt->data.evt_mesh_lpn_friendship_established.friend_address);
			LCD_write("Friendship"	, LCD_ROW_BTADDR2);

			/*******************************/
			//Enable Interrupts Only if Friendship is established
//			gpio_irq_init();
//			i2c_init();
//			proximity_powerup_config();
//			proximity_config();
			break;

		case gecko_evt_mesh_lpn_friendship_failed_id:
			printf("friendship failed\r\n");
			LCD_write("No Friend", LCD_ROW_BTADDR2);
			// try again in 2 seconds
			result  = gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(2), SOFT_TIMER_FIND_FRIEND, 1)->result;
			if (result)
			{
				printf("timer failure?!  %x\r\n", result);
			}
			break;

		case gecko_evt_mesh_lpn_friendship_terminated_id:
			printf("friendship terminated\r\n");
			LCD_write("Friend lost",LCD_ROW_BTADDR2);
			if (num_connections == 0)
			{
				// try again in 2 seconds
				result  = gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(2), SOFT_TIMER_FIND_FRIEND, 1)->result;
				if (result)
				{
					printf("timer failure?!  %x\r\n", result);
				}
			}
			break;

		case gecko_evt_le_connection_closed_id:
			/* Check if need to boot to dfu mode */
			if (boot_to_dfu)
			{
				/* Enter to DFU OTA mode */
				gecko_cmd_system_reset(2);
			}
			printf("gecko_evt_le_connection_closed_id\r\n");
			conn_handle = 0xFF;
			num_connections--;
//			if (num_connections > 0)
//			{
//				if (--num_connections == 0)
//				{
//					lpn_init();
//				}
//			}
			break;

		case gecko_evt_mesh_node_reset_id:
			printf("evt gecko_evt_mesh_node_reset_id\r\n");
			initiate_factory_reset();
			break;

		case gecko_evt_system_external_signal_id:

			if (init_complete == 1 && (evt->data.evt_system_external_signal.extsignals & MOTION_EVENT))
			{
				CORE_ATOMIC_IRQ_DISABLE();
				EXT_EVENT &= ~MOTION_EVENT;
				CORE_ATOMIC_IRQ_ENABLE();

				/*LPN poll can be skipped (DOES AUTOMATICALLY)*/
//				result = gecko_cmd_mesh_lpn_poll()->result;
//				if (result)
//				{
//				printf("lpn polling failed : 0x%x\r\n", result);
//				}
				send_light_state();
				gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(MOTION_INT_RENABLE), SOFT_TIMER_MOTION_INTERRUPT_ENABLE, 1);
				gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0), SOFT_TIMER_MOTION_LIGHT_OFF, 1);
				letimer_start();
			}

			if (init_complete == 1 && (evt->data.evt_system_external_signal.extsignals & PROXIMITY_EVENT))
			{
				CORE_ATOMIC_IRQ_DISABLE();
				EXT_EVENT &= ~PROXIMITY_EVENT;
				CORE_ATOMIC_IRQ_ENABLE();

				/*LPN poll can be skipped (DOES AUTOMATICALLY)*/
//				result = gecko_cmd_mesh_lpn_poll()->result;
//				if (result)
//				{
//					printf("lpn polling failed : 0x%x\r\n", result);
//				}

				send_pslot_state();
				gecko_cmd_hardware_set_soft_timer(TIMER_S_TO_TICKS(0.1), SOFT_TIMER_PROXIMITY_INTERRUPT_ENABLE, 1);
			}

			if (evt->data.evt_system_external_signal.extsignals & LETIMER_EVENT)
			{
				CORE_ATOMIC_IRQ_DISABLE();
				EXT_EVENT &= ~LETIMER_EVENT;
				CORE_ATOMIC_IRQ_ENABLE();
				printf("LETIMER EVENT\r\n");
				ecode = proximity_takemeasurement();
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
