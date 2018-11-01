//***********************************************************************************
// Include files
//***********************************************************************************

#include "math.h"
#include "native_gecko.h"
#include "em_letimer.h"
#include "em_core.h"
#include "em_cmu.h"
#include "src/letimer.h"
#include "src/gpio.h"


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

	LETIMER0->IEN = 0x00000000;

	//Enable LETIMER0 Underflow Interrupt
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF);

	//Enable NVIC interrupt
	NVIC_EnableIRQ(LETIMER0_IRQn);

	//Changing the frequency depending on the Energy MOde
	prescale();

	//Interrupt Flag Clear Register
	LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF|LETIMER_IFC_COMP1|LETIMER_IFC_COMP0);

	//Enable The peripheral LETIMER0
	LETIMER_Enable(LETIMER0, true);

	// Start the timer
	//LETIMER0->CMD=LETIMER_CMD_START;

}



void prescale(void)
{
	if ( lem == sleepEM0 || lem == sleepEM1 || lem == sleepEM2 || lem==sleepEM3)
		{
			if (tp>1.99)

		{

			int maxticks = tp*32768;		// Maximum Ticks
			int raise = maxticks/65535;
			int j = pow(2,raise);
			CMU_ClockDivSet(cmuClock_LETIMER0,j);	//Prescaling Frequency
			int newfreq= 32768/j;
			int totalticks = tp * newfreq;			//Ticks corresponding to total Time Period

				LETIMER_CompareSet(LETIMER0,0,totalticks);
				while (LETIMER0->SYNCBUSY!=0)
						return;

		}

			else
		{
			int totalticks = tp * 32768;
			LETIMER_CompareSet(LETIMER0,0,totalticks);
			while (LETIMER0->SYNCBUSY!=0)
								return;

	}
}
	else
		{
				int totalticks = tp * 1000;
					LETIMER_CompareSet(LETIMER0,0,totalticks);
					while (LETIMER0->SYNCBUSY!=0)
										return;
		}
}




void LETIMER0_IRQHandler(void)
{
	//Disable all interrupts
	CORE_ATOMIC_IRQ_DISABLE();

	LETIMER_IntClear(LETIMER0, LETIMER_IFC_COMP0|LETIMER_IFC_COMP1);

	/*Update External event Scheduler Values*/
	ext_event |= letimer_event;
	gecko_external_signal(ext_event);

	LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF );

	//Enable all interrupts
	CORE_ATOMIC_IRQ_ENABLE();
}







