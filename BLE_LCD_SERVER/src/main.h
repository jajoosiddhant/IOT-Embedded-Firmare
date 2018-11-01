//***********************************************************************************
// Include files
//***********************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

//***********************************************************************************
// defined files
//***********************************************************************************
#define adv_handle			0
#define adv_interval_min	400
#define adv_interval_max	400
#define adv_timing_duration	0
#define adv_maxevents		0

#define con_interval_min	60
#define con_interval_max	60
#define con_latency			3
#define con_timeout			400

#define security_configure_flag 0x0F
//***********************************************************************************
// global variables
//***********************************************************************************
uint8_t con;
uint8_t bond_con;
int32_t txpowe;
int32_t rvalue;
char pass_word[32];
char server_address_string[6];
char client_address_string[6];
int a;
volatile uint32_t ext_event;

//***********************************************************************************
// function prototypes
//***********************************************************************************
