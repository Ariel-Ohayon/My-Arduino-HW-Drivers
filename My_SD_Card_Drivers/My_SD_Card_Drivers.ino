#include<SPI.h>

#define CS_SD PB1 // pin9 = PB1
#define SD_Enable PORTB &= ~(1 << CS_SD)
#define SD_Disable PORTB |= (1 << CS_SD)

#define CMD0  0
#define CMD0_arg  0x00000000
#define CMD0_CRC  0x95

#define CMD8  8
#define CMD8_arg  0x000001AA
#define CMD8_CRC  0x87

#define CMD55 55
#define CMD55_arg 0x00000000
#define CMD55_CRC 0xFF

#define ACMD41 41
#define ACMD41_arg 0x40000000
#define ACMD41_CRC 0xFF

#define CMD16 16
#define CMD16_arg 0x00000200
#define CMD16_CRC 0x00

#define CMD17 17
#define CMD17_CRC 0x00

#define R1 1
#define R7 5

uint8_t* SD_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t sizeof_response);
bool SD_init();
uint8_t* SD_Read_Block(int block_num);

void setup()
{
  char print_buffer[10];
  uint8_t* Block_Buffer;
  Serial.begin(9600);
  // Initialize SD Card Pin Configuration //
  DDRB |= (1 << CS_SD);
  SD_Disable;
  SPI.begin();
  if(SD_init() == 0)
  {
    Serial.println("SD initialize failed");
    while(1);
  }
  Serial.println("SD Initialize Success");
  Block_Buffer = SD_Read_Block(0x2000);  // Read Boot Sector
  if(Block_Buffer == NULL)
  {
    Serial.println("Error");
    free(Block_Buffer);
    while(1);
  }
  
  Serial.println("Data of Sector number 0: (Boot Sector)");
  Serial.print("Address ");
  for(int i=0;i<16;i++)
  {
    sprintf(print_buffer,"0x%02X ",i);
    Serial.print(print_buffer);
  }
  Serial.println();
  for(int i=0;i<32;i++)
  {
    sprintf(print_buffer," 0x%02X0: ",i);
    Serial.print(print_buffer);
    for(int j = 0; j < 16; j++)
    {
      sprintf(print_buffer,"0x%02X ",Block_Buffer[i*16+j]);
      Serial.print(print_buffer);
    }
    Serial.println();
  }
  free(Block_Buffer);
}

void loop()
{
  while(1);
}

uint8_t* SD_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t sizeof_response)
{
  uint8_t arg_bytes[4]={0};
  uint8_t *response = (uint8_t*)malloc(sizeof_response*sizeof(uint8_t));
  //  //
  cmd |= 0x40;
  for(int i = 0; i <= 3; i++)
  {
    arg_bytes[i] = arg >> (i*8);
    arg_bytes[i] &= 0xFF;
  }
  crc |= 0x01;

  // Send SD Command //
  SPI.transfer(cmd);  // Send command ID
  for(int i = 3; i >= 0; i--) // send 4 Bytes of argument
  {
    delay(1);
    SPI.transfer(arg_bytes[i]);
  }
  SPI.transfer(crc);  // send CRC and stop bit.

  // Send Dummy byte and read response:
  SPI.transfer(0xFF); // Send Dummy byte.

  for(int i = 0; i < sizeof_response; i++)
  {
    response[i] = SPI.transfer(0xFF);
  }
  return response;
}

bool SD_init()
{
  uint8_t *R1_response, *R7_response;
  // Send 74 (80) Pulses
  SD_Disable;
  for(int i = 0; i < 10; i++)
  {
    SPI.transfer(0xFF);
  }

  // Send command "CMD0" //
  SD_Enable;
  R1_response = SD_send_cmd(CMD0,CMD0_arg,CMD0_CRC,R1);
  SD_Disable;
  if(R1_response[0] != 0x1)  // Check response signal
  {
    free(R1_response);
    return 0;
  }
  free(R1_response);

  // Send command "CMD8"
  SD_Enable;
  R7_response = SD_send_cmd(CMD8,CMD8_arg,CMD8_CRC,R7);
  SD_Disable;
  if((R7_response[0] != 0x1) || (R7_response[1] != 0x00) || (R7_response[2] != 0x00) || (R7_response[3] != 0x01) || (R7_response[4] != 0xAA))
  {
    free(R7_response);
    return 0;
  }
  free(R7_response);
  
  // Send command "CMD51" and "ACMD41" twice
  for(int i=0;i<2;i++){
    SD_Enable;
    R1_response = SD_send_cmd(CMD55,CMD55_arg,CMD55_CRC,R1);
    SD_Disable;
    if(R1_response[0] != 0x1)
    {
      free(R1_response);
      return 0;
    }
    free(R1_response);
    
    SD_Enable;
    R1_response = SD_send_cmd(ACMD41,ACMD41_arg,ACMD41_CRC,R1);
    SD_Disable;
    if((R1_response[0] != 0x1) && (R1_response[0] != 0x0))
    {
      free(R1_response);
      return 0;
    }
    free(R1_response);
  }

  // Send CMD16 to set block size
  SD_Enable;
  R1_response = SD_send_cmd(CMD16,CMD16_arg,CMD16_CRC,R1);
  SD_Disable;
  if(R1_response[0] != 0x00)
  {
    free(R1_response);
    return 0;
  }
  return 1;
}

uint8_t* SD_Read_Block(int block_num)
{
  uint8_t* R1_response;
  uint8_t *block_buff = (uint8_t*)malloc(512*sizeof(uint8_t));
  SD_Enable;
  R1_response = SD_send_cmd(CMD17,block_num,CMD17_CRC,R1);
  if(R1_response[0] != 0x00)
  {
    free(R1_response);
    return NULL;
  }
  free(R1_response);
  while(SPI.transfer(0xFF) != 0xFE);
  for(int i = 0; i < 512; i++)
  {
    block_buff[i] = SPI.transfer(0xFF);
  }
  // Recv CRC
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  
  // Send dummy byte
  SPI.transfer(0xFF);
  SD_Disable;
  return block_buff;
}
