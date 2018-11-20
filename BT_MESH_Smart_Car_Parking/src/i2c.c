/*
 * i2c.c
 *
 *  Created on: Nov 18, 2018
 *      Author: user1
 */

#include "i2c.h"
#include "gpio.h"

void i2c_init(void)
{
	//CLOCK Enable For I2C
	CMU_ClockEnable(cmuClock_I2C0, true);

	//Configure SCL Line
	GPIO_PinModeSet( SCLPORT , SCLPIN , gpioModeWiredAnd , 1 );

	//		Configure SDA Line
	GPIO_PinModeSet( SDAPORT , SDAPIN , gpioModeWiredAnd , 1 );

	//Setting up SCL and SDA
	I2C0->ROUTEPEN |= I2C_ROUTEPEN_SCLPEN | I2C_ROUTEPEN_SDAPEN;

	I2C0->ROUTELOC0 |= (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SCLLOC_MASK)) //bit mask for sclloc
							| I2C_ROUTELOC0_SCLLOC_LOC14;


	I2C0->ROUTELOC0 |= (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SDALOC_MASK))
							| I2C_ROUTELOC0_SDALOC_LOC16;

	const I2C_Init_TypeDef i2cregs = I2C_INIT_DEFAULT;

	//Initialize I2C0
	I2C_Init( I2C0 , &i2cregs );


	// Enable The I2C Peripheral
	I2C_Enable( I2C0 , true );

	//Reset I2C Slave if Required
	for (int i=0; i<9 ; i++)
	{
		GPIO_PinOutClear( SCLPORT , SCLPIN );
		GPIO_PinOutSet( SDAPORT , SDAPIN );
	}

	//Syncing the bus and its peripherals
	if (I2C0->STATE & I2C_STATE_BUSY)
	{
		I2C0->CMD = I2C_CMD_ABORT;
	}


	//Clear all Interrupts
	I2C0->IFC= 0x00000000;


}


void i2c_disable (void)
{

	I2C0->ROUTEPEN &= ~(I2C_ROUTEPEN_SCLPEN | I2C_ROUTEPEN_SDAPEN);

	I2C_Enable( I2C0 , false );

	GPIO_PinOutClear( SDAPORT , SDAPIN );
	GPIO_PinOutClear( SCLPORT , SCLPIN );
}

uint8_t read8 (uint8_t address)
{

	//Initialize the TX Buffer
	I2C0->TXDATA = (0x4C<<1)|(0x00);
	//Send Start Command
	I2C0->CMD= I2C_CMD_START;
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;
	//Initialize the TX Buffer with register address
	I2C0->TXDATA = address;
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;
	//Send Start Command
	I2C0->CMD= I2C_CMD_START;
	//Initialize the TX Buffer with slave address and read commmand
	I2C0->TXDATA =(0x4C<<1)|(0x01);
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC =I2C_IFC_ACK;
	while ((I2C0->IF & I2C_IF_RXDATAV)==0);
	temp = I2C0-> RXDATA;
	tempshift= temp<<8;
	I2C0->CMD = I2C_CMD_ACK;
	while ((I2C0->IF & I2C_IF_RXDATAV)==0);
	temp1 = I2C0-> RXDATA;
	finaltemp = tempshift | temp1;
	//Send NACK Command
	I2C0->CMD = I2C_CMD_NACK;
	//Send Stop Command
	I2C0->CMD = I2C_CMD_STOP;
	return (temp);
}

uint16_t read16 (uint8_t address)
{

	//Initialize the TX Buffer
	I2C0->TXDATA = (0x4C<<1)|(0x00);
	//Send Start Command
	I2C0->CMD= I2C_CMD_START;
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;
	//Initialize the TX Buffer with register address
	I2C0->TXDATA = address;
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;
	//Send Start Command
	I2C0->CMD= I2C_CMD_START;
	//Initialize the TX Buffer with slave address and read commmand
	I2C0->TXDATA =(0x4C<<1)|(0x01);
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC =I2C_IFC_ACK;
	while ((I2C0->IF & I2C_IF_RXDATAV)==0);
	temp = I2C0-> RXDATA;
	I2C0->CMD = I2C_CMD_ACK;
	while ((I2C0->IF & I2C_IF_RXDATAV)==0);
	temp1 = I2C0-> RXDATA;
	tempshift= temp1<<8;
	finaltemp = tempshift | temp;
	//Send NACK Command
	I2C0->CMD = I2C_CMD_NACK;
	//Send Stop Command
	I2C0->CMD = I2C_CMD_STOP;

	return (finaltemp);
}

void write8 (uint8_t address , uint8_t data)
{

	//Initialize the TX Buffer
	I2C0->TXDATA = (0x4C<<1)|(0x00);
	//Send Start Command
	I2C0->CMD= I2C_CMD_START;
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;
	//Initialize the TX Buffer
	I2C0->TXDATA = address;
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;
	//Initialize the TX Buffer with data
	I2C0->TXDATA =(data);
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC =I2C_IFC_ACK;
	//Send Stop Command
	I2C0->CMD = I2C_CMD_STOP;
}


void write16 (uint8_t address , uint16_t data)
{
	//Initialize the TX Buffer
	I2C0->TXDATA = (0x4C<<1)|(0x00);
	//Send Start Command
	I2C0->CMD= I2C_CMD_START;
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;
	//Initialize the TX Buffer
	I2C0->TXDATA = address;
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;
	//Initialize the TX Buffer with data
	I2C0->TXDATA =(data & 0xFF);
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC =I2C_IFC_ACK;
	I2C0->TXDATA =(data>>8);
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0);
	//Clear the Interrupt
	I2C0->IFC =I2C_IFC_ACK;
	//Send Stop Command
	I2C0->CMD = I2C_CMD_STOP;
}


