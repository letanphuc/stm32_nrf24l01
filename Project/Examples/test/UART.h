#ifndef UART_H_
#define UART_H_


void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_put_num (unsigned long val, char dec, unsigned char neg);
char uart_data_ready(void);
char uart_getc(void);
void uart_gets(char *s);
void uart_printi(int n);
void printreg(char *name, int n);
#endif /* UART_H_ */
