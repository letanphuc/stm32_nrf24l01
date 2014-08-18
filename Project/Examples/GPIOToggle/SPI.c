#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "SPI.h"

/*
 * SPI.c
 *
 *  Created on: Nov 22, 2012
 *      Author: Admin
 */
void SPI_Init_For_RF(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Init CSN pin */
	GPIO_InitStructure.GPIO_Pin = CSN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CSN_PORT, &GPIO_InitStructure);
	
	/* Init CE pin */
	GPIO_InitStructure.GPIO_Pin = CE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CE_PORT, &GPIO_InitStructure);
	
	/* Init IRQ pin */
	GPIO_InitStructure.GPIO_Pin = IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IRQ_PORT, &GPIO_InitStructure);
	
	/* Enable SPI1 clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = SCLK_PIN | MISO_PIN | MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SCLK_PORT, &GPIO_InitStructure);

	/* SPI1 configuration */ 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE); 

	spi_csh();
}


unsigned char SPI_Xfer_byte(unsigned char data)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, (uint16_t) data);
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}

void spi_csh(void)
{
    GPIO_SetBits(CSN_PORT, CSN_PIN) ;
}

void spi_csl(void)
{
    GPIO_ResetBits(CSN_PORT, CSN_PIN) ;
}

