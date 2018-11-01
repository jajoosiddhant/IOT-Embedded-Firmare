/*
 * i2c.c
 *
 *  Created on: Sep 18, 2018
 *      Author: user1
 */

#include "src/i2c.h"

void i2c_init(void)
{
	//blocksleepmode(sleepEM2);

	//Configure SCL Line
		GPIO_PinModeSet( sclport , sclpin , gpioModeWiredAnd , 1 );

		//Configure SDA Line
		GPIO_PinModeSet( sdaport , sdapin , gpioModeWiredAnd , 1 );

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

	//si7021 Sensor Enable
	GPIO_PinOutSet( sensorport , sensorpin );
	int i=0;
	while (i<200000)
	{
		i++;
	}



	//Reset I2C Slave if Required
	for (int i=0; i<9 ; i++)
	{
		GPIO_PinOutClear( sclport , sclpin );
		GPIO_PinOutSet( sdaport , sdapin );
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

	GPIO_PinOutClear( sdaport , sdapin );
	GPIO_PinOutClear( sclport , sclpin );

	//si7021 sensor disable
	GPIO_PinOutClear( sensorport , sensorpin );

	//unblocksleepmode(sleepEM2);
}

uint16_t i2c_driver (void)
{

	//Initialize the TX Buffer
	I2C0->TXDATA = (0x40<<1)|(0x00);

	//Send Start Command
	I2C0->CMD= I2C_CMD_START;


	while ((I2C0->IF & I2C_IF_ACK)==0)
	{
//		if ((I2C0->IF & I2C_IF_NACK)==1)
//		{
//			//reset
//		}
	}


	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;

	//Initialize the TX Buffer
	I2C0->TXDATA = 0xE3;


	//int count=0;
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0)
	{
		//		if ((I2C0->IF & I2C_IF_NACK)==1)
		//		{
		//			count++;
		//			//reset
		//		}
		//		if (count==5000)
					{
						//reset
					}
	}

	//Clear the Interrupt
	I2C0->IFC=I2C_IFC_ACK;

	//Send Start Command
	I2C0->CMD= I2C_CMD_START;


	//Initialize the TX Buffer
	I2C0->TXDATA =(0x40<<1)|(0x01);


	//I2C_Enable(I2C0, I2C_CTRL_AUTOACK);


	//int count=0;
	//Check for an acknowledgment
	while ((I2C0->IF & I2C_IF_ACK)==0)
	{
		//		if ((I2C0->IF & I2C_IF_NACK)==1)
		//		{
		//		count++;
		//			//reset
		//		}
		//		if (count==5000)
				{
					//reset
				}
	}

	//Clear the Interrupt
	I2C0->IFC =I2C_IFC_ACK;

	while ((I2C0->IF & I2C_IF_RXDATAV)==0);

	uint16_t temp = I2C0-> RXDATA;

	temp= temp<<8;

	I2C0->CMD = I2C_CMD_ACK;

	while ((I2C0->IF & I2C_IF_RXDATAV)==0);

	temp |= I2C0-> RXDATA;


	//Clear RXFull interrupt Flag

	//I2C_IntClear( I2C0 , I2C_STATUS_RXFULL );


	//Send NACK Command
	I2C0->CMD = I2C_CMD_NACK;

	//Send Stop Command
	I2C0->CMD = I2C_CMD_STOP;


	//int temp;

	//temp =Si7013_MeasureRHAndTemp(I2C0, 0x10000000);

	return temp;

}

