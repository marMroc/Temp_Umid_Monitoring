

// Global function declarations
//void initialize(void);                                  // Scopo da subrotina
//void i2cConfig(uint8_t stop_count);                     // Scopo da subrotina
//void write(uint8_t reg, uint8_t data);                  // Scopo da subrotina
//void write16(uint8_t reg, uint8_t data, uint8_t data2); // Scopo da subrotina
//void read(uint8_t reg);                                 // Scopo da subrotina
//void read16(uint8_t reg);                               // Scopo da subrotina
//void read32(uint8_t reg);                               // Scopo da subrotina
//void transmit(uint8_t *field, uint8_t count);           // Scopo da subrotina
//void receive(uint8_t *field, uint8_t count);            // Scopo da subrotina

extern uint8_t *txBuffer;
extern uint8_t *rxBuffer;
extern uint8_t mBuffer[5];
extern float temperature;
extern float humidity;
extern uint16_t temp;
extern uint16_t umid;
extern uint16_t mID;
extern uint16_t cID;
extern uint16_t clock_timeout;
extern uint8_t rxCount;
extern uint8_t txCount;
