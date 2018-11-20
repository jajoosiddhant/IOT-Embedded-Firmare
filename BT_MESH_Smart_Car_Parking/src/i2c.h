/*
 * i2c.h
 *
 *  Created on: Nov 18, 2018
 *      Author: user1
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdbool.h>
//#include "native_gecko.h"
#include "em_i2c.h"

#define ICSR 0x00
#define INTERRUPTS 0x02
#define COMMAND 0x04
#define DEVICE_STATUS 0x06
#define RESULT 0x08
#define RESULT_CONFIDENCE 0x0A
#define CONFIGURE_A 0x0C
#define CONFIGURE_B 0x0E
#define HOST_TO_MCPU_MAILBOX 0x10
#define MCPU_TO_HOST_MAILBOX 0x12
#define CONFIGURE_PMU 0x14
#define CONFIGURE_I2C 0x1C
#define CONFIGURE_HW_0 0x20
#define CONFIGURE_HW_1 0x22
#define CONFIGURE_HW_2 0x24
#define CONFIGURE_HW_3 0x26
#define MOD_CHIP_ID 0x28


#define RFD77402_MODE_MEASUREMENT 0x01
#define RFD77402_MODE_STANDBY 0x10
#define RFD77402_MODE_OFF 0x11
#define RFD77402_MODE_ON 0x12

#define CODE_VALID_DATA 0x00
#define CODE_FAILED_PIXELS 0x01
#define CODE_FAILED_SIGNAL 0x02
#define CODE_FAILED_SATURATED 0x03
#define CODE_FAILED_NOT_NEW 0x04
#define CODE_FAILED_TIMEOUT 0x05

#define I2C_SPEED_STANDARD        100000
#define I2C_SPEED_FAST            400000


#define INT_CLR_REG 1 //tells which register read clears the interrupt (Default: 1, Result Register)
#define INT_CLR 0 << 1 //tells whether or not to clear when register is read (Default: 0, cleared upon register read)
#define INT_PIN_TYPE 1 << 2 //tells whether int is push-pull or open drain (Default: 1, push-pull)
#define INT_LOHI 0 << 3 //tells whether the interrupt is active low or high (Default: 0, active low)

//Setting any of the following bits to 1 enables an interrupt when that event occurs
#define INTSRC_DATA 1 //Interrupt fires with newly available data
#define INTSRC_M2H 0 << 1//Interrupt fires with newly available data in M2H mailbox register
#define INTSRC_H2M 0 << 2//Interrupt fires when H2M register is read
#define INTSRC_RST 0 << 3 //Interrupt fires when HW reset occurs


uint8_t temp1;
uint16_t temp;
uint16_t tempshift;
uint16_t finaltemp;

uint8_t errorCode;
uint8_t ecode;
uint16_t distance;
uint16_t dist_cm;
uint16_t confidenceRegister;
uint8_t validPixels;
uint16_t confidenceValue;


void i2c_init(void);
void i2c_disable (void);
void proximity_powerup_config();
void proximity_config (void);
void write8 (uint8_t address, uint8_t data);
void write16 (uint8_t address , uint16_t data);
uint8_t read8 (uint8_t address);
uint16_t read16 (uint8_t address);
void I2C0_IRQHandler (void);

bool gotostandbymode(void);
bool gotooffmode(void);
bool gotoonmode(void);
bool gotomeasurementmode(void);
uint16_t getdistance(void);
uint8_t proximity_takemeasurement(void);

#endif /* I2C_H_ */
