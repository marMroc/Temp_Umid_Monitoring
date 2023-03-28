
#include "./I2C/I2C.h"                          // Utilizado no CCS

#define HDC_1080_ADD 0x40                       // Slave address HDC1080

//***** Global Data *****
uint8_t *txBuffer;                              // 
uint8_t *rxBuffer;                              // 
uint8_t mBuffer[5];                             // 
float temperature = 0;                          // 
float humidity = 0;                             // 
uint16_t temp = 0;                              // 
uint16_t umid = 0;                              // 
uint16_t mID = 0;                               // 
uint16_t cID = 0;                               // 
uint16_t clock_timeout = 1000;                  // 
uint8_t rxCount = 0;                            // 
uint8_t txCount = 0;                            // 

bool transmit_initiated = false;                // 
bool receive_initiated = false;                 // 
bool nack_received = false;                     // 

void i2cConfig(uint8_t stop_count){
    UCB0CTLW0 = UCSWRST;                        // Reset I2C interface for configuration

    UCB0CTLW0 =                                 // USCI - B2 configuration register
                UCMST         |                 // Master mode
                UCSYNC        |                 // Synchronous mode
                UCMODE_3      |                 // I2C mode
                UCSSEL__SMCLK |                 // Select SMCLK
                UCTR          |                 // Transmitter mode
                UCSWRST       |                 // Don't release reset (yet)
                0;

    UCB0CTLW1 = UCCLTO_1      |                 // Clock low time-out select (28ms)
                UCASTP_2      |                 // Automatic STOP condition generation (enabled)
                UCGLIT_0      |                 // Deglitch time (50ns)
                0;                              //

    UCB0TBCNT = stop_count;                     //

    UCB0BRW = 20;                               // Bit clock divider 8M/20 = 400kHz

    UCB0CTLW0 &= ~UCSWRST;                      // Clear reset bit to start operation

    UCB0IE = 0;                                 //

    UCB0IE = UCNACKIE         |                 // Enable Nack interrupt
             UCTXIE           |                 // Enable TX interrupt
             UCRXIE           |                 // Enable RX interrupt
             UCCLTOIE         |                 // Enable clock low time-out interrupt
             UCSTPIE          |                 // Enable stop bit interrupt
             0;                                 //
}

void initialize_i2c(void){                      // Configure interface ports
  P1SEL0 |=  (BIT2 | BIT3);                     // Connect ports: P1.2 (SDA)
  P1SEL1 &= ~(BIT2 | BIT3);                     // Connect ports: P1.3 (SCL)
  P1DIR  &= ~(BIT2 | BIT3);                     // Configure as if they are inputs

  i2cConfig(0x01);                              // Configure i2c bus with stop count of 1 byte

  __delay_cycles(50);                           //
  //__bis_SR_register(GIE);                     // Enable interrupts
}

void Transmit_I2C(uint8_t *field, uint8_t count, uint8_t ADD_Slave){
  transmit_initiated = true;                    //
  UCB0I2CSA  = ADD_Slave;                       // Set slave address
  txCount    = count;                           //
  txBuffer   = field;                           //
  UCB0CTLW0 |= UCTR | UCTXSTT;                  // Start request as transmitter
}

void write(uint8_t reg, uint8_t data, uint8_t ADD_Slave){
  i2cConfig(0x02);                              //

  mBuffer[0] = reg;                             // Fill buffer array
  mBuffer[1] = data;                            //

  Transmit_I2C(mBuffer,2, ADD_Slave);           // Transmit buffer
  while(transmit_initiated);                    //
}

void write16(uint8_t reg, uint8_t data, uint8_t data2, uint8_t ADD_Slave){
  i2cConfig(0x03);                              //

  mBuffer[0] = reg;                             // Fill buffer array
  mBuffer[1] = data;                            //
  mBuffer[2] = data2;                           //

  Transmit_I2C(mBuffer,3, ADD_Slave);           // Transmit buffer
  while(transmit_initiated);                    //
}

void receive(uint8_t *field, uint8_t count, uint8_t ADD_Slave){
  receive_initiated = true;                     //
  UCB0I2CSA  = ADD_Slave;                       // Set slave address
  rxCount    = count;                           //
  rxBuffer   = field;                           //
  UCB0CTLW0 &= ~UCTR;                           // Put I2C bus in receive mode
  UCB0CTLW0 |= UCTXSTT;                         // Start request as a receiver
}

