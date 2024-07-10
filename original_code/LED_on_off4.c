#include <SI_EFM8SB1_Register_Enums.h>//0.7秒周期

sbit LED = P0^4;

int i;

void main (void) {

  XBR2 = 0x40;
  PCA0MD = 0x00;

  while (1) {
    LED = 0;
    for(i=1; i<=150000000000; i++){}
    LED = 1;
    for(i=1; i<=150000000000; i++){}
  }
}