void proximity_powerup_config()
{
		uint8_t setting = read8(ICSR);
		setting &= 0b11110000; //clears writable bits
		setting |= INT_CLR_REG | INT_CLR | INT_PIN_TYPE | INT_LOHI; //change bits to enable interrupt
		write8(ICSR, setting);
		setting = read8(INTERRUPTS);
		setting &= 0b00000000; //Clears bits
		setting |= INTSRC_DATA | INTSRC_M2H | INTSRC_H2M | INTSRC_RST; //Enables interrupt when data is ready
		write8(INTERRUPTS, setting);
		//Configure I2C Interface
		write8(CONFIGURE_I2C, 0x65); //0b.0110.0101 = Address increment, auto increment, host debug, MCPU debug
		//Set initialization - Magic from datasheet. Write 0x05 to 0x15 location.
		write16(CONFIGURE_PMU, 0x0500); //0b.0000.0101.0000.0000 //Patch_code_id_en, Patch_mem_en

		//if (gotooffmode() == false) return (false);

		//Set initialization - Magic from datasheet. Write 0x06 to 0x15 location.
		write16(CONFIGURE_PMU, 0x0600); //MCPU_Init_state, Patch_mem_en
		//if (gotoonmode() == false) return (false); //MCPU never turned on

	//	setpeak
	//	setthreshold

		write16(CONFIGURE_B, 0x10FF); //Set valid pixel. Set MSP430 default config.
		write16(CONFIGURE_HW_0, 0x07D0); //Set saturation threshold = 2,000.
		write16(CONFIGURE_HW_1, 0x5008); //Frequecy = 5. Low level threshold = 8.
		write16(CONFIGURE_HW_2, 0xA041); //Integration time = 10 * (6500-20)/15)+20 = 4.340ms. Plus reserved magic.
		write16(CONFIGURE_HW_3, 0x45D4); //Enable harmonic cancellation. Enable auto adjust of integration time. Plus reserved magic.
		//if (gotostandbymode() == false) return (false); //Error - MCPU never went to standby

		//Whew! We made it through power on configuration
}


void proximity_config (void)
{

	//Get the calibration data via the 0x0006 mailbox command
	  //getCalibrationData(); //Skipped

	//if (gotostandbymode() == false) return (false); //Error - MCPU never went to standby

	//Now assume user will want sensor in measurement mode

	//Set initialization - Magic from datasheet. Write 0x05 to 0x15 location.
	write16(CONFIGURE_PMU, 0x0500); //Patch_code_id_en, Patch_mem_en

//	if (gotooffmode() == false) return (false); //Error - MCPU never turned off

	//Write calibration data
	//Skipped

	//Set initialization - Magic from datasheet. Write 0x06 to 0x15 location.
	write16(CONFIGURE_PMU, 0x0600); //MCPU_Init_state, Patch_mem_en

//	if (gotoonmode() == false) return (false); //Error - MCPU never turned on

//	return (true); //Success! Sensor is ready for measurements

}


uint8_t proximity_takemeasurement(void)
{
  if (gotomeasurementmode() == false) return (CODE_FAILED_TIMEOUT); //Error - Timeout
  //New data is now available!

  //Read result
  uint16_t resultRegister = read16(RESULT);

  if (resultRegister & 0x7FFF) //Reading is valid
  {
    errorCode = (resultRegister >> 13) & 0x03;

    if (errorCode == 0)
    {
      distance = (resultRegister >> 2) & 0x07FF; //Distance is good. Read it.
      //Serial.println("Distance field valid");

      //Read confidence register
      confidenceRegister = read16(RESULT_CONFIDENCE);
      validPixels = confidenceRegister & 0x0F;
      confidenceValue = (confidenceRegister >> 4) & 0x07FF;
    }

    return (errorCode);

  }
  else
  {
    //Reading is not vald
    return (CODE_FAILED_NOT_NEW); //Error code for reading is not new
  }

}


bool gotostandbymode()
{
	write8(COMMAND,0x90);

	for (uint8_t x= 0;x<10;x++)
	{
		if ((read16(DEVICE_STATUS) & 0x001F) == 0x0000 )
		{
			return(true);
		}
	}
return (false);
}

bool gotooffmode(void)
{
  //Set MCPU_OFF
  write8(COMMAND, 0x91); //0b.1001.0001 = Go MCPU off state. Set valid command.

  //Check MCPU_OFF Status
  for (uint8_t x = 0 ; x < 10 ; x++)
  {
    if ( (read16(DEVICE_STATUS) & 0x001F) == 0x0010) return (true); //MCPU is now off
     //Suggested timeout for status checks from datasheet
  }

  return (false); //Error - MCPU never turned off
}


bool gotoonmode()
{
  //Set MCPU_ON
  write8(COMMAND, 0x92); //0b.1001.0010 = Wake up MCPU to ON mode. Set valid command.

  //Check MCPU_ON Status
  for (uint8_t x = 0 ; x < 10 ; x++)
  {
    if ( (read16(DEVICE_STATUS) & 0x001F) == 0x0018) return (true); //MCPU is now on
     //Suggested timeout for status checks from datasheet
  }

  return (false); //Error - MCPU never turned on
}

bool gotomeasurementmode(void)
{
	{
		//Single measure command
		write8(COMMAND, 0x81); //0b.1000.0001 = Single measurement. Set valid command.

		//Read ICSR Register - Check to see if measurement data is ready
		for (uint8_t x = 0 ; x < 10 ; x++)
		{
			if ( (read8(ICSR) & (1 << 4)) != 0) return (true); //Data is ready!
			//Suggested timeout for status checks from datasheet
		}

		return (false); //Error - Timeout
	}
}


uint16_t getdistance()
{
  return (distance);
}

