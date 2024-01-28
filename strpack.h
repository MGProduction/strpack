//
// strpack.h - v0.11 - string compressor/decompressor
//
// single-file C implementation by Marco Giorgini 
//
// LICENSE
//
//   See end of file.
//
// to create the implementation,
//     #define STRPACK_IMPLEMENT
// in *one* C/CPP file that includes this file.
//
// if you need ONLY decompression you can add
//     #define STRPACK_DECOMPRESSONLY
//
// default mono+bigram dictionary are created for ANSI English text
// you can create/optimize this dictionary using 
//
//  strpack_build(<filename>,"strpack_dictionary.h",<flags>)
//  
// to call this extra function you need to add this define
//     #define STRPACK_IMPLEMENT
//
// once generated the optimized dictionary you can force its usage
// using
//     #define STRPACK_USEEXTERNALDICTIONARY
//

#ifndef _STRPACK_H
#define _STRPACK_H

#ifdef __cplusplus
extern "C" {
#endif

#define strpack_full
#define strpack_safe

#if defined(strpack_full)
#define strpackcode_firstcode   251
#define strpackcode_emitbackref 251
#define strpackcode_emitrle     252
#else
#define strpackcode_firstcode   253
#endif
#define strpackcode_emitext     253
#define strpackcode_emitchar    254
#define strpackcode_emitseq     255

typedef unsigned char          u8;

#if defined(STRPACK_USEEXTERNALDICTIONARY)
#include "strpack_dictionary.h"
#else
// ANSI - optimized for English Texts
u8 strpack_dictionary[399]={0x30,0xae,0x00,0x0a,0x0d,0x20,0x21,0x2c,0x2d,0x2e,0x3b,0x3f,0x41,0x42,0x43,0x46,0x48,0x49,0x4d,0x52,0x53,0x54,0x57,0x5f,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
0x68,0x69,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x92,0x93,0x94,0x97,0x0d,0x0a,0x20,0x49,0x20,0x4d,0x20,0x61,0x20,0x62,0x20,0x63,
0x20,0x64,0x20,0x65,0x20,0x66,0x20,0x67,0x20,0x68,0x20,0x69,0x20,0x6c,0x20,0x6d,0x20,0x6e,0x20,0x6f,0x20,0x70,0x20,0x72,0x20,0x73,0x20,0x74,0x20,0x77,0x20,0x79,
0x2c,0x20,0x2e,0x0d,0x2e,0x20,0x2e,0x94,0x49,0x20,0x4d,0x72,0x54,0x68,0x61,0x20,0x61,0x63,0x61,0x64,0x61,0x69,0x61,0x6c,0x61,0x6d,0x61,0x6e,0x61,0x72,0x61,0x73,
0x61,0x74,0x61,0x76,0x61,0x79,0x62,0x65,0x62,0x6c,0x62,0x6f,0x63,0x61,0x63,0x65,0x63,0x68,0x63,0x6f,0x63,0x74,0x64,0x20,0x64,0x2c,0x64,0x65,0x64,0x69,0x64,0x6f,
0x65,0x0d,0x65,0x20,0x65,0x2c,0x65,0x61,0x65,0x63,0x65,0x64,0x65,0x6c,0x65,0x6e,0x65,0x72,0x65,0x73,0x65,0x74,0x66,0x20,0x66,0x65,0x66,0x66,0x66,0x6f,0x66,0x72,
0x67,0x20,0x67,0x65,0x67,0x68,0x68,0x20,0x68,0x61,0x68,0x65,0x68,0x69,0x68,0x6f,0x69,0x63,0x69,0x64,0x69,0x65,0x69,0x6c,0x69,0x6d,0x69,0x6e,0x69,0x6f,0x69,0x72,
0x69,0x73,0x69,0x74,0x6b,0x20,0x6b,0x65,0x6c,0x20,0x6c,0x61,0x6c,0x64,0x6c,0x65,0x6c,0x69,0x6c,0x6c,0x6c,0x6f,0x6c,0x79,0x6d,0x20,0x6d,0x61,0x6d,0x65,0x6d,0x69,
0x6d,0x6f,0x6d,0x79,0x6e,0x20,0x6e,0x63,0x6e,0x64,0x6e,0x65,0x6e,0x67,0x6e,0x69,0x6e,0x6f,0x6e,0x73,0x6e,0x74,0x6f,0x20,0x6f,0x66,0x6f,0x6d,0x6f,0x6e,0x6f,0x6f,
0x6f,0x72,0x6f,0x73,0x6f,0x74,0x6f,0x75,0x6f,0x77,0x70,0x65,0x70,0x6c,0x70,0x6f,0x70,0x70,0x70,0x72,0x72,0x20,0x72,0x2e,0x72,0x61,0x72,0x65,0x72,0x69,0x72,0x6f,
0x72,0x73,0x72,0x74,0x72,0x79,0x73,0x20,0x73,0x2c,0x73,0x61,0x73,0x65,0x73,0x68,0x73,0x69,0x73,0x6f,0x73,0x73,0x73,0x74,0x74,0x20,0x74,0x2c,0x74,0x61,0x74,0x65,
0x74,0x68,0x74,0x69,0x74,0x6f,0x74,0x72,0x75,0x20,0x75,0x6c,0x75,0x6e,0x75,0x70,0x75,0x72,0x75,0x73,0x75,0x74,0x76,0x65,0x76,0x69,0x77,0x20,0x77,0x61,0x77,0x65,
0x77,0x68,0x77,0x69,0x77,0x6f,0x79,0x20,0x79,0x6f,0x92,0x73,0x94,0x0d,0x94,0x20};
u8 strpack_dictionary_extra[194]={0x1d,0x00,0x05,0x0a,0x12,0x17,0x1c,0x22,0x2a,0x2f,0x35,0x3a,0x40,0x45,0x4a,0x4f,0x54,0x59,0x5f,0x64,0x69,0x70,0x75,0x7b,0x80,0x85,0x8c,0x91,0x96,0x9c,0xa3,
0x77,0x68,0x69,0x63,0x68,0x77,0x6f,0x75,0x6c,0x64,0x53,0x65,0x72,0x67,0x65,0x61,0x6e,0x74,0x61,0x62,0x6f,0x75,0x74,0x63,0x6f,0x75,0x6c,0x64,0x62,0x65,0x66,0x6f,
0x72,0x65,0x46,0x72,0x61,0x6e,0x6b,0x6c,0x69,0x6e,0x66,0x69,0x72,0x73,0x74,0x6c,0x69,0x74,0x74,0x6c,0x65,0x73,0x61,0x69,0x64,0x2c,0x6c,0x6f,0x6f,0x6b,0x65,0x64,
0x74,0x68,0x65,0x69,0x72,0x61,0x66,0x74,0x65,0x72,0x74,0x68,0x65,0x72,0x65,0x6d,0x69,0x67,0x68,0x74,0x6f,0x74,0x68,0x65,0x72,0x52,0x61,0x63,0x68,0x65,0x6c,0x6e,
0x65,0x76,0x65,0x72,0x66,0x6f,0x75,0x6e,0x64,0x44,0x69,0x61,0x6d,0x6f,0x6e,0x64,0x73,0x61,0x69,0x64,0x2e,0x6d,0x79,0x73,0x65,0x6c,0x66,0x68,0x65,0x61,0x72,0x64,
0x64,0x6f,0x6e,0x92,0x74,0x52,0x6f,0x73,0x61,0x6e,0x6e,0x61,0x73,0x68,0x61,0x6c,0x6c,0x62,0x65,0x69,0x6e,0x67,0x73,0x68,0x6f,0x75,0x6c,0x64,0x6e,0x6f,0x74,0x68,
0x69,0x6e,0x67};
#define strpack_dictionary_bigramstart 49
#define strpack_dictionary_trigramstart 223
#define strpack_dictionary_bibackref 223
#define strpack_dictionary_tribackref 0
#define strpack_dictionary_quadbackref 0
#endif

#if defined(STRPACK_IMPLEMENT_BUILD)
#define flags_ignore_cr         1
#define flags_minimaldict       2
#define flags_trigrams          4
#define flags_singleread        8
#endif

#if defined(STRPACK_IMPLEMENT)

int trigr_compare(const void*a,const void*b)
{
 return memcmp(a,b,3);
}
int bigr_compare(const void*a,const void*b)
{
 return memcmp(a,b,2);
}
int mono_compare(const void*a,const void*b)
{
 return memcmp(a,b,1);
}

#if defined(STRPACK_IMPLEMENT_BUILD)

int seq_compare(const void*a,const void*b)
{
 int*A=(int*)a;
 int*B=(int*)b;
 int dif=B[1]-A[1];
 if(dif)
  return dif;
 return A[0]-B[0];
}

#define SELECTED       0xFFFFFFFF

#define MASK_MONOGRAMS 0x20000000
#define MASK_BIGRAMS   0x40000000
#define MASK_TRIGRAMS  0x80000000

typedef struct{
 int  cnt;
 char*str;
}hstring;
typedef struct{
 hstring*hash;
 int     mhash,chash;
}hdict;
int  hstring_compare(const void*a,const void*b)
{
 hstring*A=(hstring*)a;
 hstring*B=(hstring*)b;
 return B->cnt-A->cnt;
}
void hdict_new(hdict*h,int size)
{
 memset(h,0,sizeof(hdict));
 if(size<=0)
  h->mhash=16*1024;
 else
  h->mhash=size;
 h->hash=(hstring*)calloc(h->mhash,sizeof(h->hash[0]));
}
void hdict_delete(hdict*h)
{
 int i;
 for(i=0;i<h->mhash;i++)
  if(h->hash[i].str)
   {free(h->hash[i].str);h->hash[i].str=(void*)0;}
 free(h->hash);
}
int hash_calc(const char*str,int len,int hash_size)
{
 unsigned int a, hash = 0;
 for (a = 0; a < len; a++) hash = hash * 257 + str[a];
 hash = hash % hash_size;
 return hash;
}
void hdict_add(hdict*h,const char*str,int cnt)
{
 int len=(int)strlen(str);
 int hv=hash_calc(str,len,h->mhash);
 while(1)
  if(h->hash[hv].str&&(strcmp(h->hash[hv].str,str)==0))
   {
    h->hash[hv].cnt+=cnt;
    break;
   }
  else
   if(h->hash[hv].cnt==0)
    {
     h->hash[hv].cnt=cnt;
     h->hash[hv].str=(char*)malloc(len+1);
     strcpy(h->hash[hv].str,str);
     h->chash++;
     if(h->chash+2>h->mhash)
      {
       hdict hh;
       int   i;
       hdict_new(&hh,h->mhash+8192);
       for(i=0;i<h->mhash;i++)
        if(h->hash[i].str)
         hdict_add(&hh,h->hash[i].str,h->hash[i].cnt);
       hdict_delete(h);
       memcpy(h,&hh,sizeof(hh));
      }
     break;
    }
   else
    hv = (hv + 1) % h->mhash;
}

int strpack_build(const char*corpus,int maxsize,int maxextsize,int flags,const char*output)
{
 int   i,j;
 u8    trigramstart,bigramstart;
 u8    mused=0,bused=0,lastcharused=0,extsize=0;
 int  *trigr,mono_count=0,bigr_count=0,trigr_count=0,t=0,m=0,b=0,skip=0;
 int  *seq,iseq=0,trigrsize=0;
 hdict hd;

 if(maxextsize>0)
  hdict_new(&hd,0);

 if(maxsize<=256)
  flags|=flags_minimaldict;

 if(flags&flags_trigrams)
  trigrsize=256*256*256;
 else
  trigrsize=256*256;
 trigr=(int*)calloc(trigrsize,sizeof(int));

 if(flags&flags_singleread)
  seq=(void*)0;
 else
  seq=(int*)calloc(256,sizeof(int)*2);

 while(iseq<strpackcode_firstcode-1)
  {
   FILE*f=fopen(corpus,"rb");
   if(f)
    {
     int top=0,mask=0,best=0;
     memset(trigr,0,sizeof(trigr[0])*trigrsize);
     for(i=0;i<iseq;i++)
      trigr[seq[i*2]&0x0FFFFFFF]=SELECTED;
     while(!feof(f))
      {
       char line[1024];     
       fgets(line,sizeof(line),f);
       if((iseq==0)&&(maxextsize>0))
        {
         i=0;
         while(line[i])
          {
           int  j=0,k=0;
           char wrd[16];
           while(line[i+j]&&(line[i+j]!=' ')&&(line[i+j]!='\r')&&(line[i+j]!='\n'))
            {
             if(k<sizeof(wrd)-1)
              wrd[k++]=line[i+j++];
             else
              j++;
            }
           wrd[k]=0;
           if((k>=5)&&(k<=12))
            hdict_add(&hd,wrd,1);
           i+=j;
           while((line[i]==' ')||(line[i]=='\r')||(line[i]=='\n'))
            i++;
          }
        }
       i=0;
       while(line[i])
        {
         u8 ch=(u8)line[i];
         if((flags&flags_ignore_cr)&&((ch=='\n')||(ch=='\r')))
          {i++;continue;}
         if(line[i+1])          
          {
           u8 nch=(u8)line[i+1];
           if(flags&flags_trigrams)
            if((flags&flags_ignore_cr)&&((nch=='\n')||(nch=='\r')))
             ;
            else
            if(line[i+2])
             {
              u8 nnch=(u8)line[i+2];
              if((flags&flags_ignore_cr)&&((nnch=='\n')||(nnch=='\r')))
               ;
              else
               if(trigr[ch+nch*256+nnch*65536]==SELECTED)
                {i+=3;continue;}
               else
                trigr[ch+nch*256+nnch*65536]++;
             }
           if(trigr[ch+nch*256]==SELECTED)
            {i+=2;continue;}
           else
            trigr[ch+nch*256]++;
          }
         if(trigr[ch]==SELECTED)
          {i++;continue;}
         else
          trigr[ch]++;
         i++;
        }
      }
     fclose(f);

     if(flags&flags_singleread)
      {
       int used=0;
       for(i=0;i<trigrsize;i++)
        if(trigr[i])
         used++;
       seq=(int*)calloc(used,sizeof(int)*2);
       for(i=0;i<trigrsize;i++)
        if(trigr[i])
         {
          int mask=0;
          if(i&0xFF0000)
           mask=MASK_TRIGRAMS;
          else
           if(i&0xFF00)
            mask=MASK_BIGRAMS;
           else
            mask=MASK_MONOGRAMS;
          seq[iseq*2]=i|mask;
          seq[iseq*2+1]=trigr[i];          
          iseq++;
         }
       qsort(seq,iseq,sizeof(seq[0])*2,seq_compare);
       for(i=0;i<min(strpackcode_firstcode-1,iseq);i++)
        if(seq[i*2]&MASK_TRIGRAMS)
         trigr_count++;
        else
        if(seq[i*2]&MASK_BIGRAMS)
         bigr_count++;
        else
         mono_count++;
       break;
      }
     else
      {
       for(i=0;i<trigrsize;i++)
        if(trigr[i]!=SELECTED)
         if(trigr[i]>top)
          {
           if(i&0xFF0000)
            mask=MASK_TRIGRAMS;
           else
            if(i&0xFF00)
             mask=MASK_BIGRAMS;
            else
             mask=MASK_MONOGRAMS;
           if((flags&flags_minimaldict)&&(mask&MASK_MONOGRAMS))
            ;
           else
            {
            top=trigr[i];
            best=i;
            }
          }

       seq[iseq*2]=mask|best;
       seq[iseq*2+1]=top;
       if(mask&MASK_BIGRAMS)
        bigr_count++;
       else
       if(mask&MASK_TRIGRAMS)
        trigr_count++;
       else
        mono_count++;
       iseq++;
      }
    }
   else
    break;
  }

 if(maxextsize>0)
  {
   int sum,cnt=0,c,pos=0;   
   qsort(hd.hash,hd.mhash,sizeof(hd.hash[0]),hstring_compare);
   for(sum=1+1,pos=c=i=0;i<hd.chash;i++)
    if(hd.hash[i].cnt>64)
     {
      int ln=strlen(hd.hash[i].str);
      if((sum+ln+1<maxextsize)&&(pos+ln<255))
       if(cnt<255)
        {cnt++;sum+=ln+1;pos+=ln;}
       else
        break;
     }
    else
     break;
   strpack_dictionary_extra[0]=(u8)cnt;
   for(sum=1+1,pos=c=i=0;i<hd.chash;i++)
    if(hd.hash[i].cnt>64)
     {
      int ln=strlen(hd.hash[i].str);
      if((sum+ln+1<maxextsize)&&(pos+ln<255))
       if(c<cnt)
        {
         strpack_dictionary_extra[1+c++]=(u8)pos;
         memcpy(strpack_dictionary_extra+1+(cnt+1)+pos,hd.hash[i].str,ln);
         pos+=ln;sum+=ln+1;
        }
     }
    else
     break;
   strpack_dictionary_extra[1+c++]=pos;
   extsize=1+(cnt+1)+pos;
   hdict_delete(&hd);
  }
 
 // minimal dict means NOT remapping used chars - so bigrams can go from lastcharused+1 to strpackcode_firstcode-1 only
 if(flags&flags_minimaldict)
  {
   int top;
   for(j=i=0;i<256*256;i++)
    if(trigr[i])
     if((i&0xFFFF00)==0)
      if(i>lastcharused)
       lastcharused=i;
   qsort(seq,iseq,sizeof(seq[0])*2,seq_compare);
   top=strpackcode_firstcode-(lastcharused+1);
   mono_count=bigr_count=trigr_count=0;
   for(i=0;i<min(top,iseq);i++)
    if((maxsize>0)&&(3+(bigr_count+1)*2>maxsize))
     ;
    else
     bigr_count++;
   bigramstart=lastcharused+1;
   trigramstart=bigramstart+bigr_count;
   strpack_dictionary[0]=0;
   strpack_dictionary[1]=(u8)bigr_count;     
   strpack_dictionary[2]=0;
   for(m=b=i=0;i<min(top,iseq);i++)
    if(b/2<bigr_count)
     {
      int ij=seq[i*2]&0x7FFFFFFF;
      strpack_dictionary[3+b++]=ij&0xFF;
      strpack_dictionary[3+b++]=ij>>8;
     }
   qsort(strpack_dictionary+3,strpack_dictionary[1],2,bigr_compare);
  }
 else
  {
   qsort(seq,iseq,sizeof(seq[0])*2,seq_compare);
   if(maxsize>0)
    {
     int size=3;
     mono_count=bigr_count=trigr_count=0;
     for(m=b=i=0;i<min(strpackcode_firstcode-1,iseq);i++)
      if(seq[i*2]&MASK_TRIGRAMS)
       {
        if(size+3>=maxsize)
         ;
        else
         {
          size+=3;
          trigr_count++;
         }
       }
      else
      if(seq[i*2]&MASK_BIGRAMS)
       {
        if(size+2>=maxsize)
         ;
        else
         {
          size+=2;
          bigr_count++;
         }
       }
      else
       if(size+1>=maxsize)
        ;
       else
        {
         size+=1;
         mono_count++;
        }
    }
   strpack_dictionary[0]=(u8)mono_count;
   strpack_dictionary[1]=(u8)bigr_count;
   strpack_dictionary[2]=(u8)trigr_count;
   bigramstart=mono_count+1;
   trigramstart=bigramstart+bigr_count;


   for(m=b=i=0;i<min(strpackcode_firstcode-1,iseq);i++)
    if(seq[i*2]&MASK_TRIGRAMS)
     {
      if(t/3<trigr_count)
       {
        int ij=seq[i*2]&0x0FFFFFFF;
        strpack_dictionary[3+mono_count+bigr_count*2+t++]=ij&0xFF;
        strpack_dictionary[3+mono_count+bigr_count*2+t++]=(ij>>8)&0xFF;
        strpack_dictionary[3+mono_count+bigr_count*2+t++]=(ij>>16)&0xFF;
       }
      else
       skip++;
     }
    else
    if(seq[i*2]&MASK_BIGRAMS)
     {
      if(b/2<bigr_count)
       {
        int ij=seq[i*2]&0x0FFFFFFF;
        strpack_dictionary[3+mono_count+b++]=ij&0xFF;
        strpack_dictionary[3+mono_count+b++]=ij>>8;
       }
      else
       skip++;
     }
    else
     if(m<mono_count)
      strpack_dictionary[3+m++]=seq[i*2];
     else
      skip++;

   qsort(strpack_dictionary+3,strpack_dictionary[0],1,mono_compare);
   qsort(strpack_dictionary+3+strpack_dictionary[0],strpack_dictionary[1],2,bigr_compare);
   qsort(strpack_dictionary+3+strpack_dictionary[0]+strpack_dictionary[1]*2,strpack_dictionary[2],3,trigr_compare);
  }
 if(output&&*output)
  {
   FILE*f=fopen(output,"wb");
   if(f)
    {    
     int hm=3+strpack_dictionary[0]+strpack_dictionary[1]*2+strpack_dictionary[2]*3;
     fprintf(f,"u8 strpack_dictionary[%d]={",hm);
     for(i=0;i<hm;i++)
     {
      if((i%32)==31) 
       fprintf(f,"\r\n");
      fprintf(f,"0x%02x",strpack_dictionary[i]);
      if(i+1<hm)
       fprintf(f,",");
     }
     fprintf(f,"};\r\n");
     if(extsize>0)
     {      
      fprintf(f,"u8 strpack_dictionary_extra[%d]={",extsize);
      for(i=0;i<extsize;i++)
      {
       if((i%32)==31) 
        fprintf(f,"\r\n");
       fprintf(f,"0x%02x",strpack_dictionary_extra[i]);
       if(i+1<extsize)
        fprintf(f,",");
      }
      fprintf(f,"};\r\n");
     }
     fprintf(f,"#define strpack_dictionary_bigramstart %d\r\n#define strpack_dictionary_trigramstart %d\r\n",bigramstart,trigramstart);
     if(trigramstart+trigr_count+16<strpackcode_firstcode)
      fprintf(f,"#define strpack_dictionary_bibackref %d\r\n",trigramstart+trigr_count);
     else
      fprintf(f,"#define strpack_dictionary_bibackref 0\r\n");
     if(trigramstart+trigr_count+16+16<strpackcode_firstcode)
      fprintf(f,"#define strpack_dictionary_tribackref %d\r\n",trigramstart+trigr_count+16);
     else
      fprintf(f,"#define strpack_dictionary_tribackref 0\r\n");
     if(trigramstart+trigr_count+16+16+16<strpackcode_firstcode)
      fprintf(f,"#define strpack_dictionary_quadbackref %d\r\n",trigramstart+trigr_count+16+16);
     else
      fprintf(f,"#define strpack_dictionary_quadbackref 0\r\n");
     fclose(f);
    }
  }

 free(trigr);
 return 1;
}
#endif

#if !defined(STRPACK_DECOMPRESSONLY)
#if defined(strpack_full)
u8 strpack_getsame(const char*text)
{
 u8   i=1;
 char cur=text[0];
 while(text[i]&&(text[i]==cur)&&(i<255))
  i++;
 if(i>3)
  return i;
 else
  return 0;
}
u8 strpack_getext(const char*text,u8*code)
{
 int i;
 for(i=0;i<strpack_dictionary_extra[0];i++)
 {
  u8 ln=strpack_dictionary_extra[1+i+1]-strpack_dictionary_extra[1+i];
  if(memcmp(text,strpack_dictionary_extra+1+strpack_dictionary_extra[0]+1+strpack_dictionary_extra[1+i],ln)==0)
   {
    if(code) *code=1+i;
    return ln;
   }
 }
 return 0;
}
//#define dbg_stats
#if defined(dbg_stats)
int dbg_backlen[64],dbg_backref[64];
#endif
u8 strpack_getbackref(const char*text,int i,u8*len,u8*code)
{
 int j=max(i-34,0),backlen=0,bestj=0;
 while(j<i)
  {
   if(text[j]==text[i])
    {
     int k=1;
     while((j+k<i)&&(text[j+k]==text[i+k])&&(k<7+2)) // 3 bits +2
      k++;
     if((k>1)&&(k>=backlen))
      {backlen=k;bestj=j;}
    }
   j++;
  }
 if(backlen)
  {
   u8 backref=(u8)(i-bestj);
#if defined(dbg_stats)    
   dbg_backlen[backlen]++;
   dbg_backref[backref]++;
#endif
   if(strpack_dictionary_bibackref)
    {
     if(backlen==2)
      if((backref>=2)&&(backref<2+16))
       {
        if(len) *len=backlen;
        if(code) *code=strpack_dictionary_bibackref+(backref-2);
        return backref;
       }
    }
   if(strpack_dictionary_tribackref)
    {
     if(backlen==3)
      if((backref>=3)&&(backref<3+16))
       {
        if(len) *len=backlen;
        if(code) *code=strpack_dictionary_tribackref+(backref-3);
        return backref;
       }
    }
   if(strpack_dictionary_quadbackref)
    {
     if(backlen==4)
      if((backref>=4)&&(backref<4+16))
       {
        if(len) *len=backlen;
        if(code) *code=strpack_dictionary_quadbackref+(backref-4);
        return backref;
       }
    }
   if(backlen>2)
    {
     if(len) *len=(u8)backlen;
     return backref;
    }
  }
 return 0;
}
#endif
#if defined(dbg_stats)
int cnt_monograms=0,cnt_bigrams=0,cnt_trigrams=0,cnt_literals=0,cnt_backrefs=0,cnt_shortbackrefs=0,cnt_rles=0;
#endif
#if defined(strpack_safe)
int strpack_compress(const char*text,char*packedtext,int maxpackedtext,int*perc)
#else
int strpack_compress(const char*text,char*packedtext,int*perc)
#endif
{
 const char*trigr_storage=strpack_dictionary+3+strpack_dictionary[0]+(strpack_dictionary[1]<<1);
 const char*bigr_storage=strpack_dictionary+3+strpack_dictionary[0];
 const char*mono_storage=strpack_dictionary+3;
 int        i=0,j=0,lastemitchar=0; 
 while(text[i])
  {
   u8 code=0,codepar1=0,codepar2=0,codecoverage=1;
#if defined(strpack_full)
   u8 same=strpack_getsame(text+i);
   u8 backcode=0,backlen=0,backref=strpack_getbackref(text,i,&backlen,&backcode);
   u8 extcode=0,extlen=strpack_getext(text+i,&extcode);
   if((same>backlen)&&(same>extlen))
   {
#if defined(dbg_stats)
    cnt_rles++;
#endif
    code=strpackcode_emitrle;
    codecoverage=codepar1=same;codepar2=text[i];
   }
   else
   if(backlen>extlen)
    {
     if(backcode)
      {
       code=backcode;
 #if defined(dbg_stats)
      cnt_shortbackrefs++;    
 #endif
      }
     else
      {
 #if defined(dbg_stats)
       cnt_backrefs++;    
 #endif
       code=strpackcode_emitbackref;
       codepar1=(backlen-2)|(backref-backlen)<<3;
      }
     codecoverage=backlen;
    }
   else
   if(extlen)
    {
     code=strpackcode_emitext;
     codepar1=extcode;
     codecoverage=extlen;
    }
#endif
   if((code==0)&&(text[i+1])&&(text[i+2]))
    {
     const char*fnd=bsearch(text+i,trigr_storage,strpack_dictionary[2],3,trigr_compare);
     if(fnd)
      {
       code=strpack_dictionary_trigramstart+((fnd-trigr_storage)/3);
       codecoverage=3;
#if defined(dbg_stats)
       cnt_trigrams++;    
#endif
      }
    }
   if((code==0)&&(text[i+1]))
    {
     const char*fnd=bsearch(text+i,bigr_storage,strpack_dictionary[1],2,bigr_compare);
     if(fnd)
      {
       code=strpack_dictionary_bigramstart+((fnd-bigr_storage)>>1);
       codecoverage=2;
#if defined(dbg_stats)
       cnt_bigrams++;    
#endif
      }
    }
   if(code==0)
    if(strpack_dictionary[0]==0)
     if(text[i]<strpack_dictionary_bigramstart)
      code=text[i];
     else
      code=0;
    else
     {
      const char*fnd=bsearch(text+i,mono_storage,strpack_dictionary[0],1,mono_compare);
      if(fnd)
       {
        code=1+(fnd-mono_storage);
#if defined(dbg_stats)
        cnt_monograms++;    
#endif
       }
     }
   if(code==0)
    {
     if(lastemitchar==-1)
      lastemitchar=j;
     packedtext[j++]=strpackcode_emitchar;
     packedtext[j++]=text[i];   
#if defined(dbg_stats)
     cnt_literals++;    
#endif
    }
   else
    {
     if(lastemitchar!=-1)
      {
       if(j-lastemitchar>2)
        {
         int  k,s=0;
         char seq[256];
         for(k=lastemitchar;k<j;k+=2)
          seq[s++]=packedtext[k+1];
         j=lastemitchar;
         packedtext[j++]=strpackcode_emitseq;
         packedtext[j++]=s;
         for(k=0;k<s;k++)
          packedtext[j++]=seq[k];
        }
      }
     lastemitchar=-1;
     packedtext[j++]=code;
     if(codepar1)
     {
      packedtext[j++]=codepar1;
      if(codepar2)
       packedtext[j++]=codepar2;
     }
    }
   i+=codecoverage;
  }
 packedtext[j]=0;
 if(perc)
  *perc=j*100/i;
 return j;
}
#endif

#if defined(strpack_safe)
int strpack_decompress(const char*packedtext,char*text,int textsize)
#else
int strpack_decompress(const char*packedtext,char*text)
#endif
{ 
 int        i=0,j=0; 
 while(packedtext[i])
  {
   u8 ch=packedtext[i++],ln;
   switch(ch)
    {
#if defined(strpack_full)
    case strpackcode_emitext:
     {
      u8 code=packedtext[i++],cnt=strpack_dictionary_extra[0];
      u8*ptr=strpack_dictionary_extra+1+cnt+1+strpack_dictionary_extra[code];
      ln=strpack_dictionary_extra[1+code]-strpack_dictionary_extra[code];
      while(ln--)
 #if defined(strpack_safe)
       if(j+1>=textsize)
        break;
       else
#endif
       text[j++]=*ptr++;
     }
    break;
    case strpackcode_emitbackref:
     {
      u8  mask=packedtext[i++];
      u8  ln=(mask&0x7)+2,pos=(mask>>3)+ln;
      int k=j-pos;
      while(ln--)
#if defined(strpack_safe)
       if(j+1>=textsize)
        break;
       else
#endif
       text[j++]=text[k++];
     }
     break;
    case strpackcode_emitrle:
     {
      ln=packedtext[i++];
      ch=packedtext[i++];
      while(ln--)
#if defined(strpack_safe)
       if(j+1>=textsize)
        break;
       else
#endif
       text[j++]=ch;
     }
    break;
#endif
    case strpackcode_emitseq:
     {
      ch=packedtext[i++];
      while(ch--)
#if defined(strpack_safe)
       if(j+1>=textsize)
        break;
       else
#endif
        text[j++]=packedtext[i++];
     }
    break;
    case strpackcode_emitchar:
#if defined(strpack_safe)
      if(j+1>=textsize)
       break;
      else
#endif
     text[j++]=packedtext[i++];
    break;
    default:
     if( (strpack_dictionary_quadbackref&&(ch>=strpack_dictionary_quadbackref)&&(ln=4)) ||
         (strpack_dictionary_tribackref&&(ch>=strpack_dictionary_tribackref)&&(ln=3)) ||
         (strpack_dictionary_bibackref&&(ch>=strpack_dictionary_bibackref)&&(ln=2)) )
      {              
       int k;
       if(ln==2)
        ch-=strpack_dictionary_bibackref;
       else
        if(ln==3)
         ch-=strpack_dictionary_tribackref;
        else
         if(ln==4)
          ch-=strpack_dictionary_quadbackref;
       k=j-(ch+ln);
       while(ln--)
#if defined(strpack_safe)
        if(j+1>=textsize)
         break;
        else
#endif
        text[j++]=text[k++];
      }
     else
      {
       const char*trigr_storage=strpack_dictionary+3+strpack_dictionary[0]+(strpack_dictionary[1]<<1);
       const char*bigr_storage=strpack_dictionary+3+strpack_dictionary[0];
       const char*mono_storage=strpack_dictionary+3;  
       const char*ptr;
       u8         ln=0;
       if(ch>=strpack_dictionary_trigramstart)
        {       
         ch-=strpack_dictionary_trigramstart;
         ptr=trigr_storage+(ch<<1)+ch;
         ln=3;
        }
       else
       if(ch>=strpack_dictionary_bigramstart)
        {
         ch-=strpack_dictionary_bigramstart;
         ptr=bigr_storage+(ch<<1);
         ln=2;
        }
       else
       if(strpack_dictionary[0])
        {ptr=mono_storage+(ch-1);ln=1;}
       if(ln)
        {
         while(ln--)
 #if defined(strpack_safe)
          if(j+1>=textsize)
           break;
          else
 #endif
          text[j++]=*ptr++;
        }
       else
 #if defined(strpack_safe)
        if(j+1>=textsize)
         break;
        else
 #endif
        text[j++]=ch;
      }
    }
  }
 text[j]=0;
 return j;
}

#else

#if defined(STRPACK_IMPLEMENT_BUILD)
int strpack_build(const char*corpus,int flags);
#endif

#if !defined(STRPACK_DECOMPRESSONLY)
#if defined(strpack_safe)
int strpack_compress(const char*text,char*packedtext,int maxpackedtext,int*perc);
#else
int strpack_compress(const char*text,char*packedtext,int*perc);
#endif
#endif

#if defined(strpack_safe)
int strpack_decompress(const char*packedtext,char*text,int textsize);
#else
int strpack_decompress(const char*packedtext,char*text);
#endif
#endif

#endif

#ifdef __cplusplus
}
#endif


//
// MIT License
// 
// Copyright (c) 2024 Marco Giorgini
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//