void read(uint8_t reg, uint8_t ADD_Slave){
  i2cConfig(0x01);                              //

  Transmit_I2C(&reg,1,ADD_Slave);               // Transmit register address
  while(transmit_initiated);                    //

  __delay_cycles(320000);                       //delay de 20 ms

  receive(mBuffer,1,ADD_Slave);                 // Receive data
  while(receive_initiated);                     //
}

void read16(uint8_t reg, uint8_t ADD_Slave){
  i2cConfig(0x01);                              //

  Transmit_I2C(&reg,1,ADD_Slave);               // Transmit register address
  while(transmit_initiated);                    //

  __delay_cycles(320000);                       //

  i2cConfig(0x02);                              // Receive data
  receive(mBuffer,2,ADD_Slave);                 //
  while(receive_initiated);                     //
}

void read32(uint8_t reg, uint8_t ADD_Slave){
  i2cConfig(0x01);                              //
  
  Transmit_I2C(&reg,1,ADD_Slave);               // Transmit register address
  while(transmit_initiated);                    //
  
  __delay_cycles(320000);                       //
  
  i2cConfig(0x04);                              // Receive data
  receive(mBuffer,4,ADD_Slave);                 //
  while(receive_initiated);                     //
}

void Configura_HDC1080(uint8_t ADD_Slave){
  initialize_i2c();                             // Initialize - I2C
  
  read16(0xFE,ADD_Slave);                       //
  mID = ((mBuffer[0]<<8)|mBuffer[1]);           // Manufacture ID = 0x5449
  
  read16(0xFF,ADD_Slave);                       //
  cID = ((mBuffer[0]<<8)|mBuffer[1]);           // Chip ID = 0x1050
  
  if( (mID == 0x5449) && (cID == 0x1050) ){     //
    __no_operation();                           // Debug
    __no_operation();                           // Debug
  }
  else{
    __no_operation();                           // Debug
    __no_operation();                           // Debug
  }
  
  //write16(0x02, 0x90, 0x00,ADD_Slav);         // ler temp e umd em sequencia
  write16(0x02, 0x80, 0x00,ADD_Slave);          // ler temp separado de umd
  
  __delay_cycles(1000000);                      //
}

void Read_HDC1080(uint8_t ADD_Slave){
  read16(0x00, ADD_Slave);                      //
  temp=( (mBuffer[0]<<8) | mBuffer[1] );        //
  
  read16(0x01, ADD_Slave);                      //
  umid = ( (mBuffer[0]<<8) | mBuffer[1] );      //
  
  temperature = ((temp/65536.0)*165.0)-40.0;    //
  humidity = (umid/65536.0)*100.0;              //
}



#pragma vector=USCI_B0_VECTOR
__interrupt void USCIB2_ISR( void ){
    switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG)){
        case USCI_NONE:         break;                      // No interrupts break;

        case USCI_I2C_UCALIFG:  break;                      // Arbitration lost

        case USCI_I2C_UCNACKIFG:nack_received = true;       // NAK received (master only)
                                UCB0CTLW0 |= UCTXSTP;       // when an acknowledge is missing the slave gets a stop condition
                                UCB0IFG   &= ~UCTXIFG;      //
                                _NOP();                     //
                                break;                      //

        case USCI_I2C_UCSTPIFG: if(transmit_initiated){transmit_initiated = false;}
                                if(receive_initiated){receive_initiated = false;}
                                break;                      //

        case USCI_I2C_UCRXIFG0: if(receive_initiated){      // RXIFG0
                                    *rxBuffer++ = UCB0RXBUF;// Get RX data
                                    _NOP();                 //
                                }
                                break;                      //

        case USCI_I2C_UCTXIFG0: if(transmit_initiated){     // TXIFG0
                                    UCB0TXBUF = *txBuffer++;//
                                    _NOP();                 //
                                }
                                break;                      //

        case USCI_I2C_UCCLTOIFG:clock_timeout = true;       // Clock low timeout - clock held low too long
                                _NOP();                     //
                                break;                      //

        default:                break;                      //
  }
}
