
/*
 * SPI.h
 *
 *  Created on: Nov 22, 2012
 *      Author: Admin
 */

#ifndef SPI_H_
#define SPI_H_

#define SCLK_PORT   GPIOA
#define SCLK_PIN	GPIO_Pin_5

#define MISO_PORT	GPIOA
#define MISO_PIN	GPIO_Pin_6

#define MOSI_PORT	GPIOA
#define MOSI_PIN	GPIO_Pin_7

#define CSN_PORT	GPIOA
#define CSN_PIN		GPIO_Pin_4

#define CE_PORT		GPIOA
#define CE_PIN 		GPIO_Pin_3

#define IRQ_PORT	GPIOA
#define IRQ_PIN 	GPIO_Pin_2


void spi_csh(void);
void spi_csl(void);
void SPI_Init_For_RF(void);
void Send_byte(int data, int time_ms);
unsigned char SPI_Xfer_byte(unsigned char data);

#endif /* SPI_H_ */
