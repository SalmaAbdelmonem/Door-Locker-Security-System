/******************************************************************************
 *
 * File Name: HMInterfaceEcu.c
 *
 * Description: Source file for the human machine interface ECU
 *
 *Author: Salma AbdelMonem
 *
 *******************************************************************************/

#include "HMInterfaceEcu.h"

/*******************************************************************************
 *                             Main Function                                   *
 *******************************************************************************/

int main(void)
{
	SREG |= (1<<7); //setting I-bit by one
	LCD_init(); //initialize LCD
	UART_init(&UART_Config); //initialize UART

	LCD_clearScreen(); //clear LCD screen
	LCD_displayString( "DOOR LOCKER" ); //display DOOR LOCKER on LCD line 1
	LCD_moveCursor(1,0);
	LCD_displayString( "SECURITY SYSTEM" ); //display SECURITY SYSTEM on LCD line 2
	_delay_ms(3000); //hold display of DOOR LOCKER SECURITY SYSTEM on LCD for 3secs

	while(1)
	{

		if( g_setPass == 0) //check if password is not set yet
		{
			Set_Password();
		}

		else if( g_setPass == 1) //check if password is set
		{
			Main_Options();
		}

// part to check if its first time to use the system or not
//		Check_First_Use();
//		if ( g_firstUse == 1 )
//		{
//			Set_Password();
//			g_firstUse = 0;
//		}
//		else if( g_firstUse == 0 )
//		{
//			Main_Options();
//		}
	}
}

/**************************************************************************************************
 *                             Frequently Used Functions Definitions                              *
 **************************************************************************************************/

