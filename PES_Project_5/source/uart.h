#ifndef UART_H

#define UART_H

#include "circular_buffer.h"

#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 				(24e6)
#define SYS_CLOCK				(48e6

extern uint8_t interrupt_flag, c;

void Init_UART0_Poll(uint32_t baud_rate);
void Init_UART0_Interrupt(uint32_t baud_rate);

uint8_t UART0_Transmit_Ready(void);
void UART0_Transmit_Char(uint8_t data);
void UART0_Transmit(uint8_t data);
void UART0_Transmit_String(char *str);
void UART0_Echo_Poll(void);
void UART0_Echo_Int(void);
void UART0_App_Poll(circ_buf_p c_buf);
void UART0_App_Int(circ_buf_p c_buf);
void UART0_Transmit_Number(uint32_t number);

uint8_t UART0_Receive_Ready();
uint8_t UART0_Receive_Char(void);
uint8_t UART0_Receive(void);

void write_fbuf(circ_buf_p cf_buf);
void freq_Init(void);

#endif
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************
