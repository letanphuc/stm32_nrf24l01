#include "stdio.h"
#include "stdint.h"
#include "nrf24l01.h"
#include "UART.h"
#include "STM32vldiscovery.h"



void Initialize(void);
void ToggleLED(void); //toggle the current state of the on-board LED
void CheckErrorsUSART(void); //fix any framing or overrun errors in the USART


void configRF_txAddress (uint8_t * add){
	printf("configRF_txAddress\n\r");
	nrf24l01_set_tx_addr(add,5);
	delay_ms(1);
}

void RF_sendData(uint8_t * data, uint8_t len){
	uint8_t i = 0;
	for (i = 0; i < len; i++){
		nrf24l01_write_tx_payload(data + i, 1, 0);
		while (!(nrf24l01_irq_pin_active() && nrf24l01_irq_tx_ds_active()));
	}
}


uint8_t RF_sendByte(uint8_t byte){
	uint8_t reply = 0;
	uint8_t count = 0;
	
	uint8_t sampleByte[16] = {0};

	nrf24l01_write_tx_payload(&byte, 1, true);
	while (!(nrf24l01_irq_pin_active() && nrf24l01_irq_tx_ds_active())){
		/* wait */
		//nrf24l01_read_register(nrf24l01_
		printf("Fifo status %d %x\n\r", byte, nrf24l01_get_fifo_status());
	}
	nrf24l01_irq_clear_all();
	nrf24l01_set_as_rx(true);

	for (count = 0; count < 100; count++) {

		if ((nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active())) {
			nrf24l01_read_rx_payload(&reply, 1); //get the payload into data
			break;
		}

	}

	nrf24l01_irq_clear_all(); //clear interrupts again


	delay_ms(10); //wait for receiver to come from standby to RX
	nrf24l01_set_as_tx(); //resume normal operation as a TX

	return reply;
}

uint8_t* RF_sendArray(uint8_t* data, uint8_t len){
	uint8_t i = 0;
	static uint8_t reply[32] = {0};

	for (i = 0; i < len; i++)
		reply[i] = RF_sendByte(data[i]);

	return reply;
}

int main(void) {
	uint8_t data = 32;
	uint8_t new_data = 21;
	unsigned char stat;
	unsigned char conf;
	
	uint8_t destIP[5] = { 192, 168, 1, 1, 111 };

	Initialize(); //initialize IO, UART, SPI, set up nRF24L01 as TX

	printf("Local host now run\n\r");

	configRF_txAddress(destIP);
	while (1) {

		//data = uart_getc();

		data ++;
		new_data = RF_sendByte(data);


		printf("\n\rData send: %d -> %d\n\r", data, new_data);
		if (new_data != data)
			printf("Erororrororororr\n\r");

		ToggleLED(); //toggle the on-board LED as visual indication that the loop has completed
//		nrf24l01_read_register(nrf24l01_CONFIG,&data, 1);
//		printf("Config: %d\n\r", data);
//		
//		nrf24l01_read_register(nrf24l01_STATUS,&data, 1);
//		printf("Satus: %d\n\r", data);

		/* test irq pin */
//		printf("IRQ: %d \n\r", nrf24l01_irq_pin_active());

//		/* test csn pin */
//		nrf24l01_set_csn();
//		nrf24l01_set_ce();
//		delay_ms(50);
//		nrf24l01_clear_csn();
//		nrf24l01_clear_ce();
//		
//		/* test ce pin */
//		if (data % 2 == 0){
//			GPIO_WriteBit(CE_PORT, CE_PIN, Bit_RESET);
//			GPIO_WriteBit(CSN_PORT, CSN_PIN, Bit_RESET);
//		}
//		else {
//			GPIO_WriteBit(CE_PORT, CE_PIN, Bit_SET);
//			GPIO_WriteBit(CSN_PORT, CSN_PIN, Bit_SET);
//		}
		
//		nrf24l01_write_register(nrf24l01_RX_PW_P0,&data, 1);
//		nrf24l01_read_register(nrf24l01_RX_PW_P0, &new_data, 1);
//		
//		printf("data = %d, new_data = %d\n\r", data, new_data);
//		
//		data ++ ;
		
//		nrf24l01_clear_csn();
//		// send command
//		stat = SPI_Xfer_byte(nrf24l01_R_REGISTER | nrf24l01_CONFIG);
//		// read response
//		conf = SPI_Xfer_byte(nrf24l01_NOP);
//		nrf24l01_set_csn();
//		uart_puts((char *) "NRF24l01 registers:\n\r");
//		printreg((char *) "status", stat);
//		printreg((char *) "config", conf);

		delay_ms(200);
	}
	
}

//initialize routine
void Initialize(void) {
	STM32vldiscovery_LEDInit(LED3);
	STM32vldiscovery_LEDOn(LED3);
	
	uart_init();

	uart_puts("Hello i'm running\n\r");

	SPI_Init_For_RF();
	nrf24l01_initialize_debug(false, 1, false); //initialize the 24L01 to the debug configuration as TX, 1 data byte, and auto-ack disabled
}
void ToggleLED(void) {
	/* not done */
	STM32vldiscovery_LEDToggle(LED3);
}
