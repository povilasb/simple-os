//////////////////////////////////////////////
// This code was made for the tutorial:
// "Making a Simple C kernel with Basic printf and clearscreen Functions"
//
// This code comes with absolutly
// NO WARRANTY
// you can not hold me(KJ), nor
// anyone else responsible for what
// this code does.
//
// This code is in the public domain,
// you may use it however you want
//////////////////////////////////////////////


#define WHITE_TXT 0x07 // white on black text

void k_clear_screen();
unsigned int k_printf(char *message, unsigned int line);


k_main() // like main in a normal C program
{
	k_clear_screen();
	k_printf("Hi!\nHow's this for a starter OS?", 0);
};

void k_clear_screen() // clear the entire text screen
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
};

unsigned int k_printf(char *message, unsigned int line) // the message and then the line #
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;

	i=(line*80*2);

	while(*message!=0)
	{
		if(*message=='\n') // check for a new line
		{
			line++;
			i=(line*80*2);
			*message++;
		} else {
			vidmem[i]=*message;
			*message++;
			i++;
			vidmem[i]=WHITE_TXT;
			i++;
		};
	};

	return(1);
};
