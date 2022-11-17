/*
 * pwm.h
 *
 *  Created on: Dec 27, 2016
 *      Author: steveb
 */

#ifndef PWM_H_
#define PWM_H_

union CTL_register
{
  struct CTL_register_field
  {
    uint32_t  PWEN1:1;
    uint32_t  MODE1:1;
    uint32_t  RPTL1:1;
    uint32_t  SBIT1:1;
    uint32_t  POLA1:1;
    uint32_t  USEF1:1;
    uint32_t  CLRF1:1;
    uint32_t  MSEN1:1;
    uint32_t  PWEN2:1;
    uint32_t  MODE2:1;
    uint32_t  RPTL2:1;
    uint32_t  SBIT2:1;
    uint32_t  POLA2:1;
    uint32_t  USEF2:1;
    uint32_t  reserved0:1;
    uint32_t  MSEN2:1;
    uint32_t  reserved1:16;
  }         field;
  uint32_t  value;
};

union STA_register
{
  struct STA_register_field
  {
    uint32_t  FULL1:1;
    uint32_t  EMPT1:1;
    uint32_t  WERR1:1;
    uint32_t  RERR1:1;
    uint32_t  GAPO1:1;
    uint32_t  GAPO2:1;
    uint32_t  GAPO3:1;
    uint32_t  GAPO4:1;
    uint32_t  BERR:1;
    uint32_t  STA1:1;
    uint32_t  STA2:1;
    uint32_t  STA3:1;
    uint32_t  STA4:1;
    uint32_t  reserved:19;
  }         field;
  uint32_t  value;
};

union DMAC_register
{
  struct DMAC_register_field
  {
    uint32_t  DREQ:8;
    uint32_t  PANIC:8;
    uint32_t  reserved:15;
    uint32_t  ENAB:1;
  }         field;
  uint32_t  value;
};

struct pwm_register
{
  union CTL_register  CTL;
  union STA_register  STA;
  union DMAC_register DMAC;
  uint32_t            unused0;
  uint32_t            RNG1;
  uint32_t            DAT1;
  uint32_t            FIF1;
  uint32_t            unused1;
  uint32_t            RNG2;
  uint32_t            DAT2;
};

#endif /* PWM_H_ */
