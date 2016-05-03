


//
// compress with LZMA
// return size of the ouput if scuessful or -1 if not
//
extern int CompressLzma(char *dptr, int dsize, char *output);

//
// uncompress with LZMA
// return size of the ouput if scuessful or -1 if not
//
extern int UnCompressLzma(char *dptr, int dsize, char *output, int maxOutSize);
