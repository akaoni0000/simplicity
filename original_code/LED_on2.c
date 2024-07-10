#include <SI_EFM8SB1_Register_Enums.h>

sbit LED = P0^5;

void main (void) {

   while (1) {
       LED = 1;
   }

}
