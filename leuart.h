//***********************************************************************************
// Include Files
//***********************************************************************************
//***********************************************************************************
// Definitions
//***********************************************************************************
enum TX{TXSGN,TXBIT1,TXBIT2,TXBIT3,TXPOINT,TXBIT4,TXCELCIUS,TXNEWLINE};
enum RX{RXSTART,RXC1, RXC2, RXSTOP};
#define RX_EMPTY 		0
#define READ_SIZE 		4
#define TEMP_SIZE 		4
#define TXBUFFER_SIZE 		8
#define RXBUFFER_SIZE		20
//***********************************************************************************
// Global Variables
//***********************************************************************************
int32_t TXSTATE;
int32_t RXSTATE;
uint32_t data;
int32_t tempInt;
char TXBuffer[TXBUFFER_SIZE]; 	//SRC, TXDATA is DST
char RXBuffer[RXBUFFER_SIZE];
char sgn,bit1,bit2,bit3,point,bit4,temp_unit,newline;
char temp[TEMP_SIZE];
char read[READ_SIZE];
//***********************************************************************************
// Function Prototypes
//***********************************************************************************
void leuart0_init();
void leuart_write_char(char);
void leuart_write_char_LDMA(char,int32_t);
void leuart_write(char[]);
void leuart_loopback();
void leuart_temp();
void update_temp_var();
void leuart_read();
void clear_read();
void decode_command();
void update_ldma_TXBuffer();
void LEUART0_IRQHandler();
