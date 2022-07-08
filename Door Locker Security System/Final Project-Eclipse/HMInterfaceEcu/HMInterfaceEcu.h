
/******************************************************************************
 *
 * File Name: HMInterfaceEcu.h
 *
 * Description: Header file for the human machine interface ECU
 *
 *Author: Salma AbdelMonem
 *
 *******************************************************************************/

#ifndef HMINTERFACEECU_H_
#define HMINTERFACEECU_H_

#include "std_types.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"
#include "HMInterfaceEcu.h"
#include <util/delay.h>
#include <avr/io.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define READY                                    'Y'
#define DONE                                     'D'
#define PASSWORD_ELEMENTS                         5
#define CHECK_PASS                               'K'
#define MATCHED_PASS                             'M'
#define UNMATCHED_PASS                           'U'
#define TIMER02_OV_TICKS_15SEC_1024              458
#define TIMER02_OV_TICKS_18SEC_1024              550
#define TIMER02_OV_TICKS_33SEC_1024              1007
#define TIMER02_OV_TICKS_1MIN_1024               1831
#define TIMER1_OV_TICKS_15SEC_1024                2
#define TIMER1_OV_TICKS_33SEC_1024                4
#define TIMER1_OV_TICKS_1MIN_1024                 7
#define WRONG_PASS                               'W'
#define CHECK_FIRST_USE                          'C'
#define FIRST_TIME_USE                           'F'
#define NOT_FIRST_TIME_USE                       'N'
#define PASSWORD_ENTERED                         'E'
#define PASSWORD_RE_ENTERED                      'R'
#define OPEN_DOOR                                'A'
#define CLOSE_DOOR                               'B'

/************************************************************************
 *                           Global variables                           *
 ************************************************************************/

uint16 g_tick=0;
uint8 command;
uint8 g_wrongEntry=0;
uint8 g_entryCounter=0;
uint8 g_pass[5];
uint8 g_pressedKey;
uint8 g_password[5];
uint8 g_setPass=0;
//uint8 g_firstUse;

/*******************************************************************************
 *                     Dynamic Configurations                                  *
 *******************************************************************************/

/* Set UART config to:
 * Disable parity bit
 * One stop bit
 * Eight-bit mode
 * Baud Rate = 9600
 */
UART_ConfigType UART_Config = {DISABLED, ONE_BIT, EIGHT_BIT,9600};

/* Set Timer config to
 * Timer 0
 * Over-Flow mode
 * Clock = 1024
 * Timer2 no clock
 * Init value = 0
 * Compare value = 256
 */
Timer_ConfigType Timer_Config = {TIMER0, OVERFLOW, F_CPU_1024, NO_CLOCK2, 0, 0xFF};

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Timer_tickCounterStateMessage(void);
void Timer_tickCounterBuzzerMessage(void);
void Door_State_Display(void);
void Unmatched_Pass_Display(void);
void Wrong_Password_Display(void);
void Buzzer_State_Display(void);
void Set_Password(void);
void Send_Password( uint8 a_enteredPass[] );
void Enter_Password( uint8 a_pass[] );
void Send_Command( uint8 command );
uint8 Receive_Command(void);
void Matched_Pass_Display(void);
void Main_Options(void);
//void Check_First_Use(void);

#endif /* HMINTERFACEECU_H_ */
