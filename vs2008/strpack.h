#ifndef _STRPACK_H
#define _STRPACK_H

#ifdef __cplusplus
extern "C" {
#endif

#define max_size              512

#define strpack_full
#define strpack_safe

#if defined(strpack_full)
#define strpackcode_firstcode   252
#define strpackcode_emitbackref 252
#define strpackcode_emitrle     253
#else
#define strpackcode_firstcode   254
#endif
#define strpackcode_emitchar    254
#define strpackcode_emitseq     255

typedef unsigned char          u8;

u8 strpack_dictionary_bigramstart,
   strpack_dictionary_trigramstart,
   strpack_dictionary[max_size];

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

int strpack_build(const char*corpus,int flags)
{
 int  i,j;
 u8   mused=0,bused=0,lastcharused=0;
 int *trigr,mono_count=0,bigr_count=0,trigr_count=0,t=0,m=0,b=0;
 int *seq,iseq=0,trigrsize=0;

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
           top=trigr[i];
           best=i;
           if(i&0xFF0000)
            mask=MASK_TRIGRAMS;
           else
            if(i&0xFF00)
             mask=MASK_BIGRAMS;
            else
             mask=MASK_MONOGRAMS;
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
 
 // minimal dict means NOT remapping used chars - so bigrams can go from lastcharused+1 to strpackcode_firstcode-1 only
 if(flags&flags_minimaldict)
  {
   int top;
   for(j=i=0;i<256*256;i++)
    if(trigr[i]&0xFF00)
     {
      char b[3];
      b[0]=i&0xFF;b[1]=i>>8;b[2]=0;
      seq[j++]=MASK_BIGRAMS|i;
      seq[j++]=trigr[i];      
     }
   qsort(seq,j/2,sizeof(seq[0])*2,seq_compare);
   top=strpackcode_firstcode-(lastcharused+1);
   for(i=0;i<min(top,j/2);i++)
    bigr_count++;
   strpack_dictionary_bigramstart=lastcharused+1;
   strpack_dictionary_trigramstart=0;
   strpack_dictionary[0]=0;
   strpack_dictionary[1]=(u8)bigr_count;     
   strpack_dictionary[2]=0;
   for(m=b=i=0;i<min(top,j/2);i++)
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
   strpack_dictionary_bigramstart=mono_count+1;
   strpack_dictionary_trigramstart=strpack_dictionary_bigramstart+bigr_count;
   strpack_dictionary[0]=(u8)mono_count;
   strpack_dictionary[1]=(u8)bigr_count;
   strpack_dictionary[2]=(u8)trigr_count;

   for(m=b=i=0;i<min(strpackcode_firstcode-1,iseq);i++)
    if(seq[i*2]&MASK_TRIGRAMS)
     {
      int ij=seq[i*2]&0x0FFFFFFF;
      strpack_dictionary[3+mono_count+bigr_count*2+t++]=ij&0xFF;
      strpack_dictionary[3+mono_count+bigr_count*2+t++]=(ij>>8)&0xFF;
      strpack_dictionary[3+mono_count+bigr_count*2+t++]=(ij>>16)&0xFF;
     }
    else
    if(seq[i*2]&MASK_BIGRAMS)
     {
      int ij=seq[i*2]&0x0FFFFFFF;
      strpack_dictionary[3+mono_count+b++]=ij&0xFF;
      strpack_dictionary[3+mono_count+b++]=ij>>8;
     }
    else
     strpack_dictionary[3+m++]=seq[i*2];

   qsort(strpack_dictionary+3,strpack_dictionary[0],1,mono_compare);
   qsort(strpack_dictionary+3+strpack_dictionary[0],strpack_dictionary[1],2,bigr_compare);
   qsort(strpack_dictionary+3+strpack_dictionary[0]+strpack_dictionary[1]*2,strpack_dictionary[2],3,trigr_compare);
  }

 free(trigr);
 return 1;
}
#endif

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
u8 strpack_getbackref(const char*text,int i,u8*len)
{
 int j=max(i-34,0),maxk=0,bestj=0;
 while(j<i)
  {
   if(text[j]==text[i])
    {
     int k=1;
     while((j+k<i)&&(text[j+k]==text[i+k])&&(k<7+2)) // 3 bits +2
      k++;
     if((k>2)&&(k>maxk))
      {maxk=k;bestj=j;}
    }
   j++;
  }
 if(maxk)
  {
   if(len) *len=(u8)maxk;
   return (u8)(i-bestj);
  }
 else
  return 0;
}
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
 int        unpacked=0,packed=0,literals=0,backrefs=0,rles=0;
 while(text[i])
  {
   u8 code=0,codepar1=0,codepar2=0,codecoverage=1;
#if defined(strpack_full)
   u8 same=strpack_getsame(text+i);
   u8 backlen=0,backref=strpack_getbackref(text,i,&backlen);
   if(same>backlen)
   {
    rles++;
    code=strpackcode_emitrle;
    codecoverage=codepar1=same;codepar2=text[i];
   }
   else
   if(backlen)
   {
    backrefs++;
    code=strpackcode_emitbackref;
    codepar1=(backlen-2)|(backref-backlen)<<3;
    codecoverage=backlen;
   }
#endif
   if((code==0)&&(text[i+1])&&(text[i+2]))
    {
     const char*fnd=bsearch(text+i,trigr_storage,strpack_dictionary[2],3,trigr_compare);
     if(fnd)
      {
       code=strpack_dictionary_trigramstart+((fnd-trigr_storage)/3);
       codecoverage=3;
       packed++;
      }
    }
   if((code==0)&&(text[i+1]))
    {
     const char*fnd=bsearch(text+i,bigr_storage,strpack_dictionary[1],2,bigr_compare);
     if(fnd)
      {
       code=strpack_dictionary_bigramstart+((fnd-bigr_storage)>>1);
       codecoverage=2;
       packed++;
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
        unpacked++;
       }
     }
   if(code==0)
    {
     if(lastemitchar==-1)
      lastemitchar=j;
     packedtext[j++]=strpackcode_emitchar;
     packedtext[j++]=text[i];   
     literals++;
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

#if defined(strpack_safe)
int strpack_decompress(const char*packedtext,char*text,int textsize)
#else
int strpack_decompress(const char*packedtext,char*text)
#endif
{ 
 int        i=0,j=0; 
 while(packedtext[i])
  {
   u8 ch=packedtext[i++];
   switch(ch)
    {
#if defined(strpack_full)
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
      u8 ln=packedtext[i++];
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

#if defined(strpack_safe)
int strpack_compress(const char*text,char*packedtext,int maxpackedtext,int*perc);
#else
int strpack_compress(const char*text,char*packedtext,int*perc);
#endif

#if defined(strpack_safe)
int strpack_decompress(const char*packedtext,char*text,int textsize);
#else
int strpack_decompress(const char*packedtext,char*text);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif