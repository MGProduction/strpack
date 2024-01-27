# strpack
yet another simple string compression (with a fast / handy decompression for use on 8 bit machines as well)

This simple C library is a modified version of what I use in my **Storytllr64 engine** (IF engine for **C64**) to compress messages and element names. 
It's supposed to compress even small strings, and it's meant to be used with single-byte char strings (NOT multi-byte, like utf-8).

## usage

	#define STRPACK_IMPLEMENT
	#include "../strpack.h"
	
	...
	char pstr[256];	
	strpack_compress("This is a test",pstr,sizeof(pstr),NULL;
	...
	char ostr[256];
	strpack_decompress(pstr,ostr,sizeof(ostr);

	If you don't need a compressor (i.e. because you compress resources in a separate program, then your program just uses them) you can add
 
	#define STRPACK_DECOMPRESSONLY

	If you instead want to optimize strpack internal dictionary you can add 
 
	#define STRPACK_IMPLEMENT_BUILD
 
	and use 
 
	strpack_build("corpus.txt","strpack_dictionary.h",0);
 
	to generate a new one and then adding
 
	#define STRPACK_USEEXTERNALDICTIONARY
 
	to specify that you want to include what is in that file as a dictionary

## info

It's somewhat similar to other libraries ([smaz](https://github.com/antirez/smaz), [shoco](https://github.com/Ed-von-Schleck/shoco), etc) in that it's a "dictionary-based" compressor, BUT it uses a minimal dictionary, with mainly bigrams and a reduced set of longer strings (not yet included, although you can generate trigrams as well), because on low memory systems even dictionary size AND usage is important. 

It also uses some LZ/RLE code (not useful for short strings, but it helps to save extra space in medium-sized clauses).

As with smaz, compressed strings are still strings, with a /0 at the end (so you can e.g. strcat them and decompress the result).


This is a quick comparison to smaz (using its sample strings)

	'the' compressed by 34%
	'This is a small string' compressed by 46%
	'foobar' compressed by 34%
	'the end' compressed by 43%
	'not-a-g00d-Exampl333' enlarged by 15%
	'Smaz is a simple compression library' compressed by 39%
	'Nothing is more difficult, and therefore more precious, than to be able to decide' compressed by 46%
	'this is an example of what works very well with smaz' compressed by 45%
	'1000 numbers 2000 will 10 20 30 compress very little' compressed by 22%
	'Nel mezzo del cammin di nostra vita, mi ritrovai in una selva oscura' compressed by 39%
	'Mi illumino di immenso' compressed by 37%
	'L'autore di questa libreria vive in Sicilia' compressed by 38%
	'http://google.com' compressed by 12%
	'http://programming.reddit.com' compressed by 25%
	'http://github.com/antirez/smaz/tree/master' compressed by 10%
	'My name's Marco Giorgini and I live in Modena' compressed by 29%
	'L'autore di questa libreria vive in Emilia Romagna' compressed by 34%
	'strpack is somewhat similar to smaz or shoco' compressed by 41%
