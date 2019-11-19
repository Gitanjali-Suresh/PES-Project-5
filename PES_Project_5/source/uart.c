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
 * UART OPERATIONS [1]
 *****************************************************************************/\

/********* Standard Header Files *********/
#include <stdio.h>
#include <stdint.h>

/********* Board specific Header Files *********/
#include "fsl_debug_console.h"
#include "MKL25Z4.H"

/********* User defined Header Files *********/
#include "uart.h"
#include "logger.h"
#include "led.h"

/***************** Global Variable Declarations ******************/
uint8_t freq[127];

/***************** Function to initialize UART0 in polling mode **************/
void Init_UART0_Poll(uint32_t baud_rate)
{
	uint16_t sbr;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK/2)/(baud_rate * UART_OVERSAMPLE_RATE)));

	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(0) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	UART0->S1 &= ~UART0_S1_RDRF_MASK;
}

/***************** Function to initialize UART0 in interrupt mode **************/
void Init_UART0_Interrupt(uint32_t baud_rate)
{
	uint16_t sbr;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK/2)/(baud_rate * UART_OVERSAMPLE_RATE)));

	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(0) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	// Enable interrupts. Listing 8.11 on p. 234

	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	UART0->S1 &= ~UART0_S1_RDRF_MASK;
}

/***************** Function to check if transmitter is ready to transmit **************/
uint8_t UART0_Transmit_Ready(void)
{
		while (!(UART0->S1 & UART0_S1_TDRE_MASK))
		{
			blue_off();
			red_off();
			green_on();
		}
		blue_off();
		red_off();
		green_on();

		return 1;
}

/***************** Function to transmit character **************/
void UART0_Transmit_Char(uint8_t data)
{
		UART0->D = data;
}

/***************** Function to transmit multiple characters **************/
void UART0_Transmit(uint8_t data_poll)
{
	uint8_t flag;
	flag = UART0_Transmit_Ready();

	if(flag == 1)
	{
		UART0_Transmit_Char(data_poll);
	}
	else
	{
		PRINTF("Transmitter not ready!\n\r");
    	blue_off();
    	green_off();
    	red_on();
	}
}

/***************** Function to check if receiver is ready to receive **************/
uint8_t UART0_Receive_Ready()
{
	while (!(UART0->S1 & UART0_S1_RDRF_MASK))
	{
		green_off();
		red_off();
		blue_on();
	}
	green_off();
	red_off();
	blue_on();
	return 1;
}

/***************** Function to receive character **************/
uint8_t UART0_Receive_Char(void)
{
	uint8_t data;
	data = UART0->D;
	return data;
}

/***************** Function to receive multiple characters **************/
uint8_t UART0_Receive(void)
{
	uint8_t flag, data;
	flag = UART0_Receive_Ready();

	if(flag == 1)
	{
		data = UART0_Receive_Char();
	}
	else
	{
		PRINTF("Receiver not ready!\n\r");
    	blue_off();
    	green_off();
    	red_on();
		data = 0;
	}
	return data;
}

/***************** Function to echo in polling mode **************/
void UART0_Echo_Poll(void)
{
	uint8_t c;
	c = UART0_Receive();
	log_Str(Status,UART0_Echo_Poll_fun,"");
	if(c == 0x20)
		log_string("Space");
	else if(c == 8)
		log_string("Backspace");
	else if(c == 13)
		log_string("Enter");
	else
		UART0_Transmit(c);
}

/***************** Function to transmit string **************/
void UART0_Transmit_String(char *str)
{
	uint8_t i = 0;
	while(str[i] != '\0')
	{
		UART0_Transmit(str[i]);
		i++;
	}
}

/***************** Function to transmit number **************/
void UART0_Transmit_Number(uint32_t number)
{
    uint8_t temp, i=0;
    uint8_t a[2];
    for(i=0;i<2;i++)
    {
        temp = number % 10;
        a[i] = temp;
        number = number / 10;
    }
    for(i=0;i<2;i++)
    	UART0_Transmit(a[1-i]+'0');
}

/***************** Function to echo in interrupt mode **************/
void UART0_Echo_Int(void)
{
	uint8_t c;
	c = UART0_Receive();
	if(interrupt_flag == 1) //This flag is set in the interrupt handler
	{
		log_Str(Status,UART0_Echo_Int_fun,"");
		if(c == 0x20)
			log_string("Space");
		else if(c == 8)
			log_string("Backspace");
		else if(c == 13)
			log_string("Enter");
		else
			UART0_Transmit(c);
		interrupt_flag = 0;
	}
}

/***************** Function to generate report in polling mode **************/
void UART0_App_Poll(circ_buf_p c_buf)
{
	volatile uint8_t i;
	buf_status status;

	i = c;
	freq[i]++;
    status = buf_write(c_buf,c);
    if(status == fail)
    {
    	log_Str(Debug,UART0_Echo_fun,"Unable to write!");
    	blue_off();
    	green_off();
    	red_on();
    }
}

/***************** Function to generate report in interrupt mode **************/
void UART0_App_Int(circ_buf_p c_buf)
{
	uint8_t i;
	buf_status status;
	if(interrupt_flag == 1)
	{
		//UART0_Transmit(c);

		i = c;
		freq[i]++;
	    status = buf_write(c_buf,c);
	    if(status == fail)
	    {
	    	log_Str(Debug,UART0_Echo_fun,"Unable to write!");
	    	blue_off();
	    	green_off();
	    	red_on();
	    }
	    interrupt_flag = 0;
	}
}

/***************** Function to write to the frequency circular buffer **************/
void write_fbuf(circ_buf_p cf_buf)
{
	uint8_t i;
    for(i = 0;i < 128;i++)
    {
        if(freq[i] != 0)
        {
            buf_write(cf_buf,i);
            buf_write(cf_buf,freq[i]);
        }
    }
}

/***************** Function to zero initialize the frequency array **************/
void freq_Init(void)
{
	uint8_t i;
	for(i = 0;i < 128;i++)
		freq[i] = 0;
}

/**************************************** References *******************************************
 * [1] https://github.com/alexander-g-dean/ESF/blob/master/Code/Chapter_8/Serial-Demo/src/UART.c

 ************************************************************************************************/
