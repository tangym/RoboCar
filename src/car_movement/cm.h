/*
 * cm.h
 *
 *  Created on: Feb 3, 2018
 *      Author: steveb
 */

#ifndef CM_H_
#define CM_H_

#define CM_PASSWORD ((uint32_t)0x5A)

union CM_CTL_register
{
  struct CM_CTL_register_field
  {
    uint32_t  SRC:4;      // 0=GND, 1=oscillator, 2=testdebug0, 3=testdebug1, 4=PLLA per, 5=PLLC per, 6=PLLD per, 7=HDMI auxiliary, 8-15=GND
    uint32_t  ENAB:1;
    uint32_t  KILL:1;
    uint32_t  unused0:1;
    uint32_t  BUSY:1;
    uint32_t  FLIP:1;
    uint32_t  MASH:2;     // 0=integer division, 1=1-stage MASH, 2=2-stage MASH, 3=3-stage MASH
    uint32_t  unused1:13;
    uint32_t  PASSWD:8;   // set to CM_PASSWORD
  }         field;
  uint32_t  value;
};

union CM_DIV_register
{
  struct CM_DIV_register_field
  {
    uint32_t  DIVF:12;
    uint32_t  DIVI:12;
    uint32_t  PASSWD:8;   // set to CM_PASSWORD
  }         field;
  uint32_t  value;
};

struct cm_register
{
  uint8_t               unused0[0x70];
  union CM_CTL_register CM_GP0CTL;
  union CM_DIV_register CM_GP0DIV;
  union CM_CTL_register CM_GP1CTL;
  union CM_DIV_register CM_GP1DIV;
  union CM_CTL_register CM_GP2CTL;
  union CM_DIV_register CM_GP2DIV;
  uint8_t               unused1[0x18];
  union CM_CTL_register CM_PWMCTL;
  union CM_DIV_register CM_PWMDIV;
};

#endif /* CM_H_ */
