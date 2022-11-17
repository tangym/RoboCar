/*
 * spi.h
 *
 *  Created on: Jan 29, 2017
 *      Author: steveb
 */

#ifndef SPI_H_
#define SPI_H_

union CS_register
{
  struct CS_register_field
  {
    uint32_t  CS:2;
    uint32_t  CPHA:1;
    uint32_t  CPOL:1;
    uint32_t  CLEAR:2;
    uint32_t  CSPOL:1;
    uint32_t  TA:1;
    uint32_t  DMAEN:1;
    uint32_t  INTD:1;
    uint32_t  INTR:1;
    uint32_t  ADCS:1;
    uint32_t  REN:1;
    uint32_t  LEN:1;
    uint32_t  LMONO:1;
    uint32_t  TE_EN:1;
    uint32_t  DONE:1;
    uint32_t  RXD:1;
    uint32_t  TXD:1;
    uint32_t  RXR:1;
    uint32_t  RXF:1;
    uint32_t  CSPOL0:1;
    uint32_t  CSPOL1:1;
    uint32_t  CSPOL2:1;
    uint32_t  DMA_LEN:1;
    uint32_t  LEN_LONG:1;
    uint32_t  reserved:6;
  }         field;
  uint32_t  value;
};

union CLK_register
{
  struct CLK_register_field
  {
    uint32_t  CDIV:16;      /* SCK = APB_clock/CDIV; CDIV must be a multiple of 2 with 0,1 = 65536; APB_clock = 250MHz */
    uint32_t  reserved:16;
  }         field;
  uint32_t  value;
};

union DLEN_register
{
  struct DLEN_register_field
  {
    uint32_t  LEN:16;
    uint32_t  reserved:16;
  }         field;
  uint32_t  value;
};

union LTOH_register
{
  struct LTOH_register_field
  {
    uint32_t  TOH:4;
    uint32_t  reserved:28;
  }         field;
  uint32_t  value;
};

union DC_register
{
  struct DC_register_field
  {
    uint32_t  TDREQ:8;
    uint32_t  TPANIC:8;
    uint32_t  RDREQ:8;
    uint32_t  RPANIC:8;
  }         field;
  uint32_t  value;
};

struct spi_register
{
  union CS_register   CS;
  uint32_t            FIFO; /* only the low byte is valid during send/receive (the low byte is valid, the high 3 bytes are 0) */
  union CLK_register  CLK;
  union DLEN_register DLEN;
  union LTOH_register LTOH;
  union DC_register   DC;
};

#endif /* SPI_H_ */
