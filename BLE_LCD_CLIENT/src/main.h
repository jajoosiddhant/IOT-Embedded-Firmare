//***********************************************************************************
// Include files
//***********************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//***********************************************************************************
// defined files
//***********************************************************************************
#define adv_handle				(0)
#define adv_interval_min		(400)
#define adv_interval_max		(400)
#define adv_timing_duration		(0)
#define adv_maxevents			(0)

#define con_interval_min		(60)
#define con_interval_max		(60)
#define con_latency				(3)
#define con_timeout				(400)

#define security_configure_flag (0x0F)

#define	sc_phy					(0x01)
#define s_phy 					(0x05)
#define scanning_interval		(0x10)
#define scanning_window			(0x10)
#define scanning_type			(0x01)

#define accept_bond				(1)
#define pairing_button_press	(1)
#define bonding_button_press	(2)

//#define uuid_len 				 2
#define enable_indication_flag	(0x02)
#define threshold_temp			(15)

#define UINT32_TO_FLT(data) (((float)((int32_t)(data) & 0x00FFFFFFU)) * (float)pow(10, ((int32_t)(data) >> 24)))
//***********************************************************************************
// global variables
//***********************************************************************************
uint8_t con;
uint8_t bond_con;
int32_t txpowe;
int32_t rvalue;
uint64_t server_address_int;
char pass_word[32];
char server_address_string[6];
char client_address_string[6];
char server_temp_degree_string[80];
char sign_string[10];
uint8_t a;
uint8_t pair_bond;
volatile uint32_t ext_event;

//***********************************************************************************
// function prototypes
//***********************************************************************************

