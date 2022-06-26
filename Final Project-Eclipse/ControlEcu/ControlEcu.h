/******************************************************************************
 *
 * File Name: ControlEcu.c
 *
 * Description: Source file for the Control ECU
 *
 *Author: Salma AbdelMonem
 *
 *******************************************************************************/

#ifndef CONTROLECU_H_
#define CONTROLECU_H_

#include "timer.h"
#include "dc_motor.h"
#include "uart.h"
#include "buzzer.h"
#include "eeprom.h"
#include <avr/io.h>
#include <util/delay.h>

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
uint8 g_pass[5];
uint8 g_rePass[5];
uint8 g_savedPass[5];
uint8 g_password[5];
uint8 g_check;

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

void Check_Password( uint8 a_enteredPass[], uint8 a_enteredPass2[] );
void Read_Password( uint8 a_savedPass[] );
void Save_Password(uint8 a_addPass[]);
void Receive_Password( uint8 a_enteredPass[] );
void Send_Command( uint8 command );
uint8 Receive_Command(void);
void Timer_tickCounterMotor(void);
void Timer_tickCounterBuzzer(void);

#endif /* CONTROLECU_H_ */
