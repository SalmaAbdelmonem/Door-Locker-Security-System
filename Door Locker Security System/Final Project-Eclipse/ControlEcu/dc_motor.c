/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dc_motor.c
 *
 * Description: source file for the AVR DC_Motor driver
 *
 * Author: Salma AbdelMonem
 *
 *******************************************************************************/

#include "dc_motor.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void DcMotor_Init(void)
{
	/* Configure the direction for 2 pins of motor as output pins */
	GPIO_setupPinDirection(PORT_ID, PIN_IN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORT_ID, PIN_IN2_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORT_ID, PIN_EN_ID, PIN_OUTPUT);

	/* Initialize motor to stop mode */
	GPIO_writePin(PORT_ID, PIN_IN1_ID, LOGIC_LOW);
	GPIO_writePin(PORT_ID, PIN_IN2_ID, LOGIC_LOW);
}


void DcMotor_Rotate(DcMotor_State state)
{
	/* insert state of motor in the two first pins of PORT B*/
	GPIO_writePort(PORT_ID, state);

	/*set speed of motor by using PWM of Timer 0*/
	GPIO_writePin(PORT_ID, PIN_EN_ID, LOGIC_HIGH);
}
void DcMotor_deInit(void)
{
	/* Disable Dc-Motor */
	GPIO_writePin(PORT_ID, PIN_IN1_ID, LOGIC_LOW);
	GPIO_writePin(PORT_ID, PIN_IN2_ID, LOGIC_LOW);
	GPIO_writePin(PORT_ID, PIN_EN_ID, LOGIC_LOW);
}
