#define EZP_GPIO_START     0xFF  //  Start GPIO
#define EZP_GPIO_SET_DIR     0x01  // set input/output direction
#define EZP_GPIO_SET_DIR_IN      0x11 // only set one pin direction input
#define EZP_GPIO_SET_DIR_OUT     0x12 // only set one pin direction output
#define EZP_GPIO_READ        0x02     // read register data
#define EZP_GPIO_WRITE       0x03     // write data to register
#define EZP_GPIO_SET         0x21     // set register one bit to 1
#define EZP_GPIO_CLEAR       0x31     // set register one bit to 0
#define EZP_GPIO_READ_BIT        0x04 // read register one bit
#define EZP_GPIO_WRITE_BIT       0x05 // write register one bit
#define EZP_GPIO_READ_BYTE       0x06 // read a byte data from register
#define EZP_GPIO_WRITE_BYTE      0x07 // write a byte data to register
#define EZP_GPIO_READ_INT        0x02 //same as read
#define EZP_GPIO_WRITE_INT       0x03 //same as write
#define EZP_GPIO_SET_INT     0x21 //same as set
#define EZP_GPIO_CLEAR_INT       0x31 //same as clear
#define EZP_GPIO_ENABLE_INTP     0x08 // enable interrupt
#define EZP_GPIO_DISABLE_INTP    0x09 // disable interrupt
#define EZP_GPIO_REG_IRQ     0x0A     
#define EZP_GPIO_LED_SET     0x41    // set one pin to led and led behavior

