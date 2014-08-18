#include <stdio.h>
#include "debug.h"

struct __FILE
{
  int handle;

  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};

/* FILE is typedef’d in stdio.h. */

FILE __stdout;

int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}
int ferror(FILE *f)
{
  /* Your implementation of ferror(). */
  return 0;
}

