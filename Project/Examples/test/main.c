/**
  ******************************************************************************
  * @file    SPI_RF_Slave/main.c 
  * @author  PG
  * @version V1.0.0
  * @date    9-05-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "nrf24l01.h"
#include "stm32f10x_usart.h"
#include "stdio.h"

unsigned char i;
unsigned char ADD[3]={0};
unsigned char tx_buff[32];
unsigned char rx_buff[32];
static __IO uint32_t TimingDelay;

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
SPI_InitTypeDef   SPI_InitStructure;


/* Private define ------------------------------------------------------------*/

//Define the commands for operate the nRF24L01P
#define READ_nRF_REG    0x00  // Command for read register
#define WRITE_nRF_REG   0x20 	// Command for write register
#define RD_RX_PLOAD     0x61  // Command for read Rx payload
#define WR_TX_PLOAD     0xA0  // Command for write Tx payload
#define FLUSH_TX        0xE1 	// Command for flush Tx FIFO
#define FLUSH_RX        0xE2  // Command for flush Rx FIFO
#define REUSE_TX_PL     0xE3  // Command for reuse Tx payload
#define NOP             0xFF  // Reserve

//Define the register address for nRF24L01P
#define CONFIG          0x00  //  Configurate the status of transceiver, mode of CRC and the replay of transceiver status
#define EN_AA           0x01  //  Enable the atuo-ack in all channels
#define EN_RXADDR       0x02  //  Enable Rx Address
#define SETUP_AW        0x03  // Configurate the address width
#define SETUP_RETR      0x04  //  setup the retransmit
#define RF_CH           0x05  // Configurate the RF frequency
#define RF_SETUP        0x06  // Setup the rate of data, and transmit power
#define NRFRegSTATUS    0x07  //
#define OBSERVE_TX      0x08  //
#define CD              0x09  // Carrier detect
#define RX_ADDR_P0      0x0A  // Receive address of channel 0
#define RX_ADDR_P1      0x0B  // Receive address of channel 1
#define RX_ADDR_P2      0x0C  // Receive address of channel 2
#define RX_ADDR_P3      0x0D  // Receive address of channel 3
#define RX_ADDR_P4      0x0E  // Receive address of channel 4
#define RX_ADDR_P5      0x0F  // Receive address of channel 5
#define TX_ADDR         0x10  //       Transmit address
#define RX_PW_P0        0x11  //  Size of receive data in channel 0
#define RX_PW_P1        0x12  //  Size of receive data in channel 1
#define RX_PW_P2        0x13  //  Size of receive data in channel 2
#define RX_PW_P3        0x14  //  Size of receive data in channel 3
#define RX_PW_P4        0x15  // Size of receive data in channel 4
#define RX_PW_P5        0x16  //  Size of receive data in channel 5
#define FIFO_STATUS     0x17  // FIFO Status

/* Private macro -------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay_ms(uint32_t nTime);
void TimingDelay_Decrement(void);
void SPI_Configuration(void);
void UART_Config(void);
//void Delay(__IO uint32_t nCount);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	
void chop_tat(void);

int main(void)
{
	unsigned char status;
	char i;
  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
	
	nRF24L01_HW_Init();
	UART_Config();

  while (1)
  {
		RX_Mode();
		if(nRF24L01_RxPacket(rx_buff))
		{
// 			 printf(" buff[0]= %d",rx_buff[0]);
// 		   printf(" buff[1]= %d",rx_buff[1]);
// 			 printf(" buff[2]= %d",rx_buff[2]);
// 			 printf(" buff[3]= %d",rx_buff[3]);
// 			 printf(" buff[4]= %d",rx_buff[4]);
// 			 printf(" buff[5]= %d",rx_buff[5]);
// 			 printf(" buff[6]= %d",rx_buff[6]);
// 			 printf(" buff[7]= %d",rx_buff[7]);
			 for(i=0;i<32;i++)
			 {
				 //USART_SendData(USART2, rx_buff[i]);
				 printf("%d",rx_buff[i]);
			 }
			 printf("\n\r");
			 chop_tat();
		}
		status=SPI_RD_Reg(0x17);
		//if((status & 0x02)==0x02)
		//if((status )==0x02)
		//chop_tat();
// 		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty
//   		printf(" fifo %d",status);
//   		status=SPI_RD_Reg(NRFRegSTATUS);
//   		printf(" status %d",status);
// 		status=SPI_RD_Reg(RF_CH);
// 		printf(" rf chanel %d",status);
// 		status=SPI_RD_Reg(RF_SETUP);
// 		printf(" rf set up %d",status);
// 		status=SPI_RD_Reg(CONFIG);
// 		printf(" config %d",status);
// /// 		status=SPI_RD_Reg(RX_PW_P0);
// // 		printf(" number of byte P0 %d",status);
// // 		status=SPI_RD_Reg(RX_PW_P1);
// // 		printf(" number of byte P1 %d",status);
// 		printf(" buff[1]= %d",rx_buff[1]);
// 		printf(" buff[0]= %d",rx_buff[0]);
 		Delay_ms(500);
		
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
// void Delay(__IO uint32_t nCount)
// {
//   while(nCount--)
//   {
//   }
// }
void Delay_ms(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void chop_tat(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	Delay_ms(500);
	GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	Delay_ms(500);
}


void UART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);   // USART2_TX
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);  // USART2_RX
	
	/* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	/* UART Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	
	/* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
  USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART2, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

	return ch;
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
