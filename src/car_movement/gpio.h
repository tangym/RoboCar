#define GPFSEL_INPUT               0x0
#define GPFSEL_OUTPUT              0x1
#define GPFSEL_ALTERNATE_FUNCTION0 0x4
#define GPFSEL_ALTERNATE_FUNCTION1 0x5
#define GPFSEL_ALTERNATE_FUNCTION2 0x6
#define GPFSEL_ALTERNATE_FUNCTION3 0x7
#define GPFSEL_ALTERNATE_FUNCTION4 0x3
#define GPFSEL_ALTERNATE_FUNCTION5 0x2

union GPFSEL
{
  struct GPFSEL_field
  {
    uint32_t FSEL0:3;
    uint32_t FSEL1:3;
    uint32_t FSEL2:3;
    uint32_t FSEL3:3;
    uint32_t FSEL4:3;
    uint32_t FSEL5:3;
    uint32_t FSEL6:3;
    uint32_t FSEL7:3;
    uint32_t FSEL8:3;
    uint32_t FSEL9:3;
    uint32_t reserved:2;
  } field;
  uint32_t value;
};

#define GPIO_SET( REGISTER_SET, PIN ) \
  do \
  { \
    if ((PIN) < 32) \
    { \
      (REGISTER_SET)->GPSET0 = 1<<((PIN)%32); \
    } \
    else \
    { \
      (REGISTER_SET)->GPSET1 = 1<<((PIN)%32); \
    } \
  } while (0)

#define GPIO_CLR( REGISTER_SET, PIN ) \
  do \
  { \
    if ((PIN) < 32) \
    { \
      (REGISTER_SET)->GPCLR0 = 1<<((PIN)%32); \
    } \
    else \
    { \
      (REGISTER_SET)->GPCLR1 = 1<<((PIN)%32); \
    } \
  } while (0)

#define GPIO_READ( REGISTER_SET, PIN ) \
  ((((PIN) < 32) ? (REGISTER_SET)->GPLEV0 : (REGISTER_SET)->GPLEV1) & (1<<((PIN)%32)))

struct gpio_register
{
  union GPFSEL GPFSEL0;
  union GPFSEL GPFSEL1;
  union GPFSEL GPFSEL2;
  union GPFSEL GPFSEL3;
  union GPFSEL GPFSEL4;
  union GPFSEL GPFSEL5;

  uint32_t reserved0;

  uint32_t GPSET0;
  uint32_t GPSET1;

  uint32_t reserved1;

  uint32_t GPCLR0;
  uint32_t GPCLR1;

  uint32_t reserved2;

  uint32_t GPLEV0;
  uint32_t GPLEV1;

  uint32_t reserved3;

  uint32_t GPEDS0;
  uint32_t GPEDS1;

  uint32_t reserved4;

  uint32_t GPREN0;
  uint32_t GPREN1;

  uint32_t reserved5;

  uint32_t GPFEN0;
  uint32_t GPFEN1;

  uint32_t reserved6;

  uint32_t GPHEN0;
  uint32_t GPHEN1;

  uint32_t reserved7;

  uint32_t GPLEN0;
  uint32_t GPLEN1;

  uint32_t reserved8;

  uint32_t GPAREN0;
  uint32_t GPAREN1;

  uint32_t reserved9;

  uint32_t GPAFEN0;
  uint32_t GPAFEN1;

  uint32_t reserved10;

  uint32_t GPUD;
  uint32_t GPPUDCLK0;
  uint32_t GPPUDCLK1;
};

