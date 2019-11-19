#ifndef _LOGGER_H

#define _LOGGER_H

#endif

typedef enum {
	main_fun,
	UART0_IRQ,
	UART0_Echo_fun,
	UART0_Echo_Poll_fun,
	UART0_Echo_Int_fun,
	test_fun,
	test_init,
	test_shut,
	str_write,
	int_write,
	test_recover,
	test_safe
}eFunction_name;

typedef enum {
	Status,
	Debug,
	Test
}eLog_level;

void log_Str(eLog_level mode, eFunction_name func, char *str);
void log_Int(uint32_t number);
void log_Char(char ch);
void log_string(char *str);
