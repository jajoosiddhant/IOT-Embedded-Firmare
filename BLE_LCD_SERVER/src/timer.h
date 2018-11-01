/*
 * timer.h
 *
 *  Created on: Sep 29, 2018
 *      Author: user1
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "em_timer.h"

#endif /* TIMER_H_ */

#define sensor_wait_time	0.08
#define timer_event			0x02


void time_init(void);
void TIMER0_IRQHandler(void);
