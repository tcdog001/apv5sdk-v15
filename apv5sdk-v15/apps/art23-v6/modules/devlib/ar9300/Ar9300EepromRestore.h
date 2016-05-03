
//
// the maximum number of states we write in the memory
//
#define MSTATE 100

#define MOUTPUT 2048

#define MDEFAULT 15

#define MVALUE 100

enum CompressAlgorithm
{
    _CompressNone=0,
    _CompressLzma,
    _CompressPairs,
    _CompressBlock,
    _Compress4,
    _Compress5,
    _Compress6,
    _Compress7,
};


#define ReferenceCurrent 0


#define CompressionHeaderLength 4

#define CompressionChecksumLength 2

//
// code[4], reference [4], minor[8], major[4], length[12]
//
extern int CompressionHeaderUnpack(unsigned char *best, int *code, int *reference, int *length, int *major, int *minor);


//
// Restore the configuration structure by reading the eeprom.
// This function destroys any existing in-memory structure content.
//
extern int Ar9300EepromRestore(void);

//
// Read the configuration data from the eeprom.
// The data can be put in any specified memory buffer.
//
// Returns -1 on error. 
// Returns address of next memory location on success.
//
extern int Ar9300EepromRestoreInternal(unsigned char *mptr, int msize);


//
// Returns the first address in the eeprom at which we write configuration data.
//
extern int Ar9300EepromBaseAddress();


extern int Ar9300EepromLowLimit();


extern unsigned short Ar9300CompressionChecksum(unsigned char *data, int dsize);
