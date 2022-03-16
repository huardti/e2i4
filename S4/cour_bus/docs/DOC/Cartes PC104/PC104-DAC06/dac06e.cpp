#include <stdio.h>
#include <conio.h>
#undef outp				// for BC++ 4.51

#define	DAC_BASE	0x180
	int	code;

	char	* ptr0, *ptr1;

void dac06_setcode(unsigned int base_addr, int channel, int value)
{
	unsigned int chann_lsb;

	chann_lsb = base_addr + channel*2;


	outp(chann_lsb,value%256);
	outp(chann_lsb+1,value/256);

}

void main (void)
{
      printf("test DAC06\n");
      getch();
      printf(" 000");
		outp(DAC_BASE,0x00);
      outp(DAC_BASE+1,0x00);
       getch();
      printf(" 7FF");
      outp(DAC_BASE,0xFF);
      outp(DAC_BASE+1,0x07);
       getch();
      printf(" FFF");
      outp(DAC_BASE,0xFF);
		outp(DAC_BASE+1,0x0F);
      getch();
		printf("\ntest dent de scie RESET pour sortir\n");

		code = 0;
		while(1)
		{
			outp(DAC_BASE,code%256);
			outp(DAC_BASE+1,code/256);
			code += 16;
			if (code >= 4096)
			{
         	code = 0;
				//if(kbhit()) break;
			}
		}

}