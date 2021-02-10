#include <stdio.h>
#include<unistd.h> 

#define MAXA 100

int main ()
{
   char * sonuc[MAXA];
   //reads the result string and puts it into an array.
   read(3, sonuc , MAXA);

   FILE * fp;
   int i;
   
   fp = fopen ("sonuc.txt","w");
 
   fprintf (fp,sonuc);
   fclose (fp);
   return 0;
}