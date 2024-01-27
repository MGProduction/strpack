# strpack
yet another simple string compression (with fast/handy decompression to use also in 8bit machines)

This simple C library is a modified version of what I use in my **Storytlr64 engine** (IF engine for **C64**) to compress messages and element names. It's supposed to compress even small strings, but it's meant to be used with single-byte char strings (NOT multibyte, like utf-8).

It's somehow similar to other libraries ([smaz](https://github.com/antirez/smaz), [shoco](https://github.com/Ed-von-Schleck/shoco), etc) because it's a "dictionary-based" compressor BUT it uses a minimal dictionary, with mainly bigrams and a reduced set of longer strings, because in low memory systems even dictionary size AND usage matters. It also uses some LZ/RLE code (if you want it, it's disabled by default because that code can be useful only on longer strings).

As with smaz, compressed strings are still strings, with a /0 only at the end.
