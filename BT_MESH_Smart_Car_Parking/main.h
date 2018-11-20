/*
 * main.h
 *
 *  Created on: Nov 4, 2018
 *      Author: user1
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#define BEARER				(0x03)

#define PUBLICKEY 			(0x00)
#define AUTH_METHOD			(0x03)
#define	OUTPUT_ACTION		(0x03)
#define	OUTPUT_SIZE			(0x08)
#define	INPUT_ACTION		(0x00)
#define	INPUT_SIZE			(0x04)
#define	LOCATION_BITMASK 	(0x01)

#define PSKEY				(0x4004)
#define MOTION_T			(0)
#define PROXIMITY_T			(0)

#define SOFT_TIMER_1				(50)
#define SOFT_TIMER_FACTORY_RESET 	(51)
#define SOFT_TIMER_PROVISIONING		(52)
#define SOFT_TIMER_FIND_FRIEND		(53)

static PACKSTRUCT(struct light_states
{
	bool light_onstate;
	uint16_t motion_t;
	uint16_t proximity_t;

})light_states;


// GLOBAL VARIABLES
uint8 num_connections = 0;
static uint8_t conn_handle = 0xFF;
uint16_t pin_read;
uint16_t result;
uint8_t flag1;
uint8_t flag2;
char string_disp[30];
uint8_t netkey_type;
uint16_t key_index;

void initiate_factory_reset(void);
void push_buttons_init(void);
void light_node_init(void);
static int load_ps_data(void);
#endif /* SRC_MAIN_H_ */
