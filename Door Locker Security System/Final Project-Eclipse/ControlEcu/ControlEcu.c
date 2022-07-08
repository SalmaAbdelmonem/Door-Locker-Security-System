/******************************************************************************
 *
 * File Name: ControlEcu.c
 *
 * Description: Source file for the Control ECU
 *
 *Author: Salma AbdelMonem
 *
 *******************************************************************************/

#include "ControlEcu.h"

/*******************************************************************************
 *                             Main Function                                   *
 *******************************************************************************/

int main(void)
{
	SREG |= (1<<7); //setting I-bit by one
	EEPROM_init(); //initialize EEPROM
	Buzzer_init(); //initialize Buzzer
	DcMotor_Init(); //initialize DC Motor
	UART_init( &UART_Config ); //initialize UART

//	uint8 identityCounter=0;

	while(1)
	{
		switch( Receive_Command() ) //switch to know which command will be executed
		{

//part to check if this system is used before or not
//				case CHECK_FIRST_USE:
//
//					Read_Password( g_savedPass );
//
//					for( uint8 i = 0; i < PASSWORD_ELEMENTS; i++ )
//					{
//						if(g_savedPass[i] == 1)
//						{
//							identityCounter++;
//						}
//					}
//					if (identityCounter == PASSWORD_ELEMENTS)
//					{
//						Send_Command( FIRST_TIME_USE );
//					}
//					else
//					{
//						Send_Command( NOT_FIRST_TIME_USE );
//					}
//
//					break;

		case CHECK_PASS: //check if the entered password is correct or not

			Receive_Password(g_password);
			Read_Password( g_savedPass );
			Check_Password( g_password, g_savedPass );
			Send_Command( g_check );

			break;

		case PASSWORD_ENTERED: //save first entered password in setting new password process in a global array

			Receive_Password( g_pass );

			break;

		case PASSWORD_RE_ENTERED: //save second password in another global array and compare between them

			Receive_Password( g_rePass );

			Check_Password( g_pass, g_rePass );

			if(g_check == MATCHED_PASS)
			{
				Save_Password( g_pass );
			}

			Send_Command( g_check );

			break;

		case OPEN_DOOR: //control the door mmovement

			Timer0_setCallBack( Timer_tickCounterMotor );
			Timer_init( &Timer_Config );

			DcMotor_Rotate(CW);
			while( g_tick <= TIMER02_OV_TICKS_15SEC_1024 ){};

			DcMotor_Rotate(STOP);
			while( (g_tick <= TIMER02_OV_TICKS_18SEC_1024) && (g_tick > TIMER02_OV_TICKS_15SEC_1024) ){};

			DcMotor_Rotate(ACW);
			while( (g_tick < TIMER02_OV_TICKS_33SEC_1024) && (g_tick > TIMER02_OV_TICKS_18SEC_1024) ){};

			break;

		case WRONG_PASS: //enable buzzer in case of 3 times wrong entered

			Timer_init( &Timer_Config );
			Timer0_setCallBack( Timer_tickCounterBuzzer );

			Buzzer_state(ON);
			while( g_tick < TIMER02_OV_TICKS_1MIN_1024 ){};

			Buzzer_state(OFF);

			break;
		}
	}
}

/**************************************************************************************************
 *                            Frequently Used Functions Definitions                               *
 **************************************************************************************************/
/***************************************************************************************************
 * [Function Name]:   Check_Password
 *
 * [Description]:     Function to Check on the entered password and save it in a global array.
 *
 * [Arguments]:       TWO-Arrays their size is 5 of unsigned character.
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Check_Password( uint8 a_enteredPass1[], uint8 a_enteredPass2[] )
{
	uint8 counter = 0;
	for(uint8 i = 0; i < PASSWORD_ELEMENTS; i++)
	{
		if( a_enteredPass1[i] == a_enteredPass2[i] )
		{
			counter++;
		}
	}
	if( counter == PASSWORD_ELEMENTS )
	{
		g_check = MATCHED_PASS;
	}
	else
	{
		g_check = UNMATCHED_PASS;
	}
}

/***************************************************************************************************
 * [Function Name]:   Read_Password
 *
 * [Description]:     Function to Read the password from EEPROM and store it in a global array.
 *
 * [Arguments]:       Array its size is 5 of unsigned character.
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Read_Password( uint8 a_savedPass[] )
{
	for(uint8 i = 0; i < PASSWORD_ELEMENTS; i++)
	{
		EEPROM_readByte( 0x0311+i, &a_savedPass[i] );
		_delay_ms(100);
	}
}

/***************************************************************************************************
 * [Function Name]:   Save_Password
 *
 * [Description]:     Function to Store the password in EEPROM.
 *
 * [Arguments]:       Array its size is 5 of unsigned character.
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Save_Password(uint8 a_addPass[])
{
	uint8 i;
	for(i = 0; i < PASSWORD_ELEMENTS; i++)
	{
		EEPROM_writeByte( 0x0311+i, a_addPass[i] );
		_delay_ms(100);
	}
}

/***************************************************************************************************
 * [Function Name]:   Receive_Password
 *
 * [Description]:     Function to Receive password from HMI ECU and store it in a global array.
 *
 * [Arguments]:       Array its size is 5 of unsigned character.
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Receive_Password( uint8 a_enteredPass[] )
{
	while(UART_recieveByte() != READY){};
	UART_sendByte(READY);

	uint8 i;
	for(i = 0 ; i < PASSWORD_ELEMENTS; i++)
	{
		a_enteredPass[i] = UART_recieveByte();
	}
	UART_sendByte(DONE);
}

/***************************************************************************************************
 * [Function Name]:   Send_Command
 *
 * [Description]:     Function to Send command to HMI ECU.
 *
 * [Arguments]:       Command to tell or check on smth.
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Send_Command( uint8 command )
{
	UART_sendByte(READY);
	while(UART_recieveByte() != READY){};
	UART_sendByte(command);
	while(UART_recieveByte() != DONE){};
}

/***************************************************************************************************
 * [Function Name]:   Receive_Command
 *
 * [Description]:     Function to receive command from HMI ECU.
 *
 * [Arguments]:       void
 *
 * [Returns]:         Command to tell or check on smth.
 ***************************************************************************************************/

uint8 Receive_Command(void)
{
	while(UART_recieveByte() != READY){};
	UART_sendByte(READY);
	command=UART_recieveByte();
	UART_sendByte(DONE);
	return command;
}

/***************************************************************************************************
 * [Function Name]:   Timer_tickCounterMotor
 *
 * [Description]:     Function which is call back fn of timer refer to,
 * Control the operation time of door movement.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Timer_tickCounterMotor(void)
{
	g_tick++;
	if( g_tick == TIMER02_OV_TICKS_33SEC_1024 )
	{
		Timer_deInit(TIMER0);
		DcMotor_deInit();
		g_tick=0;
	}
}

/***************************************************************************************************
 * [Function Name]:   Timer_tickCounterBuzzer
 *
 * [Description]:     Function which is call back fn of timer refer to,
 * Control the operation time of buzzer.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Timer_tickCounterBuzzer(void)
{
	g_tick++;
	if( g_tick == TIMER02_OV_TICKS_1MIN_1024 )
	{
		Timer_deInit(TIMER0);
		Buzzer_state(OFF);
		g_tick=0;

	}
}

/**************************************************************************************************/