/***************************************************************************************************
 * [Function Name]:   Main_Options
 *
 * [Description]:     Function which is hold choices of open the door or change the password.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Main_Options(void)
{

	LCD_clearScreen();
	LCD_displayString( "(+):Open Door" ); //display (+):Open Door on LCD line 1
	LCD_moveCursor(1,0);
	LCD_displayString( "(-):Change Pass" ); //display (-):Change Pass on LCD line 2

	switch( KEYPAD_getPressedKey() ) // switch to check which condition is chosen + or -
	{
	case '+': //open the door choice
		LCD_clearScreen();
		LCD_displayString( "ENTER YOUR PASS:" );
		LCD_moveCursor(1,0x05);

		Enter_Password( g_password );
		Send_Command( CHECK_PASS ); //command to tell Control ECU check if the password is correct or not
		Send_Password( g_password ); //send entered password to Control ECU

		switch (Receive_Command()) //switch check which command is received from Control ECU
		{
		case MATCHED_PASS: //if entered password is correct
			Send_Command( OPEN_DOOR ); //send command open the door to Control ECU
			Door_State_Display();
			break;

		case UNMATCHED_PASS: //if entered password is not correct
			Wrong_Password_Display();
			break;
		}
		break;

		case '-': //set password choice
			LCD_clearScreen();
			LCD_displayString( "ENTER YOUR PASS:" );
			LCD_moveCursor(1,0x05);
			g_entryCounter = 0;

			Enter_Password( g_password );
			Send_Command( CHECK_PASS );
			Send_Password(g_password);

			switch ( Receive_Command() )
			{
			case MATCHED_PASS: //if entered password is correct
				Set_Password();
				break;

			case UNMATCHED_PASS: //if entered password is not correct
				Wrong_Password_Display();
				break;
			}
			break;
	}
}

/***************************************************************************************************
 * [Function Name]:   Check_First_Use
 *
 * [Description]:     Function to Check if this system is used before or not.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

//
//void Check_First_Use(void)
//{
//	Send_Command( CHECK_FIRST_USE );
//	switch ( Receive_Command() )
//	{
//	case FIRST_TIME_USE:
//		g_firstUse = 1;
//		break;
//
//	case NOT_FIRST_TIME_USE:
//		g_firstUse = 0;
//		break;
//	}
//
//}

/***************************************************************************************************
 * [Function Name]:   Set_Password
 *
 * [Description]:     Function to Set new password and Store it in a global array.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Set_Password(void)
{
	g_setPass = 0;
	if(g_entryCounter==0)
	{
		g_entryCounter++;
		LCD_clearScreen();
		LCD_displayString( "ENTER NEW PASS:" );
		LCD_moveCursor(1,0x05);

		Enter_Password( g_pass );
		Send_Command( PASSWORD_ENTERED );
		Send_Password( g_pass );
	}

	if(g_entryCounter==1)
	{
		g_entryCounter++;
		LCD_clearScreen();
		LCD_displayString( " RE-ENTER PASS:");
		LCD_moveCursor(1,0x05);

		Enter_Password(g_pass);
		Send_Command(PASSWORD_RE_ENTERED);
		Send_Password(g_pass);
	}

	if (g_entryCounter==2)
	{
		g_entryCounter = 0;
		switch( Receive_Command() )
		{
		case MATCHED_PASS:
			Matched_Pass_Display();
			g_setPass = 1;
			break;

		case UNMATCHED_PASS:
			Unmatched_Pass_Display();
			Set_Password();
			break;
		}
	}
}

/***************************************************************************************************
 * [Function Name]:   Enter_Password
 *
 * [Description]:     Function to take password from user and Store it in a global array.
 *
 * [Arguments]:       Array its size is 5 of type unsigned character.
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Enter_Password( uint8 a_pass[] )
{

	for(uint8 i = 0; i < PASSWORD_ELEMENTS; i++)
	{
		g_pressedKey = KEYPAD_getPressedKey();
		_delay_ms(1500);

		if( ( g_pressedKey >= 0 ) && ( g_pressedKey <= 9) && (g_pressedKey != '=') )
		{
			a_pass[i] = g_pressedKey;
			LCD_displayCharacter('*');
//			LCD_intgerToString(g_pressedKey);
		}
		else if (( g_pressedKey == '*' ) || ( g_pressedKey == '/') || (g_pressedKey == '+') || (g_pressedKey == '-') )
		{
			i--;
		}
	}

	g_pressedKey = KEYPAD_getPressedKey();
	_delay_ms(1500);

	while(g_pressedKey != '=')
	{
		g_pressedKey = KEYPAD_getPressedKey();
		_delay_ms(1500);
	}
}

/***************************************************************************************************
 * [Function Name]:   Send_Password
 *
 * [Description]:     Function to Send password to Control ECU.
 *
 * [Arguments]:       Array its size is 5 of unsigned character.
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Send_Password( uint8 a_enteredPass[] )
{
	UART_sendByte(READY);

	while( UART_recieveByte() != READY ){};

	for(uint8 i = 0; i < PASSWORD_ELEMENTS; i++)
	{
		UART_sendByte( a_enteredPass[i] );
	}

	while( UART_recieveByte() != DONE ){};
}

/***************************************************************************************************
 * [Function Name]:   Wrong_Password_Display
 *
 * [Description]:     Function to Display WRONG PASSWORD when a wrong one is entered and
 * count num of wrong trials and if they exceed 3 times it sends WRONG_PASS command to Control
 * ECU and call Buzeer_State_Display.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Wrong_Password_Display(void)
{
	LCD_clearScreen();
	LCD_displayString( " WRONG PASSWORD");
	_delay_ms(3000);
	g_wrongEntry++;

	if(g_wrongEntry == 3)
	{
		Send_Command( WRONG_PASS );
		Buzzer_State_Display();
	}
}

/***************************************************************************************************
 * [Function Name]:   Buzzer_State_Display
 *
 * [Description]:     Function to Display state of buzzer when a wrong password is entered
 * three times in a row.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Buzzer_State_Display(void)
{
	LCD_clearScreen();
	Timer_init( &Timer_Config );
	Timer0_setCallBack( Timer_tickCounterBuzzerMessage );

	while( g_tick != TIMER02_OV_TICKS_1MIN_1024 )
	{
		LCD_moveCursor(0,0x06);
		LCD_displayString( "THEIF" );
		LCD_moveCursor(1,0x06);
		LCD_displayString( "THEIF" );
	}
}

/***************************************************************************************************
 * [Function Name]:   Matched_Pass_Display
 *
 * [Description]:     Function to Display if the password is matched.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Matched_Pass_Display(void)
{
	LCD_clearScreen();
	LCD_displayString( "  PASS IS SET" );
	_delay_ms(3000);
}

/***************************************************************************************************
 * [Function Name]:   Unmatched_Pass_Display
 *
 * [Description]:     Function to Display if the password is unmatched.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Unmatched_Pass_Display(void)
{
	LCD_clearScreen();
	LCD_displayString( " UNMATCHED PASS" );
	_delay_ms(3000);
}

/***************************************************************************************************
 * [Function Name]:   Door_State_Display
 *
 * [Description]:     Function to Display the state of the door while opening and closing.
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Door_State_Display(void)
{

	LCD_clearScreen();
	Timer_init( &Timer_Config );
	Timer0_setCallBack( Timer_tickCounterStateMessage );

	LCD_moveCursor(0,0);
	LCD_displayString( "DOOR IS OPENING");
	while( g_tick <= TIMER02_OV_TICKS_15SEC_1024 ){};

	LCD_moveCursor(0,0);
	LCD_displayString( "  DOOR IS OPEN  ");
	while( (g_tick <= TIMER02_OV_TICKS_18SEC_1024) && (g_tick > TIMER02_OV_TICKS_15SEC_1024) ){};

	LCD_moveCursor(0,0);
	LCD_displayString("DOOR IS CLOSING");
	while( (g_tick < TIMER02_OV_TICKS_33SEC_1024) && (g_tick > TIMER02_OV_TICKS_18SEC_1024) ){};

	LCD_moveCursor(0,0);
	LCD_displayString("DOOR IS LOCKED");
	_delay_ms(4000);

}

/***************************************************************************************************
 * [Function Name]:   Timer_tickCounterBuzzerMessage
 *
 * [Description]:     Function which is call back fn of timer refer to, Control buzzer state
 * display using timer
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Timer_tickCounterBuzzerMessage(void)
{
	g_tick++;
	if( g_tick == TIMER02_OV_TICKS_1MIN_1024 )
	{
		Timer_deInit(TIMER0);
		g_tick=0;
		g_wrongEntry = 0;
	}
}

/***************************************************************************************************
 * [Function Name]:   Timer_tickCounterStateMessage
 *
 * [Description]:     Function which is call back fn of timer refer to, Control door state
 * display using timer
 *
 * [Arguments]:       void
 *
 * [Returns]:         void
 ***************************************************************************************************/

void Timer_tickCounterStateMessage(void)
{
	g_tick++;

	if( g_tick == TIMER02_OV_TICKS_33SEC_1024 )
	{
		Timer_deInit(TIMER0);
		g_tick=0;
		g_wrongEntry = 0;
	}
}

/***************************************************************************************************
 * [Function Name]:   Send_Command
 *
 * [Description]:     Function to Send command to Control ECU
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
 * [Description]:     Function to Receive command from Control ECU
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

/**************************************************************************************************/

