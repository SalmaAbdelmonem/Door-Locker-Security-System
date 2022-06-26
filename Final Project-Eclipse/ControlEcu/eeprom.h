 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: eeprom.h
 *
 * Description: Header file for the EXTERNAL EEPROM Memory
 *
 * Author: Salma AbdelMonem
 *
 *******************************************************************************/


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"
#include "twi.h"

/*******************************************************************************
 *                               Definitions                                   *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void EEPROM_init(void);
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);

#endif /* EXTERNAL_EEPROM_H_ */
