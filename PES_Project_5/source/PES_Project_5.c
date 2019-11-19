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

/********* Standard Header Files *********/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/********* Board specific Header Files *********/
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/********* User defined Header Files *********/
#include "circular_buffer.h"
#include "systick.h"
#include "uart.h"
#include "logger.h"
#include "led.h"
#include "unit_test.h"

/********* Mode definitions *********/
#define DEBUG_MODE
//#define STATUS

/********* Critical Section definitions *********/
#define START_CRITICAL DisableIRQ(UART0_IRQn);
#define END_CRITICAL EnableIRQ(UART0_IRQn);

/***************** Global Variable Declarations ******************/
circ_buf_p c_buf;
uint8_t interrupt_flag = 0, c, break_flag = 0;

/**************** Main Function *****************/
int main(void)
{
	/************** Board Initializations *************/
	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    Init_Systick();				//Systick Initialization
    led_init();					//LED Initialization

    /********* Blue LED Initialization *********/
	blue_on();
	green_off();
	red_off();

#ifndef TEST

#ifdef APP
    buf_status status;	//status variable to indicate the status of circular buffer operations
#endif

#if USE_UART_INTERRUPTS==0 		// Polling version of code

    Init_UART0_Poll(115200);

#elif USE_UART_INTERRUPTS==1 	// Interrupt version of code

    Init_UART0_Interrupt(115200);

#endif

/**************** ECHO Mode operation through polling ****************/
#ifdef ECHO_POLL
    	log_Str(Status,main_fun,"In Echo Mode (Polling).");
    	log_Str(Status,main_fun,"Enter the character: ");
#endif

/**************** ECHO Mode operation through interrupts ****************/
#ifdef ECHO_INT
    	log_Str(Status,main_fun,"In Echo Mode (Interrupt).");
    	log_Str(Status,main_fun,"Enter the character: ");
#endif

/**************** APPLICATION Mode operation through polling ****************/
#ifdef APP_POLL
    	log_Str(Status,main_fun,"In Application Mode (Polling).\n\r");
#endif

/**************** APPLICATION Mode operation through interrupts ****************/
#ifdef APP_INT
    	log_Str(Status,main_fun,"In Application Mode (Interrupt).\n\r");
#endif

    while(1)
    {
    	/**************** ECHO Mode operation through polling ****************/
		#ifdef ECHO_POLL
    		UART0_Echo_Poll();
    		log_Str(Status,main_fun,"Enter the character: ");
		#endif

    	/**************** ECHO Mode operation through interrupts ****************/
		#ifdef ECHO_INT
    		END_CRITICAL
    		UART0_Echo_Int();
    		log_Str(Status,main_fun,"Enter the character: ");
		#endif

		#ifdef APP
    	    uint8_t *buf = malloc(BUF_SIZE * sizeof(uint8_t));	//Creating a pointer to store the circular buffer contents
    	    status = ptr_validity(buf);		//Checking if the buffer pointer is valid

	    	if(status == success)
				#ifdef DEBUG_MODE
	    			log_Str(Debug,main_fun,"Valid buffer pointer.");
				#endif
	    	else
	    	{
				#ifdef DEBUG_MODE
	    			log_Str(Debug,main_fun,"Invalid buffer pointer!");
				#endif
		    	blue_off();
		    	green_off();
		    	red_on();
	    	}

    	    freq_Init();						//Initializing the frequency array
    	    c_buf = buf_init(buf,BUF_SIZE);		//Initializing the circular buffer
    	    status = buf_verify(c_buf);			//Verifying the initialization of the circular buffer
	    	if(status == success)
				#ifdef DEBUG_MODE
	    			log_Str(Debug,main_fun,"Buffer initialized successfully!");
				#endif
	    	else
	    	{
				#ifdef DEBUG_MODE
	    			log_Str(Debug,main_fun,"Buffer initialization failed!");
				#endif
		    	blue_off();
		    	green_off();
		    	red_on();
	    	}

    	    uint8_t *f_buf = malloc(2 * FREQ_SIZE * sizeof(uint8_t));		//Creating another buffer pointer for storing the report
    	    status = ptr_validity(f_buf);									//Validity check of the buffer pointer
	    	if(status == success)
				#ifdef DEBUG_MODE
	    			log_Str(Debug,main_fun,"Valid frequency buffer pointer.");
				#endif
	    	else
	    	{
				#ifdef DEBUG_MODE
	    			log_Str(Debug,main_fun,"Invalid frequency buffer pointer!");
				#endif
		    	blue_off();
		    	green_off();
		    	red_on();
	    	}

    	    circ_buf_p cf_buf = buf_init(f_buf,(FREQ_SIZE * 2));	//Initializing the circular buffer for report
    	    status = buf_verify(cf_buf);							//Verifying the initialization of the circular buffer
	    	if(status == success)
				#ifdef DEBUG_MODE
	    			log_Str(Debug,main_fun,"Frequency Buffer initialized successfully!");
				#endif
	    	else
	    	{
				#ifdef DEBUG_MODE
	    			log_Str(Debug,main_fun,"Frequency Buffer initialization failed.");
				#endif
		    	blue_off();
		    	green_off();
		    	red_on();
	    	}

//    	    log_Str(Status,main_fun,"Buffer status before adding any values.");
//    	    buf_print(c_buf);

    	    log_Str(Status,main_fun,"Enter the values to be written to the buffer and hit ENTER to generate the report.\n\r");
    	    while((c = UART0_Receive()) != 13)
    	    {
    	    	break_flag = 0;
        	    while(!buf_full(c_buf))
        	    {
        	    	/**************** APPLICATION Mode operation through polling ****************/
    				#ifdef APP_POLL
        	    		UART0_App_Poll(c_buf);
        	    		c = UART0_Receive();
        	    		if(c == 13)
        	    		{
        	    			break_flag = 1;
        	    			break;
        	    		}
    				#endif

        	    	/**************** APPLICATION Mode operation through interrupts ****************/
    				#ifdef APP_INT
        	    		END_CRITICAL
        	    		UART0_App_Int(c_buf);
        	    		c = UART0_Receive();
        	    		if(c == 13)
        	    		{
        	    			break_flag = 1;
        	    			break;
        	    		}
    				#endif
        	    }
        	    /********* Emptying the buffer if the buffer overflows *********/
        	    if(buf_full(c_buf) == 1)
        	    {
            	    while(!buf_empty(c_buf))
            	    {
            	        uint8_t data;
            	        status = buf_read(c_buf,&data);

        	        	if(status == fail)
        	        	{
							#ifdef DEBUG_MODE
        	        			log_Str(Debug,main_fun,"Unable to read data!");
							#endif
        	        		blue_off();
        	    		    green_off();
        	    		    red_on();
        	        	}
            	    }
        	    }
        	    if(break_flag)	//To break out of the loop to generate the report when ENTER key(13) is pressed
        	    	break;
    	    }
    	    START_CRITICAL
    	    write_fbuf(cf_buf);
    	    log_string("\n\r");
    	    log_Str(Status,main_fun,"The report is as follows: ");
			#ifdef DEBUG_MODE
    	    	log_Str(Debug,main_fun,"Reading back values from frequency buffer...");
			#endif
    	    log_string("\n\r");
    	    /********** Printing the report from the frequency buffer until its empty ***********/
    	    while(!buf_empty(cf_buf))
    	    {
    	        uint8_t data;

    	        status = buf_read(cf_buf,&data);
    	        if(status == success)
    	        {
    	        	if(data == 0x20)
    	        		log_string("Space");
    	        	else if(data == 127)
    	        		log_string("Backspace");
    	        	else
    	        		log_Char(data);
    	        }
    	    	else
    	    	{
    				#ifdef DEBUG_MODE
    	    			log_Str(Debug,main_fun,"Unable to read data!");
    				#endif
    		    	blue_off();
    		    	green_off();
    		    	red_on();
    	    	}

    	        log_string(" - ");

    	        status = buf_read(cf_buf,&data);
    	        if(status == success)
    	        	log_Int(data);
    	    	else
    	    	{
    				#ifdef DEBUG_MODE
    	    			log_Str(Debug,main_fun,"Unable to read data!");
    				#endif
    		    	blue_off();
    		    	green_off();
    		    	red_on();
    	    	}

    	        if(buf_empty(cf_buf) != 1)
    	        	log_Char(';');

    	        log_string("\n\r");
    	    }
    	    log_Str(Status,main_fun,"The buffer is empty! ");
    	    /********** Freeing the buffers once the desired operations are performed ***********/
    	    free(buf);
    	    buf_free(c_buf);
			#ifdef DEBUG_MODE
    	    	log_Str(Debug,main_fun,"Buffer freed!");
			#endif

    	    free(f_buf);
    	    buf_free(cf_buf);
			#ifdef DEBUG_MODE
    	    	log_Str(Debug,main_fun,"Frequency Buffer freed!\n\r");
			#endif

		#endif
    }

#endif

    /************* TEST mode *************/
	#ifdef TEST
		test(c_buf);
	#endif
    return 0;
}

/************** UART 0 Interrupt Handler *****************/
void UART0_IRQHandler()
{
	START_CRITICAL
	interrupt_flag = 1;
}
