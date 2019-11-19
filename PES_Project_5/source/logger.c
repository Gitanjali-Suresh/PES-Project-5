/******************************************************************************
*                                                                             *
*                       PRINCIPLES OF EMBEDDED SOFTWARE                       *
*                   PROJECT 5 - UART Communications with PC                   *
*   Project By:GITANJALI SURESH (GISU9983) & SARAYU MANAGOLI (SAMA2321)       *
*                   Cross Platform IDE: MCUXpresso IDE v11                    *
*                   Compiler: MinGW/GNU gcc v8.2.0 (PC version)               *
*                       Cross-Compiler: ARM GCC (FB version)                  *
*                                                                             *
*******************************************************************************/
/*****************************************************************************
 * LOGGER
 *****************************************************************************/

/********* Standard Header Files *********/
#include <stdint.h>

/********* User defined Header Files *********/
#include "logger.h"
#include "uart.h"
#include "systick.h"

/********* Board specific Header Files *********/
#include "fsl_debug_console.h"

/********** Function to print different modes of the project **********/
void log_Level(eLog_level mode)
{
	if(mode == Status)
	{
		UART0_Transmit_String("\n\rSTATUS: ");
	}
	else if(mode == Debug)
	{
		UART0_Transmit_String("\n\rDEBUG: ");
	}
	else
	{
		UART0_Transmit_String("\n\rTEST: ");
	}
	return;
}

/********** Function to print different function names **********/
void log_Str(eLog_level mode, eFunction_name func, char *string)
{
	log_Level(mode);
	Print_Timestamp();
	if(func == UART0_IRQ)
	{
		UART0_Transmit_String(": UART0_IRQ: ");
	}
	else if(func == UART0_Echo_fun)
	{
		UART0_Transmit_String(": UART0_Echo: ");
	}
	else if(func == UART0_Echo_Poll_fun)
	{
		UART0_Transmit_String(": UART0_Echo_Poll: ");
	}
	else if(func == UART0_Echo_Int_fun)
	{
		UART0_Transmit_String(": UART0_Echo_Int: ");
	}
	else if(func == main_fun)
	{
		UART0_Transmit_String(": main: ");
	}
	else if(func == test_init)
	{
		UART0_Transmit_String(": System_Init: ");
	}
	else if(func == test_shut)
	{
		UART0_Transmit_String(": System_Shutdown: ");
	}
	else if(func == str_write)
	{
		UART0_Transmit_String(": System_WriteString: ");
	}
	else if(func == int_write)
	{
		UART0_Transmit_String(": System_WriteInt: ");
	}
	else if(func == test_recover)
	{
		UART0_Transmit_String(": System_Recover: ");
	}
	else if(func == test_safe)
	{
		UART0_Transmit_String(": System_Safestate: ");
	}
	else
	{
		UART0_Transmit_String(": Test_fun: ");
	}
	UART0_Transmit_String(string);
	return;
}

/********** Function to print a number **********/
void log_Int(uint32_t number)
{
	UART0_Transmit_Number(number);
}

/********** Function to print a character **********/
void log_Char(char ch)
{
	UART0_Transmit(ch);
}

/********** Function to print a string (used only for uc testing) **********/
void log_string(char *str)
{
	UART0_Transmit_String(str);
}
