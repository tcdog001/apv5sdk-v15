

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "Ar9300Device.h"

#include "wlantype.h"

#include "osdep.h"

#include "mEepStruct9300.h"

//#include "Compress.h"

#include "Ar9300EepromRestore.h"

//
// the address where the first configuration block is written
//
static int BaseAddress=0x3ff;                // 1KB

//
// the lower limit on configuration data
//
static int LowLimit=0x040;          // ??????


int Ar9300EepromBaseAddress()
{
    return BaseAddress;
}


int Ar9300EepromLowLimit()
{
    return LowLimit;
}


unsigned short Ar9300CompressionChecksum(unsigned char *data, int dsize)
{
    int it;
    int checksum=0;

    for(it=0; it<dsize; it++)
    {
        checksum += data[it];
        checksum &= 0xffff;
    }

    return checksum;
}


//
// code[3], reference [6], length[11], minor[8], major[4], 
//
int CompressionHeaderUnpack(unsigned char *best, int *code, int *reference, int *length, int *major, int *minor)
{
    unsigned int value[4];

    value[0]=best[0];
    value[1]=best[1];
    value[2]=best[2];
    value[3]=best[3];
    *code=((value[0]>>5)&0x0007);
    *reference=(value[0]&0x001f)|((value[1]>>2)&0x0020);
    *length=((value[1]<<4)&0x07f0)|((value[2]>>4)&0x000f);
    *major=(value[2]&0x000f);
    *minor=(value[3]&0x00ff);

    return 4;
}


static int EepromRead(int address, unsigned char *buffer, int many)
{
    int it;

    for(it=0; it<many; it++)
    {
        Ar9300EepromRead(address-it,buffer+it,1);
    }
    return 0;
}


static int UncompressBlock(unsigned char *mptr, int msize, unsigned char *block, int size)
{
    int it;
    int spot;
    int offset;
    int length;

    spot=0;
    for(it=0; it<size; it+=(length+2))
    {
        offset=block[it];
        offset&=0xff;
        spot+=offset;
        length=block[it+1];
        length&=0xff;
        if(length>0 && spot>=0 && spot+length<msize)
        {
//            printf("restore at %d: spot=%d offset=%d length=%d\n",it,spot,offset,length);
            memcpy(&mptr[spot],&block[it+2],length);
            spot+=length;       // added +length, th 090924
        }
        else if(length>0)
        {
//            printf("bad restore at %d: spot=%d offset=%d length=%d\n",it,spot,offset,length);
            return -1;
        }
    }
    return 0;
}


