/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the TIMER AVR driver (support all timers)
 *
 *Author: Salma AbdelMonem
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "gpio.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/***************************************************************************************/

typedef enum
{
	TIMER0, TIMER1, TIMER2
}Timer_Id;

typedef enum
{
	OVERFLOW, COMPARE
}Timer_Mode;

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef enum
{
	NO_CLOCK2,F2_CPU_CLOCK,F2_CPU_8,F2_CPU_32,F2_CPU_64,F2_CPU_128,F2_CPU_256,F2_CPU_1024
}Timer_Clock2;

typedef struct
{
	Timer_Id timer_id;
	Timer_Mode timer_mode;
	Timer_Clock clock;
	Timer_Clock2 clock2;
	uint16 init_value;
	uint16 compare_value;
}Timer_ConfigType;


/***************************************************************************************/

void Timer_init(const Timer_ConfigType * Config_Ptr);
void Timer0_setCallBack(void(*t_ptr)(void));
void Timer1_setCallBack(void(*t_ptr)(void));
void Timer2_setCallBack(void(*t_ptr)(void));
void Timer_deInit(Timer_Id timer_id);


#endif /* TIMER_H_ */
