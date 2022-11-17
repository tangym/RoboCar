
 /**************************************************
* CMPEN 473, Spring 2022, Penn State University
* 
* Sample Program hw0a1blikey2 - Red   LED at GPIO12 Blinking
*                             - Green LED at GPIO13 Blinking
*                             - by single keyboard hit (without Enter Key hit)
* Revision V1.1   On 1/18/2022
* By Kyusun Choi
* 
* Red   LED on GPIO12 (with 500 ohm resistor in series)
* Green LED on GPIO13 (with 500 ohm resistor in series)
* 
* 'r' => red   LED on/off, hit 'r' key to toggle red   LED on/off
* 'g' => green LED on/off, hit 'g' key to toggle green LED on/off
*  hit any other key to quit
* 
***************************************************/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <fcntl.h>
#include "import_registers.h"
#include "cm.h"
#include "gpio.h"
#include "spi.h"
#include "pwm.h"
#include "io_peripherals.h"
#include "enable_pwm_clock.h"
#include "wait_period.h"
# include <unistd.h>

#define PWM_RANGE 100

int get_pressed_key(void)
{
  struct termios  original_attributes;
  struct termios  modified_attributes;
  int             ch;

  tcgetattr( STDIN_FILENO, &original_attributes );
  modified_attributes = original_attributes;
  modified_attributes.c_lflag &= ~(ICANON | ECHO);
  modified_attributes.c_cc[VMIN] = 1;
  modified_attributes.c_cc[VTIME] = 0;
  tcsetattr( STDIN_FILENO, TCSANOW, &modified_attributes );

  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &original_attributes );

  return ch;
}
int DLevel=50;
int temp;
char charachter;
char charachter_2;


