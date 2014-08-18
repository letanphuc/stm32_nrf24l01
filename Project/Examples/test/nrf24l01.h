#define ADR_WIDTH 5

#define RX_PLOAD_WIDTH 32
#define TX_PLOAD_WIDTH 32
#define Buffer_Size 32

//Define RF power value
#define P0dBm   0
#define Pm6dBm  1
#define Pm12dBm 2
#define Pm18dBm 3

//#define RF rate
#define R2mbps   0
#define R1mbps   1
#define R250kbps 3



void RX_Mode(void);
void TX_Mode(void);
void nRF24L01_TxPacket(unsigned char * tx_buf);
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf);
void nRF24L01_HW_Init(void);
void nRF24L01_Init_Soft(void);
unsigned char SPI_RD_Reg(unsigned char reg);
unsigned char SPI_WR_Reg(unsigned char reg, unsigned char value);
void nRF24L01_Delay_us(unsigned long n);
void nRF24L01_SPI_NSS_L(void);
void nRF24L01_SPI_NSS_H(void);
void nRF24L01_CE_L(void);
void nRF24L01_CE_H(void);

