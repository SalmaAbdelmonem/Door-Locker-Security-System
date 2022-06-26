/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER AVR driver
 *
 * Author: Salma AbdelMonem
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
#include "gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID              PORTC_ID
#define BUZZER_PIN_ID               PIN6_ID
#define LED_PIN_ID                  PIN7_ID

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	OFF,ON
}Buzzer_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void Buzzer_init(void);
void Buzzer_state(Buzzer_State state);

#endif /* BUZZER_H_ */
