//***********************************************************************************
// Include files
//***********************************************************************************

#include "src/letimer.h"
#include "src/gpio.h"
#include "em_letimer.h"
#include "em_core.h"
#include "em_cmu.h"


//#include <stdint.h>
//#include <stdbool.h>


//***********************************************************************************
// defined files
//***********************************************************************************



//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************

void LETIMER0_IRQHandler(void)
{
	int flags;

	//Disable all interrupts
	CORE_ATOMIC_IRQ_DISABLE();

	//Get Pending Interrupts
	flags = LETIMER_IntGet(LETIMER0);

	//Check interrupt due to Underflow
	if (flags & LETIMER_IF_UF)
	{
		//Check interrupt due to Underflow
		LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF);
	}

	/*Update external event Scheduler Values*/
	EXT_EVENT |= LETIMER_EVENT;

	gecko_external_signal(EXT_EVENT);

	//Enable all interrupts
	CORE_ATOMIC_IRQ_ENABLE();
}


/*
 * Enable LETIMER Clocks
 */

void letimer_cmu_init()
{
	//Enabling Oscillator for designated Energy Modes
	CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);

	//Configure the oscillator to the peripheral clock Branch
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);

	// CLOCK Enable for CLock tree
	CMU_ClockEnable(cmuClock_LFA, true);

	// CLOCK Enable for Core LE.
	CMU_ClockEnable (cmuClock_CORELE, true);

	//CLock Enable for LETIMER0
	CMU_ClockEnable (cmuClock_LETIMER0, true);
}

void letimer_init(void)
{
	const LETIMER_Init_TypeDef regs =
	{
			 .enable         = false,                   /* Start counting when init completed. */

			  .debugRun       = false,                  /* Counter shall not keep running during debug halt. */

			  .comp0Top       = true,                   /* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */

			  .bufTop         = false,                  /* Don't load COMP1 into COMP0 when REP0 reaches 0. */

			  .out0Pol        = 0,                      /* Idle value for output 0. */

			  .out1Pol        = 0,                      /* Idle value for output 1. */

			  .ufoa0          = letimerUFOANone,         /* PWM output on output 0 */

			  .ufoa1          = letimerUFOANone,       /* PWM output on output 1*/

			  .repMode        = letimerRepeatFree       /* Count until stopped */
	};

	// Initialize Letimer0
	LETIMER_Init(LETIMER0, &regs);

	//Changing the frequency depending on the Energy MOde
	prescale();

	//Interrupt Flag Clear Register
//	LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF);

	//Enable LETIMER0 COMP1 and Underflow Interrupt
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF);

	//Enable NVIC interrupt
	NVIC_EnableIRQ(LETIMER0_IRQn);

	//Interrupt Flag Clear Register
	LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF);
}


/*
 * Function to start LETIMER counting
 */
void letimer_start()
{
	//Enable The peripheral LETIMER0
//	LETIMER_Enable(LETIMER0, true);
	// Start the timer
	LETIMER0->CMD=LETIMER_CMD_START;
}


/*
 * Function to stop LETIMER counting
 */
void letimer_stop(void)
{
	LETIMER0->CMD=LETIMER_CMD_STOP;
}

/*
 * Function to prescale the Clock Frequency
 */
void prescale(void)
{
	CMU_ClockDivSet(cmuClock_LETIMER0,8);
	totalticks = wait_timeperiod * 125;
	LETIMER_CompareSet(LETIMER0,0,totalticks);
	while (LETIMER0->SYNCBUSY!=0)
		return;
}
