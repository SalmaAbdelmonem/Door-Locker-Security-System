/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the TIMER AVR driver (support all timers)
 *
 *Author: Salma AbdelMonem
 *
 *******************************************************************************/

#include "timer.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_Timer0)(void) = NULL_PTR;

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_Timer1)(void) = NULL_PTR;

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_Timer2)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr_Timer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_callBackPtr_Timer0)();
	}
}

/********************************************************************************/

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr_Timer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr_Timer0)();
	}
}

/********************************************************************************/

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr_Timer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr_Timer1)();
	}
}

/********************************************************************************/

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr_Timer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr_Timer1)();
	}
}

/********************************************************************************/

ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr_Timer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_Timer2)();
	}
}

/********************************************************************************/

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr_Timer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_Timer2)();
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	switch(Config_Ptr->timer_id)
	{
	case TIMER0:

		TCCR0 |= (1<<7);
		TCNT0 = (TCNT0 & 0x00) | (Config_Ptr->init_value); //Set Timer initial value
		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock); //Set Timer clock

		switch(Config_Ptr->timer_mode)
		{
		case OVERFLOW:

			TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
			TCCR0 = (TCCR0 & 0x07) | (Config_Ptr->timer_mode); //Set Timer mode
			break;

		case COMPARE:

			TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt
			OCR0 = (OCR0 & 0x00) | (Config_Ptr->compare_value); //Set Timer compare value
			TCCR0 = (TCCR0 & 0x07) | (Config_Ptr->timer_mode); //Set Timer mode
			break;
		}
		break;

		/***************************************************************************************/

		case TIMER1:


			TCNT1 = (TCNT1 & 0x00) | (Config_Ptr->init_value); //Set Timer initial value
			TCCR1A = 0x08; //Set Timer mode
			TCCR1B = (TCCR1B & 0x07) | (Config_Ptr->clock); //Set Timer clock

			switch(Config_Ptr->timer_mode)
			{
			case OVERFLOW:

				TIMSK |= (1<<TOIE1); /* Enable Timer1 OverFlow A Interrupt */
				TCCR1B = (TCCR1B & 0x07) | ((Config_Ptr->timer_mode)<<3); //Set Timer mode
				break;

			case COMPARE:

				TCCR1B = (TCCR1B & 0x07) | ((Config_Ptr->timer_mode)<<3); //Set Timer mode
				TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */
				OCR1A = (OCR1A & 0x00) | (Config_Ptr->compare_value); //Set Timer compare value
				break;
			}
			break;

			/***************************************************************************************/
			case TIMER2:


				TCCR2 = (1<<FOC2);
				TCNT2 = (TCNT2 & 0x00) | (Config_Ptr->init_value); //Set Timer initial value
				TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->clock2); //Set Timer clock

				switch(Config_Ptr->timer_mode)
				{
				case OVERFLOW:

					TIMSK |= (1<<TOIE2); // Enable Timer2 Overflow Interrupt
					TCCR2 &= ~(1<<WGM21) & ~(1<<WGM20); //Set Timer mode
					break;

				case COMPARE:

					TIMSK |= (1<<OCIE2); // Enable Timer0 Compare Interrupt
					OCR2 = (OCR2 & 0x00) | (Config_Ptr->compare_value); //Set Timer compare value
					TCCR2 |= (1<<WGM21); //Set Timer mode
					break;
				}
				break;
	}
}
/***************************************************************************************/
void Timer_deInit(Timer_Id timer_id)
{
	switch(timer_id)
	{
	case TIMER0:


		/* Clear All Timer0 Registers */
		TCNT0 = 0;
		TCCR0 = 0;
		OCR0 = 0;
		break;

	case TIMER1:

		/* Clear All Timer1 Registers */
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		OCR1A = 0;
		OCR1B = 0;
		break;

	case TIMER2:

		/* Clear All Timer2 Registers */
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2 = 0;
		break;
	}
}

/***************************************************************************************/

void Timer0_setCallBack(void(*t_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_Timer0 = t_ptr;
}

/***************************************************************************************/

void Timer1_setCallBack(void(*t_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_Timer1 = t_ptr;
}

/***************************************************************************************/

void Timer2_setCallBack(void(*t_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_Timer2= t_ptr;
}

/***************************************************************************************/

