// sample.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRPACK_IMPLEMENT
//#define STRPACK_DECOMPRESSONLY
#define STRPACK_USEEXTERNALDICTIONARY
#define STRPACK_IMPLEMENT_BUILD
#include "../strpack.h"

//#define COMPARATIVE_TESTS
#if defined(COMPARATIVE_TESTS)
#include "bin/smaz.c"
#include "bin/smaz2.c"
#endif

int strpack_testcompressfile(const char*corpus)
{
 FILE*f=fopen(corpus,"rb");
 if(f)
  {
   int real=0,strpack_compressed=0,strpack_result=0,err=0;
   #if defined(COMPARATIVE_TESTS)
   int smaz_compressed=0,smaz_result=0;
   int smaz2_compressed=0,smaz2_result=0;
   #endif
   while(!feof(f))
    {
     char line[9102],oline[1024],cline[1024];
     fgets(line,sizeof(line),f);
     real+=(int)strlen(line);
     strpack_compressed+=strpack_compress(line,oline,sizeof(oline),NULL);
     strpack_decompress(oline,cline,sizeof(cline));
     if(strcmp(line,cline))
      err++;
     #if defined(COMPARATIVE_TESTS)
     smaz_compressed+=smaz_compress(line,strlen(line),oline,sizeof(oline));
     smaz2_compressed+=smaz2_compress(oline,sizeof(oline),line,strlen(line));
     #endif
    }
   fclose(f);
   #if defined(COMPARATIVE_TESTS)
   smaz_result=smaz_compressed*100/real;
   smaz2_result=smaz2_compressed*100/real;
   #endif
   strpack_result=strpack_compressed*100/real;
   return strpack_result;
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

 strpack_build("bin\\TheMoonstone.txt",512,256,0,"..\\strpack_dictionary.h"); // 
 
 strpack_testcompressfile("bin\\TheMoonstone.txt");
 strpack_testcompressfile("bin\\TheHoundofBaskerville.txt");

 strpack_build("bin\\debug_msg.txt",512,256,0,"..\\strpack_dictionary.h"); // 
 strpack_testcompressfile("bin\\debug_msg.txt");

 f=fopen("test.txt","wb");
 for(i=0;*samples[i];i++)
 {
  #if defined(COMPARATIVE_TESTS)
  int lens;
  #endif
  len=strpack_compress(samples[i],pstr,sizeof(pstr),&perc);gperc+=perc;
  #if defined(COMPARATIVE_TESTS)
  lens=smaz_compress(samples[i],strlen(samples[i]),pstr,sizeof(pstr));
  fprintf(f,"'%s' compressed to %d bytes (orig: %d smaz: %d)\n",samples[i],len,strlen(samples[i]),lens);
  #else
  fprintf(f,"'%s' compressed to its %d%%\n",samples[i],perc);
  #endif
 }
 fclose(f);

	return 0;
}

