#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "UART.h"
#include "stdio.h"

/****************************************************************************
 * FUNCTIONS
 ******************************************************************************/

/****************************************************************************
 * MODULE PUPLIC FUNCTIONS:
 * void uart_init();
 * void uart_putc(char c);
 * void uart_puts(const char *s);
 * void uart_put_num (unsigned long val, char dec, unsigned char neg);
 * char uart_data_ready();
 * char uart_getc();
 * void uart_gets(char *s);
 *
 ******************************************************************************/

//*****************************************************************************
// Initialization for UART module
//*****************************************************************************
void uart_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);


	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;

	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;

	USART_Init(USART1,&USART_InitStructure);

	USART_Cmd(USART1,ENABLE);	
}

//*****************************************************************************
// Send a  character through UART
//*****************************************************************************
void uart_putc(char c) {
	USART_SendData(USART1, (uint8_t) c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

//*****************************************************************************
// Send a  string through UART
// Note that the string is ended with '/0' (or 0x00)
//*****************************************************************************
void uart_puts(const char* s) {
	while (*s != '\0') {
		uart_putc(*s);
		s++;
	}
}

//*****************************************************************************
// Send a constant or a number stored in 'val' through UART
// The number must be multiplied and convert in type "unsigned long"
//                                                or "unsigned int"
//   dec: amount of digits after '.'
//   neg: sign of the number, neg = 0 if the number is positive
//
// Example 1: uart_put_num(val, 3, 0); // val = 12345
//        will put "12.345"
// Example 2: uart_put_num(val, 6, 1); // val = 12345
//        will put "-0.012345"
//
// Note: This function may contain some bugs. If you can detect and fix them
//       , please contact me :d
//*****************************************************************************
void uart_put_num(unsigned long val, char dec, unsigned char neg) {
	char i, j, digit, k;
	long total;
	long temp;
	for (i = 0, total = val; total > 0; i++) // count number
		total /= 10;
	total = i;

	if (neg != 0)
		uart_putc('-');

	if ((total - dec) <= 0)	// if total < dec put 0.xxx
		uart_putc('0');
	else {

		for (i = 0; i < (total - dec); i++) {
			temp = 1;
			for (j = 1; j < (total - i); j++)
				temp *= 10;
			digit = (val / temp) % 10;
			uart_putc(digit + 0x30);
		}
	}
	if (dec > 0) {
		uart_putc('.');
		if ((dec - total) > 0)	// ex: val = 55; dec = 3 put 0.055
				{
			for (i = 0; i < (dec - total); i++)
				uart_putc('0');
			k = 0;
		} else
			k = total - dec;
		for (i = k; i < total; i++) {
			temp = 1;
			for (j = 1; j < (total - i); j++)
				temp *= 10;
			digit = (val / temp) % 10;
			uart_putc(digit + 0x30);
		}
	}
}

//*****************************************************************************
// Wait to receive a character through UART
//*****************************************************************************
char uart_getc() {
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return (char) USART_ReceiveData(USART1);
}

//*****************************************************************************
// Wait to receive a string through UART
// Note that the string is ended with '/0' (or 0x00)
//*****************************************************************************
void uart_gets(char *s) {
	*s = uart_getc();
	while (*s != '\0') {
		s++;
		*s = uart_getc();
	}
}

//*****************************************************************************
// Check if USCI_A0 RX Buffer contain a complete character
//*****************************************************************************
char uart_data_ready() {
	return USART_GetFlagStatus(USART1, USART_FLAG_TC) == SET;
}

/****************************************************************************
 * USCI_A0 RECEIVE INTERRUPT SERVICE ROUTINE
 ******************************************************************************/
//#pragma vector = USCIAB0RX_VECTOR
//__interrupt void USCI0RX_ISR(void) {
//	//uart_putc(UCA0RXBUF); // echo the received character
//	//
//	//...
//	//
//}

void printreg(char *name, int n) {
	int size = 8;
	int i;

	int mask = 1 << (size - 1);
	
	uart_puts((char *) "- ");
	uart_puts(name);
	uart_puts((char *) ": ");

	for (i = 0; i < size; i++) {
		if ((n & (mask >> i)) != 0) {
			uart_putc('1');
		} else {
			uart_putc('0');
		}
	}
	uart_puts((char *) " (");
	uart_printi(n);
	uart_puts((char *) ")\n\r");
}
void uart_printi(int n) {
	int buffer[16];
	int i;
	int j;

	if (n == 0) {
		uart_putc('0');
		return;
	}

	for (i = 15; i > 0 && n > 0; i--) {
		buffer[i] = (n % 10) + '0';
		n /= 10;
	}

	for (j = i + 1; j <= 15; j++) {
		uart_putc(buffer[j]);
	}
}

/* for debug */
int fputc(int _c, register FILE *_fp) {
	uart_putc((char) _c);
	return _c;
}
int fputs(const char *_ptr, register FILE *_fp) {
	uart_puts(_ptr);
	return 1;
}

/******************************************************************************
 * END OF UART.C
 *******************************************************************************/
