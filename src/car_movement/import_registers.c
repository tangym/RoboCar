#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "import_registers.h"

#if 0
#define PHYSICAL_ADDRESS 0x20000000 /* base for BCM2708 */
#elseif 0
#define PHYSICAL_ADDRESS 0x3F000000 /* base for BCM2709 */
#else
#define PHYSICAL_ADDRESS 0xFE000000 /* base for BCM2835 */
#endif
#define ADDRESS_LENGTH   0x02000000

volatile void * import_registers( void )
{
  volatile void * return_value; /* the return value of this function */
  int             mmap_file;    /* the file descriptor used to map the memory */

  mmap_file = open( "/dev/mem", O_RDWR|O_SYNC );
  if (mmap_file != -1)
  {
    /* try to put the physical I/O space at the same address range in the virtual address space */
    return_value = mmap(
      (void *)PHYSICAL_ADDRESS,
      ADDRESS_LENGTH,
      PROT_READ|PROT_WRITE|PROT_EXEC,
      MAP_SHARED,
      mmap_file,
      PHYSICAL_ADDRESS );
    if (return_value != MAP_FAILED)
    {
      ; /* mapped memory */
    }
    else
    {
      printf( "unable to map register space\n" );

      close( mmap_file );

      return_value = NULL;
    }
  }
  else
  {
    printf( "unable to open /dev/mem\n" );

    return_value = NULL;
  }

  return return_value;
}

