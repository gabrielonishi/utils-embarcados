#include "asf.h"

void init(void);

void init(void){
  // Initialize the board clock
  sysclk_init();
  // Disativa WatchDog Timer
  WDT->WDT_MR = WDT_MR_WDDIS;
}

int main(void)
{
  init();
  while (1){

  }
  return 0;
}