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
 * SysTICK for TIMESTAMPS
 *****************************************************************************/

/********* Standard Header Files *********/
#include <stdint.h>

/********* Board specific Header Files *********/
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/********* User defined Header Files *********/
#include "systick.h"
#include "logger.h"

uint8_t hour, minute, second, n;

/************** Function to initialize SysTick **************/
void Init_Systick(void)
{
	SysTick->LOAD = (48000000L/160);
	NVIC_SetPriority(SysTick_IRQn, 3);	//setting the priority of SysTick interrupt
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

/************** SysTick Interrupt Handler **************/
void SysTick_Handler(void)
{
	n += 10;
	if(n == 100)
	{
		n = 0;
		second += 1;
		if(second == 60)
		{
			second = 0;
			minute += 1;
			if(minute == 60)
			{
				hour += 1;
				minute = 0;
			}
		}
	}
}

/************** Function to print timestamps **************/
void Print_Timestamp(void)
{
	log_Int(hour);
	log_Char(':');
	log_Int(minute);
	log_Char(':');
	log_Int(second);
	log_Char('.');
	log_Int(n);
}
