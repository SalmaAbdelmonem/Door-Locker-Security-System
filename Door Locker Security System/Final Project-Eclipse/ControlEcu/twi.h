 /******************************************************************************
 *
 * Module: I2C
 *
 * File Name: i2c.h
 *
 * Description: Header file for the I2C AVR driver
 *
 * Author: Salma AbdelMonem
 *
 *******************************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NORMAL_MODE=0x20, FAST_MODE=0X02
}I2c_BitRate;

typedef enum
{
	ONE, FOUR, SIXTEEN, SIXTY_FOUR
}I2c_Prescaler;

typedef struct
{
	I2c_BitRate bit_rate;
	I2c_Prescaler prescaler;
	uint8 address;
}I2c_ConfigType;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(const I2c_ConfigType * Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
