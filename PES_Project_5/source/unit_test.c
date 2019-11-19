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
/***********************************************************************************
 * UNIT TESTING [1]
 ***********************************************************************************/

/********* Standard Header Files *********/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/********* Board specific Header Files *********/
#include "MKL25Z4.h"

/********* User defined Header Files *********/
#include "logger.h"
#include "uCUnit.h"
#include "uart.h"
#include "systick.h"

/********* Unit Test function definition *********/
void test(circ_buf_p c_buf)
{
	uint8_t c, i = 0, data;
	buf_status status;

	/*************** UART Driver Test **************/
	#if USE_UART_INTERRUPTS==0 		// Polling version of code

    	Init_UART0_Poll(115200);

    	UCUNIT_Init(); /* initialize framework */
        UCUNIT_TestcaseBegin("UART Driver Test (Polling)");

    	log_Str(Test,test_fun,"Enter any character: ");
    	c = UART0_Receive();
    	log_Str(Test,test_fun,"The entered character is: ");
    	UART0_Transmit(c);

      	UCUNIT_CheckIsEqual(c,'t');
      	UCUNIT_TestcaseEnd();
      	log_string("\n\r");

	#elif USE_UART_INTERRUPTS==1 	// Interrupt version of code

    	Init_UART0_Interrupt(115200);

    	UCUNIT_Init(); /* initialize framework */
        UCUNIT_TestcaseBegin("UART Driver Test (Interrupt)");

    	log_Str(Test,test_fun,"Enter any character: ");
    	c = UART0_Receive();
    	log_Str(Test,test_fun,"The entered character is: ");
    	UART0_Transmit(c);

      	UCUNIT_CheckIsEqual(c,'t');
      	UCUNIT_TestcaseEnd();
      	log_string("\n\r");

	#endif

    /*************** Buffer Pointer Validity Test **************/
	uint8_t *buf = malloc(BUF_SIZE * sizeof(uint8_t));
	status = ptr_validity(buf);
    UCUNIT_TestcaseBegin("Buffer Pointer Validity Test");
   	if(status == success)
   		log_Str(Test,test_fun,"Valid buffer pointer.");
   	else
   		log_Str(Test,test_fun,"Invalid buffer pointer!");
    UCUNIT_CheckIsEqual(status,success);
   	UCUNIT_TestcaseEnd();
   	log_string("\n\r");

   	/*************** Circular Buffer Initialization Test **************/
    c_buf = buf_init(buf,BUF_SIZE);
    status = buf_verify(c_buf);
    UCUNIT_TestcaseBegin("Circular Buffer Initialization Test");
   	if(status == success)
   		log_Str(Test,test_fun,"Circular buffer initialized successfully!");
   	else
   		log_Str(Test,test_fun,"Circular buffer initialization failed!");
    UCUNIT_CheckIsEqual(status,fail);
   	UCUNIT_TestcaseEnd();
   	log_string("\n\r");

   	/*************** Buffer Wrap Around Test (FULL) **************/
   	while(!buf_full(c_buf))
   	{
   		buf_write(c_buf,i);
   		i++;
   		if(c_buf->head == BUF_SIZE-1)
   		{
   	   		UCUNIT_TestcaseBegin("Buffer Wrap Around Test (Before buffer is FULL)");
   	   	    UCUNIT_CheckIsEqual(c_buf->head,(BUF_SIZE-1));
   	   	   	UCUNIT_TestcaseEnd();
   	   	   	log_string("\n\r");
   		}
   	}
   	if(buf_full(c_buf) == 1)
   	{
   		log_Str(Test,test_fun,"The buffer is FULL!");
   		UCUNIT_TestcaseBegin("Buffer Wrap Around Test (FULL case)");
   	    UCUNIT_CheckIsEqual(c_buf->head,0);
   	   	UCUNIT_TestcaseEnd();
   	   	log_string("\n\r");
   	}

   	/*************** Buffer Over filling Test **************/
   	UCUNIT_TestcaseBegin("Over Fill Test");
	log_Str(Test,test_fun,"Enter any character to be input to the buffer (Full): ");
	c = UART0_Receive();
	log_Str(Test,test_fun,"The entered character is: ");
	UART0_Transmit(c);
	status = buf_write(c_buf,c);
	if(status == success)
		log_Str(Test,test_fun,"Data written to the buffer successfully!");
	else
		log_Str(Test,test_fun,"Unable to write the data to the buffer!");
    UCUNIT_CheckIsEqual(status,fail);
   	UCUNIT_TestcaseEnd();
	log_string("\n\r");

	/*************** Buffer Wrap Around Test (EMPTY) **************/
   	while(!buf_empty(c_buf))
   	{
   	   	status = buf_read(c_buf,&data);
   	   	if(status == fail)
   	   		log_Str(Test,test_fun,"Unable to read the data!");
   	   	if(c_buf->tail == BUF_SIZE-1)
   	   	{
   	   		UCUNIT_TestcaseBegin("Buffer Wrap Around Test (Before buffer is Empty)");
   	   	    UCUNIT_CheckIsEqual(c_buf->tail,(BUF_SIZE-1));
   	   	   	UCUNIT_TestcaseEnd();
   	   	   	log_string("\n\r");
   	   	}
   	}
   	if(buf_empty(c_buf) == 1)
   	{
   	    UCUNIT_TestcaseBegin("Buffer Wrap Around Test (EMPTY Case)");
   	    UCUNIT_CheckIsEqual(c_buf->tail,0);
   	   	UCUNIT_TestcaseEnd();
   	   	log_string("\n\r");
   	}

   	/*************** Buffer Over emptying Test **************/
   	UCUNIT_TestcaseBegin("Over Empty Test");
   	status = buf_read(c_buf,&data);
   	if(status == fail)
   		log_Str(Test,test_fun,"Unable to read the data!");
   	else
   	{
   		log_Str(Test,test_fun,"The data read from the buffer is ");
   		UART0_Transmit(data);
   	}
    UCUNIT_CheckIsEqual(status,fail);
   	UCUNIT_TestcaseEnd();
	log_string("\n\r");

	/*************** Buffer Data Access Test **************/
	UCUNIT_TestcaseBegin("Test of data write to the circular buffer");
	log_Str(Test,test_fun,"Enter any character to be input to the buffer: ");
	c = UART0_Receive();
	log_Str(Test,test_fun,"The entered character is: ");
	UART0_Transmit(c);
	status = buf_write(c_buf,c);
	if(status == success)
		log_Str(Test,test_fun,"Data written to the buffer successfully!");
	else
		log_Str(Test,test_fun,"Unable to write the data to the buffer!");
    UCUNIT_CheckIsEqual(status,fail);
   	UCUNIT_TestcaseEnd();
	log_string("\n\r");

	UCUNIT_TestcaseBegin("Test of data access from the buffer");
   	status = buf_read(c_buf,&data);
   	if(status == fail)
   		log_Str(Test,test_fun,"Unable to read the data!");
   	else
   	{
   		log_Str(Test,test_fun,"The data read from the buffer is ");
   		UART0_Transmit(data);
   	}
    UCUNIT_CheckIsEqual(data,c);
   	UCUNIT_TestcaseEnd();
	log_string("\n\r");

	/*************** Buffer Destroying Test **************/
   	status = buf_free(c_buf);
    UCUNIT_TestcaseBegin("Test of destroying the circular buffer");
    UCUNIT_CheckIsEqual(status,fail);
   	UCUNIT_TestcaseEnd();
	log_string("\n\r");

   	free(buf);

	/* Finish all the tests */
	UCUNIT_WriteSummary();
	UCUNIT_Shutdown();
	return;
}

/***************************************** References **************************************************
[1] https://mcuoneclipse.com/2018/08/26/tutorial-%CE%BCcunit-a-unit-test-framework-for-microcontrollers/

********************************************************************************************************/
