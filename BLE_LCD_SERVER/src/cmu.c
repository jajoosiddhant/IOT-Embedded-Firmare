//***********************************************************************************
// Include files
//***********************************************************************************
#include "src/cmu.h"
#include "src/gpio.h"
#include "src/letimer.h"


//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void cmu_init(void)
{
	if ( lem == sleepEM0 || lem == sleepEM1 || lem == sleepEM2 || lem==sleepEM3)
		{
			//Enabling Oscillator for designated Energy Modes
			CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

			//Configure the oscillator to the peripheral clock Branch
			CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);
		}
	else
		{
			//Enabling Oscillator for designated Energy Modes
			CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);

			//Configure the oscillator to the peripheral clock Branch
			CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);
		}

	// CLOCK Enable for CLock tree
	CMU_ClockEnable(cmuClock_LFA, true);

	// CLOCK Enable for Core LE.
	CMU_ClockEnable (cmuClock_CORELE, true);

	// CLOCK Enable for LETIMER0.
	CMU_ClockEnable (cmuClock_LETIMER0, true);

	// CLOCK Enable for GPIO
	CMU_ClockEnable(cmuClock_GPIO, true);

	CMU_ClockEnable(cmuClock_HFPER, true);

	//CLOCK Enable For I2C
	CMU_ClockEnable(cmuClock_I2C0, true);

	//CLOCK Enable For TIMER0
	CMU_ClockEnable(cmuClock_TIMER0, true);

}
