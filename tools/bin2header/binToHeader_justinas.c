#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[])
{
   FILE *file;
   FILE *wfile;
   char *buffer;
   char fname[255];

   //printf("Failo pavadinimas: ");
   //scanf("%s",&fname);
   sprintf(fname,"%s",argv[1]);

   int place = strstr(fname,".") - fname;
   char newfname[255];
   strncpy(newfname,fname,place);
   strcat(newfname,".h");

   file = fopen(fname,"rb");
   wfile = fopen(newfname,"w");

   fseek(file,0,SEEK_END);
   long fsize = ftell(file);
   rewind(file);

   buffer = (char*)malloc(fsize);
   fread(buffer,1,fsize,file);

   int i;

   for(i = 1; i <= fsize; i++)
   {

	   fprintf(wfile,"0x");
	   char byte;
	   byte = (*(buffer+i-1));

	   if(!byte)
	   {
		  fprintf(wfile,"00, ");
	   }
	   else if(i%2 == 1)
	   {
		   byte = (*(buffer+i-1));
		   char *temp = (char*) malloc(8);
		   sprintf(temp,"%x",byte);
		   char temp2[2];
		   strncpy(temp2,(temp+6),2);
		   fprintf(wfile,"%c%c, ",toupper(temp2[0]),toupper(temp2[1]));
		   free(temp);
	   }
	   else
	   {
		   byte = (*(buffer+i-1));
		   char *temp = (char*) malloc(8);
		   sprintf(temp,"%x",byte);
		   int j;
		   for(j = 0; j < strlen(temp); j++)
		   {
			  *(temp+j) = toupper(*(temp+j));
		   }
		   if(strlen(temp) == 1)
		   {
			  fprintf(wfile,"0%s, ",temp);
		   }
		   else
		   {
			  fprintf(wfile,"%s, ",temp);
		   }
		   free(temp);
	   }

	   if(i%10 == 0)
	   {
		  fprintf(wfile,"\n");
	   }

   }

   free(buffer);
   fclose(file);
   fclose(wfile);

   return 0;
}
