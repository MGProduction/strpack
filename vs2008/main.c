// sample.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRPACK_IMPLEMENT
//#define STRPACK_DECOMPRESSONLY
//#define STRPACK_USEEXTERNALDICTIONARY
#define STRPACK_IMPLEMENT_BUILD
#include "../strpack.h"

int strpack_testcompressfile(const char*corpus)
{
 FILE*f=fopen(corpus,"rb");
 if(f)
  {
   int real=0,compressed=0,err=0;
   while(!feof(f))
    {
     char line[9102],oline[1024],cline[1024];
     fgets(line,sizeof(line),f);
     real+=(int)strlen(line);
     compressed+=strpack_compress(line,oline,sizeof(oline),NULL);
     strpack_decompress(oline,cline,sizeof(cline));
     if(strcmp(line,cline))
      err++;
    }
   fclose(f);
   return compressed*100/real;
  }
 return 0;
}

int main(int argc, char* argv[])
{
 const char samples[][256]={"the",
                            "This is a small string",
                            "foobar",
                            "the end",
                            "not-a-g00d-Exampl333",
                            "Smaz is a simple compression library",
                            "Nothing is more difficult, and therefore more precious, than to be able to decide",
                            "this is an example of what works very well with smaz",
                            "1000 numbers 2000 will 10 20 30 compress very little",
                            "Nel mezzo del cammin di nostra vita, mi ritrovai in una selva oscura",
                            "Mi illumino di immenso",
                            "L'autore di questa libreria vive in Sicilia",
                            "http://google.com",
                            "http://programming.reddit.com",
                            "http://github.com/antirez/smaz/tree/master",
                            "My name's Marco Giorgini and I live in Modena",
                            "L'autore di questa libreria vive in Emilia Romagna",
                            "strpack is somewhat similar to smaz or shoco",
                            "The program is designed to work well with English text",
                            "As long as the messages are latin letters natural language messages with common statistical properties, the program will only seldom use more space than needed",
                            "Anche se in maniera meno efficiente, questo algoritmo di compressione è in grado di comprimere testi in altre lingue.",
                            ""};
 char pstr[256];
 int  i,len,perc,gperc=0;
 FILE*f;

 strpack_build("bin\\TheMoonstone.txt","..\\strpack_dictionary.h",0); // 

 strpack_testcompressfile("bin\\TheMoonstone.txt");

 f=fopen("test.txt","wb");
 for(i=0;*samples[i];i++)
 {
  len=strpack_compress(samples[i],pstr,sizeof(pstr),&perc);gperc+=perc;
  fprintf(f,"'%s' compressed by %d%%\n",samples[i],100-perc);
 }
 fclose(f);

	return 0;
}

