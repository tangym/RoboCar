/*
 * enable_pwm_clock.c
 *
 *  Created on: Feb 3, 2018
 *      Author: steveb
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "import_registers.h"
#include "gpio.h"
#include "cm.h"
#include "pwm.h"
#include "spi.h"
#include "io_peripherals.h"
#include "enable_pwm_clock.h"

void enable_pwm_clock( volatile struct io_peripherals *io )
{
  /*
   * This logic was shamelessly stolen from the WiringPi library.
   *    Copyright (c) 2012-2017 Gordon Henderson
   *    Additional code for pwmSetClock by Chris Hall <chris@kchall.plus.com>
   *
   * Where did they get this information? I have no idea.
   * The word on the street is that Broadcom only hands out the full datasheet for the BCM2835 under NDA.
   * I assume that they either have signed the NDA or know someone who has.
   * The comments in the body of this function are the original authors.
   *
   * It looks like there is a block of configuration registers for the Clock Manager.
   * This range starts at offset 0x101000.
   * Within this range, the following are documented in the BCM2835 ARM Peripherals datasheet
   * 70 = CM_GP0CTL
   * 74 = CM_GP0DIV
   * 78 = CM_GP1CTL
   * 7C = CM_GP1DIV
   * 80 = CM_GP2CTL
   * 84 = CM_GP2DIV
   *
   * The WiringPi code suggests the following addresses:
   * A0 = PWMCTL
   * A4 = PWMDIV
   */
  int                   divisor = 32;     // 19.2 / 32 = 600KHz
  union CTL_register    pwm_control;
  union CM_CTL_register cm_control;
  union CM_DIV_register div_register;

  pwm_control = io->pwm.CTL;    // preserve PWM_CONTROL

  // We need to stop PWM prior to stopping PWM clock in MS mode otherwise BUSY stays high.
  io->pwm.CTL.value = 0;        // Stop PWM

  // Stop PWM clock before changing divisor. The delay after this does need to
  // this big (95uS occasionally fails, 100uS OK), it's almost as though the BUSY
  // flag is not working properly in balanced mode. Without the delay when DIV is
  // adjusted the clock sometimes switches to very slow, once slow further DIV
  // adjustments do nothing and it's difficult to get out of this mode.
  cm_control.value        = 0;
  cm_control.field.PASSWD = CM_PASSWORD;
  cm_control.field.ENAB   = 0;  // Stop PWM Clock
  cm_control.field.SRC    = 1;
  io->cm.CM_PWMCTL        = cm_control;
  usleep( 110 );                // prevents clock going sloooow

  while (io->cm.CM_PWMCTL.field.BUSY != 0)  // Wait for clock to be !BUSY
  {
    usleep( 1 );
  }

  div_register.value        = 0;
  div_register.field.PASSWD = CM_PASSWORD;
  div_register.field.DIVI   = divisor;
  io->cm.CM_PWMDIV          = div_register;

  cm_control.value        = 0;
  cm_control.field.PASSWD = CM_PASSWORD;
  cm_control.field.ENAB   = 1;  // Start PWM clock
  cm_control.field.SRC    = 1;
  io->cm.CM_PWMCTL        = cm_control;
  io->pwm.CTL = pwm_control;    // restore PWM_CONTROL

  return;
}

    /*
     * original state
     * CTL = 00008585
     * STA = 00000102
     * DMAC= 00000707
     * RNG1= 00000020
     * DAT1= 00000005
     * FIF1= 70776D30
     * RNG2= 00000020
     * DAT2= 0000001B
     *
     * after running "gpio -g mode 12 pwm"... note that this fixes transmission
     * CTL = 00000101
     * STA = 00000102
     * DMAC= 00000707
     * RNG1= 00000400
     * DAT1= 00000010
     * FIF1= 70776D30
     * RNG2= 00000400
     * DAT2= 00000010
     *
     * Other values seen
     * CTL = 00008585
     * STA = 00000602
     * DMAC= 00000707
     * RNG1= 00000020
     * DAT1= 00000010
     * FIF1= 70776D30
     * RNG2= 00000200
     * DAT2= 00000010
     */
