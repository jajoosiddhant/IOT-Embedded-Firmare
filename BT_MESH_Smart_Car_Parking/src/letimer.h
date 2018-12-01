/*
 * letimer.h
 *
 *  Created on: Nov 16, 2018
 *      Author: user1
 */

#ifndef SRC_LETIMER_H_
#define SRC_LETIMER_H_

#include "em_letimer.h"
#include "em_core.h"
#include "em_cmu.h"
#include "hal-config.h"
#include "native_gecko.h"



#define LETIMER_EVENT		(0x08)	//Scheduler Event Flag for LETIMER
#define wait_timeperiod		(120)	//seconds

volatile uint32_t EXT_EVENT;
int totalticks;
int onticks;


void letimer_cmu_init(void);
void letimer_init(void);
void letimer_start(void);
void letimer_stop(void);
void prescale(void);

#endif /* SRC_LETIMER_H_ */
