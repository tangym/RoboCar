/*
 * io_peripherals.h
 *
 *  Created on: Feb 3, 2018
 *      Author: steveb
 */

#ifndef IO_PERIPHERALS_H_
#define IO_PERIPHERALS_H_

struct pcm_register
{
  uint8_t unused; /* empty structure */
};

struct io_peripherals
{
  uint8_t               unused0[0x101000];
  struct cm_register    cm;               /* offset = 0x101000, width = 0xA8 */
  uint8_t               unused1[0xFF000-sizeof(struct cm_register)];
  struct gpio_register  gpio;             /* offset = 0x200000, width = 0x84 */
  uint8_t               unused2[0x3000-sizeof(struct gpio_register)];
  struct pcm_register   pcm;              /* offset = 0x203000, width = 0x24 */
  uint8_t               unused3[0x1000-sizeof(struct pcm_register)];
  struct spi_register   spi;              /* offset = 0x204000, width = 0x18 */
  uint8_t               unused4[0x8000-sizeof(struct spi_register)];
  struct pwm_register   pwm;              /* offset = 0x20c000, width = 0x28 */
};

#endif /* IO_PERIPHERALS_H_ */