int main( void )
{
  volatile struct io_peripherals *io;
  bool  done = false;
  
  io = import_registers();

  if (io != NULL)
  {
    /* print where the I/O memory was actually mapped to */
    printf( "mem at 0x%8.8X\n", (unsigned long)io );


    // Initially Clean the GPIO
    /* set the pin function to OUTPUT GPIO 5 and 6   */
    io->gpio.GPFSEL0.field.FSEL5 = GPFSEL_OUTPUT;
    io->gpio.GPFSEL0.field.FSEL6 = GPFSEL_OUTPUT;
    /* set the pin function to OUTPUT for GPIO22 and 23 */
    io->gpio.GPFSEL2.field.FSEL2 = GPFSEL_OUTPUT;
	io->gpio.GPFSEL2.field.FSEL3 = GPFSEL_OUTPUT;

  
    
  
    printf( "\n press 'w'-> forward, 's'->stop, 'x'->Backward, 'o'->Orange Turn On LED\n");
    printf( " press 'q' to  exit\n");
    enable_pwm_clock( io );
    
    /* set the pin function to alternate function 0 for GPIO12, PWM for LED on GPIO12 */
    /* set the pin function to alternate function 0 for GPIO13, PWM for LED on GPIO13 */
    io->gpio.GPFSEL1.field.FSEL2 = GPFSEL_ALTERNATE_FUNCTION0;
    io->gpio.GPFSEL1.field.FSEL3 = GPFSEL_ALTERNATE_FUNCTION0;

    /* configure the PWM channels */
    io->pwm.RNG1 = PWM_RANGE;     /* the range value, 100 level steps */
    io->pwm.RNG2 = PWM_RANGE;     /* the range value, 100 level steps */
    io->pwm.DAT1 = 1;             /* initial beginning level=1/100=1% */
    io->pwm.DAT2 = 1;             /* initial beginning level=1/100=1% */
    io->pwm.CTL.field.MODE1 = 0;  /* PWM mode */
    io->pwm.CTL.field.MODE2 = 0;  /* PWM mode */
    io->pwm.CTL.field.RPTL1 = 1;  /* not using FIFO, but repeat the last byte anyway */
    io->pwm.CTL.field.RPTL2 = 1;  /* not using FIFO, but repeat the last byte anyway */
    io->pwm.CTL.field.SBIT1 = 0;  /* idle low */
    io->pwm.CTL.field.SBIT2 = 0;  /* idle low */
    io->pwm.CTL.field.POLA1 = 0;  /* non-inverted polarity */
    io->pwm.CTL.field.POLA2 = 0;  /* non-inverted polarity */
    io->pwm.CTL.field.USEF1 = 0;  /* do not use FIFO */
    io->pwm.CTL.field.USEF2 = 0;  /* do not use FIFO */
    io->pwm.CTL.field.MSEN1 = 1;  /* use M/S algorithm, level=pwm.DAT1/PWM_RANGE */
    io->pwm.CTL.field.MSEN2 = 1;  /* use M/S algorithm, level=pwm.DAT2/PWM_RANGE */
    io->pwm.CTL.field.CLRF1 = 1;  /* clear the FIFO, even though it is not used */
    io->pwm.CTL.field.PWEN1 = 1;  /* enable the PWM channel */
    io->pwm.CTL.field.PWEN2 = 1;  /* enable the PWM channel */
    
    do
    {
      charachter=get_pressed_key();
      charachter_2 =charachter;
      switch (charachter)
      {
          
         
        //stop
        case 's':
          // clean GPIO22, RESER
          GPIO_CLR(&(io->gpio), 5);
          GPIO_CLR(&(io->gpio), 6);
          GPIO_CLR(&(io->gpio), 22);
          GPIO_CLR(&(io->gpio), 23);
          printf( "stop \n");
          break;
          //forward
        
        
        case 'w':
          GPIO_SET(&(io->gpio), 5);
          GPIO_CLR(&(io->gpio), 6);             //forward case- trhe car moves forward  if it's presased w on the keyboard
          GPIO_SET(&(io->gpio), 22);
          GPIO_CLR(&(io->gpio), 23);
          printf( "forward movement\n");
          for (int i=0;i<1000;i++){
            
            io->pwm.DAT1=DLevel;                // this is the time period it will go forward for the. sleep time whcih i used is basically usleep wgich makes the sleep time to be 10^6ms. sleep time maintains the previous state of the motor
            io->pwm.DAT2 =DLevel;
            usleep(1500);


          }
      
          GPIO_CLR(&(io->gpio), 5);
          GPIO_CLR(&(io->gpio), 6);
          GPIO_CLR(&(io->gpio), 22);
          GPIO_CLR(&(io->gpio), 23);                // after the motor is finished running for 2 secs it will clear the pins which at the ennd stop after a given time.
          
         
          break;
        
            
            
        
        
    
        
          //backward
        case 'x':
           GPIO_CLR(&(io->gpio), 5);
           GPIO_SET(&(io->gpio), 6);
           GPIO_CLR(&(io->gpio), 22);                  // Almost similar to forward
           GPIO_SET(&(io->gpio), 23);
           printf( "Backward movement\n");
           for (int i=0;i<1000;i++){
            io->pwm.DAT1=DLevel;
            io->pwm.DAT2 =DLevel;
            usleep(1500);
          }
          GPIO_CLR(&(io->gpio), 5);
          GPIO_CLR(&(io->gpio), 6);
          GPIO_CLR(&(io->gpio), 22);
          GPIO_CLR(&(io->gpio), 23);
          break;
          
          //left
        case 'a':
          GPIO_SET(&(io->gpio), 5);
          GPIO_CLR(&(io->gpio), 6);
          GPIO_CLR(&(io->gpio), 22);                //left  takes a left turn fort 0.7 secs. the time is controlled with a for loop and usleep
          GPIO_CLR(&(io->gpio), 23);
          printf("turn left\n");
          for (int i=0;i<700;i++){
            io->pwm.DAT1=DLevel;
            io->pwm.DAT2 =DLevel;
            usleep(500);
          }
          GPIO_CLR(&(io->gpio), 5);
          GPIO_CLR(&(io->gpio), 6);
          GPIO_CLR(&(io->gpio), 22);
          GPIO_CLR(&(io->gpio), 23);
          break;

          //right

        case 'd':
          GPIO_CLR(&(io->gpio), 5);
          GPIO_CLR(&(io->gpio), 6);                 // same explanation as left
          GPIO_SET(&(io->gpio), 22);
          GPIO_CLR(&(io->gpio), 23);
          printf("turn right\n");
          for (int i=0;i<700;i++){
            io->pwm.DAT1=DLevel;
            io->pwm.DAT2 =DLevel;
            usleep(500);
          }
          GPIO_CLR(&(io->gpio), 5);
          GPIO_CLR(&(io->gpio), 6);
          GPIO_CLR(&(io->gpio), 22);
          GPIO_CLR(&(io->gpio), 23);
          break;
        //speed down
        case 'j':
          if (DLevel>0)
            DLevel -=5;
          io->pwm.DAT1 = DLevel;
		  printf("Speed:%d\n", DLevel);
          break;

        //speed up
        case 'i':
          if (DLevel<100)
            DLevel +=5;
          io->pwm.DAT1 = DLevel;
		  printf("Speed:%d\n", DLevel);
          break;
        case 'q':
          done = true;
          break;
      }
    } while (!done);


    /* clean the GPIO pins */
    io->gpio.GPFSEL0.field.FSEL5 = GPFSEL_INPUT;
    io->gpio.GPFSEL0.field.FSEL6 = GPFSEL_INPUT;
    io->gpio.GPFSEL2.field.FSEL2 = GPFSEL_INPUT;
    io->gpio.GPFSEL2.field.FSEL3 = GPFSEL_INPUT;
    io->gpio.GPFSEL1.field.FSEL2 = GPFSEL_INPUT;
    io->gpio.GPFSEL1.field.FSEL3 = GPFSEL_INPUT;


    printf( "\n Key hit is not 'r' or 'g' key, now quiting ... \n");
    
  }

  else
  {
    ; /* warning message already issued */
  }

  return 0;
}