//
// Read the configuration data from the eeprom.
// The data can be put in any specified memory buffer.
//
// Returns -1 on error. 
// Returns address of next memory location on success.
//
int Ar9300EepromRestoreInternal(unsigned char *mptr, int msize)
{
    int cptr;
    unsigned char word[MOUTPUT]; 
    unsigned char *dptr;
    int code;
    int reference,length,major,minor;
    int osize;
    int it;
    int overhead;
    int ib;
    int usize;
    unsigned short checksum, mchecksum;
    //
    // the memory is written with a series of records describing differences in the data
    // from one pass to the next. we have to read all of them to end up
    // in the correct state.
    //
    cptr=BaseAddress;
    for(it=0; it<MSTATE; it++)
    {            
        //
        // read enough data to unpack any possible header
        // currently 2 16-bit words is enough
        //
        EepromRead(cptr,word,CompressionHeaderLength);
        //
        // are we done?
        // test for unwritten memory. Eeprom is all ones. OTP is all zeros.
        //
        if((word[0]==0 && word[1]==0 && word[2]==0 && word[3]==0) || 
            (word[0]==0xff && word[1]==0xff && word[2]==0xff && word[3]==0xff))
        {
            break;
        }
        //
        // if this is a bad header, skip it
        //
        CompressionHeaderUnpack(word, &code, &reference, &length, &major, &minor);
//        printf("found block at %x: code=%d ref=%d length=%d major=%d minor=%d\n",cptr,code,reference,length,major,minor);
        if(length>=1024)
        {
//            printf("skipping bad header\n");
            cptr-=CompressionHeaderLength;
            continue;
        }
        //
        // read all of the data
        //
        osize=length;                
        EepromRead(cptr,word,CompressionHeaderLength+osize+CompressionChecksumLength);
        //
        // compute and check the checksum;
        //
        checksum=Ar9300CompressionChecksum(&word[CompressionHeaderLength], length);
        mchecksum= word[CompressionHeaderLength+osize]|(word[CompressionHeaderLength+osize+1]<<8);
//        printf("checksum %x %x\n",checksum,mchecksum);
        if(checksum==mchecksum)
        {
            switch(code)
            {
                case _CompressNone:
                    if(length!=msize)
                    {
//                        printf("eeprom struct size mismatch memory=%d eeprom=%d\n",msize,length);
                        return -1;
                    }
                    //
                    // interpret the data
                    //
                    memcpy(mptr,(unsigned char *)(word+CompressionHeaderLength),length);
//                    printf("restored eeprom %d: uncompressed, length %d\n",it,length);
                    break;
#ifdef UNUSED
                case _CompressLzma:
                    //
                    // find the reference data
                    //
                    if(reference==ReferenceCurrent)
                    {
                        dptr=mptr;
                    }
                    else
                    {
                        dptr=(unsigned char *)Ar9300EepromStructDefault(reference);
                        if(dptr==0)
                        {
//                            printf("cant find reference eeprom struct %d\n",reference);
                            return -1;
                        }
                    }
                    //
                    // uncompress the data
                    //
                    usize= -1;
    //                usize=UnCompressLzma(word+overhead,length,output,MOUTPUT);
                    if(usize!=msize)
                    {
//                        printf("uncompressed data is wrong size %d %d\n",usize,msize);
                        return -1;
                    }
                    //
                    // interpret the data
                    //
                    for(ib=0; ib<msize; ib++)
                    {
                        mptr[ib]=dptr[ib]^word[ib+overhead];
                    }
//                    printf("restored eeprom %d: compressed, reference %d, length %d\n",it,reference,length);
                    break;
                case _CompressPairs:
                    //
                    // find the reference data
                    //
                    if(reference==ReferenceCurrent)
                    {
                        dptr=mptr;
                    }
                    else
                    {
                        dptr=(unsigned char *)Ar9300EepromStructDefault(reference);
                        if(dptr==0)
                        {
//                            printf("cant find reference eeprom struct %d\n",reference);
                            return -1;
                        }
                    }
                    //
                    // interpret the data
                    //
                    // NEED SOMETHING HERE
//                    printf("restored eeprom %d: pairs, reference %d, length %d, \n",it,reference,length);
                    break;
#endif
                case _CompressBlock:
                    //
                    // find the reference data
                    //
                    if(reference==ReferenceCurrent)
                    {
                        dptr=mptr;
                    }
                    else
                    {
                        dptr=(unsigned char *)Ar9300EepromStructDefault(reference);
                        if(dptr==0)
                        {
//                            printf("cant find reference eeprom struct %d\n",reference);
                            return -1;
                        }
                    }
                    //
                    // interpret the data
                    //
//                    printf("restore eeprom %d: block, reference %d, length %d\n",it,reference,length);
                    UncompressBlock(mptr,msize,(unsigned char *)(word+CompressionHeaderLength),length);
                    break;
                default:
//                    printf("unknown compression code %d\n",code);
                    return -1;
            }
        }
        else
        {
//            printf("skipping block with bad checksum\n");
        }
        cptr-=(CompressionHeaderLength+osize+CompressionChecksumLength);
    }

    return cptr;
}


//
// Restore the configuration structure by reading the eeprom.
// This function destroys any existing in-memory structure content.
//
int Ar9300EepromRestore()
{
    unsigned char *mptr;
    int msize;
    //
    // get a pointer to the data structure
    //
    mptr=(unsigned char *)Ar9300EepromStructInit(0);
    msize=Ar9300EepromStructSize();

    if(mptr!=0 && msize>0)
    {
        if(Ar9300EepromRestoreInternal(mptr,msize)>=0)
        {
//            Ar9300EepromDisplayAll();
            return 0;
        }
    }

    return -1;
}


