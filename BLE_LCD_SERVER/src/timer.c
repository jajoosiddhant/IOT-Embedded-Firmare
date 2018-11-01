/*
 * timer.c
 *
 *  Created on: Sep 29, 2018
 *      Author: user1
 */
#include "src/timer.h"
#include "main.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "em_core.h"
#include "native_gecko.h"

void time_init(void)
{
	const TIMER_Init_TypeDef tregs =
	{                                                                                   \
			    false,                 /* Enable timer when initialization completes. */           \
			    false,                /* Stop counter during debug halt. */                       \
			    timerPrescale64,       /* No prescaling. */                                        \
				timerClkSelHFPerClk,  /* Select HFPER clock. */                                   \
			    false,                /* Not 2x count mode. */                                    \
			    false,                /* No ATI. */                                               \
			    timerInputActionNone, /* No action on falling input edge. */                      \
			    timerInputActionNone, /* No action on rising input edge. */                       \
			    timerModeUp,          /* Up-counting. */                                          \
			    false,                /* Do not clear DMA requests when DMA channel is active. */ \
			    false,                /* Select X2 quadrature decode mode (if used). */           \
			    false,                /* Disable one shot. */                                     \
			    false                 /* Not started/stopped/reloaded by other timers. */         \

	};

	// Initialize Letimer0
	TIMER_Init(TIMER0, &tregs);

	TIMER0->IEN = 0x00000000;

	//Enable TIMER0 Overflow Interrupt
	TIMER_IntEnable(TIMER0,TIMER_IEN_OF);

	//Enable NVIC interrupt
	NVIC_EnableIRQ(TIMER0_IRQn);

	//Interrupt Flag Clear Register
	TIMER_IntClear(TIMER0,TIMER_IFC_OF);

	int sensorticks = (sensor_wait_time) * 600000;


	TIMER_TopSet(TIMER0,sensorticks);
	TIMER_CounterSet(TIMER0,0x00);


}

void TIMER0_IRQHandler(void)
{
		//Disable all interrupts
		CORE_ATOMIC_IRQ_DISABLE();

		// Start the timer
		TIMER0->CMD=TIMER_CMD_STOP;

		ext_event |= timer_event;
		gecko_external_signal(ext_event);

		TIMER_IntClear(TIMER0, TIMER_IFC_OF);

		//Enable all interrupts
		CORE_ATOMIC_IRQ_ENABLE();
}
