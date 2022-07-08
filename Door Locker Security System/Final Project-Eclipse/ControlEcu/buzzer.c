/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the BUZZER AVR driver
 *
 * Author: Salma AbdelMonem
 *
 *******************************************************************************/

#include "buzzer.h"

/*******************************************************************************
 *                             Functions Definitions                           *
 *******************************************************************************/

void Buzzer_init(void)
{
	/* Configure the direction of 1 pin as output pin */
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(BUZZER_PORT_ID, LED_PIN_ID, PIN_OUTPUT);

	/* Initialize buzzer to stop mode */
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
	GPIO_writePin(BUZZER_PORT_ID, LED_PIN_ID, LOGIC_LOW);
}

void Buzzer_state(Buzzer_State state)
{
	/* insert state of buzzer in the two first pins of PORT*/
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, state);
	GPIO_writePin(BUZZER_PORT_ID, LED_PIN_ID, state);
}
