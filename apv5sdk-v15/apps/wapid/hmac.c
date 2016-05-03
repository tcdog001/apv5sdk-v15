
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmac.h"

   /**************************** sha.h ****************************/
   /*
    *  Description:
    *      This file implements the Secure Hash Signature Standard
    *      algorithms as defined in the National Institute of Standards
    *      and Technology Federal Information Processing Standards
    *      Publication (FIPS PUB) 180-1 published on April 17, 1995, 180-2
    *      published on August 1, 2002, and the FIPS PUB 180-2 Change
    *      Notice published on February 28, 2004.
    *
    *      A combined document showing all algorithms is available at
    *              http://csrc.nist.gov/publications/fips/
    *              fips180-2/fips180-2withchangenotice.pdf
    *
    *      The five hashes are defined in these sizes:
    *              SHA-1           20 byte / 160 bit
    *              SHA-224         28 byte / 224 bit
    *              SHA-256         32 byte / 256 bit
    *              SHA-384         48 byte / 384 bit
    *              SHA-512         64 byte / 512 bit
    */


typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned int int_least16_t;

/*All SHA functions return one of these values.*/
enum
{
	shaSuccess = 0,
	shaNull=1,/*Null pointer parameter*/
	shaStateError=3,/*called Input after FinalBits or Result*/
};

/*These constants hold size information for each of the SHA hashing operations*/
enum
{
	SHA256_Message_Block_Size = 64,		
	SHA256HashSize = 32,
	USHA_Max_Message_Block_Size = 128,	
	USHAMaxHashSize = 64,
};

/*This structure will hold context information for the SHA-256 hashing operation.*/
typedef struct SHA256Context
{
	uint32_t Intermediate_Hash[SHA256HashSize/4]; /*Message Digest*/
	uint32_t Length_High;/*Message length in bits*/ /*must in front of Length_Low, and neighboring*/
	uint32_t Length_Low;/*Message length in bits*/
	int_least16_t Message_Block_Index;/*Message_Block array index*/ /*512-bit message blocks*/
	uint8_t Message_Block[SHA256_Message_Block_Size];
	int Computed;/*Is the digest computed?*/
	int Corrupted;/*Is the digest corrupted?*/
} SHA256Context;

/*This structure will hold context information for the HMAC keyed hashing operation.*/
typedef struct HMACContext
{
	int hashSize;/*hash size of SHA being used*/
	int blockSize;/*block size of SHA being used*/
	SHA256Context shaContext;/*SHA context*/
	unsigned char k_opad[USHA_Max_Message_Block_Size];/*outer padding - key XORd with opad*/
} HMACContext;





   /*************************** sha224-256.c ***************************/
   /*
    * Description:
    *   This file implements the Secure Hash Signature Standard
    *   algorithms as defined in the National Institute of Standards
    *   and Technology Federal Information Processing Standards
    *   Publication (FIPS PUB) 180-1 published on April 17, 1995, 180-2
    *   published on August 1, 2002, and the FIPS PUB 180-2 Change
    *   Notice published on February 28, 2004.
    *
    *   A combined document showing all algorithms is available at
    *       http://csrc.nist.gov/publications/fips/
    *       fips180-2/fips180-2withchangenotice.pdf
    *
    *   The SHA-224 and SHA-256 algorithms produce 224-bit and 256-bit
    *   message digests for a given data stream. It should take about
    *   2**n steps to find a message with the same digest as a given
    *   message and 2**(n/2) to find any two messages with the same
    *   digest, when n is the digest size in bits. Therefore, this
    *   algorithm can serve as a means of providing a
    *   "fingerprint" for a message.
    *
    * Portability Issues:
    *   SHA-224 and SHA-256 are defined in terms of 32-bit "words".
    *   This code uses <stdint.h> (included via "sha.h") to define 32
    *   and 8 bit unsigned integer types. If your C compiler does not
    *   support 32 bit unsigned integers, this code is not
    *   appropriate.
    *
    * Caveats:
    *   SHA-224 and SHA-256 are designed to work with messages less
    *   than 2^64 bits long. This implementation uses SHA224/256Input()
    *   to hash the bits that are a multiple of the size of an 8-bit
    *   character, and then uses SHA224/256FinalBits() to hash the
    *   final few bits of the input.
    */
#define SHA_Ch(x,y,z)	(((x) & (y)) ^ ((~(x)) & (z)))
#define SHA_Maj(x,y,z)	(((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

/*Define the SHA shift, rotate left and rotate right macro*/
#define SHA256_SHR(bits,word)	((word) >> (bits))
#define SHA256_ROTL(bits,word)	(((word) << (bits)) | ((word) >> (32-(bits))))
#define SHA256_ROTR(bits,word)	(((word) >> (bits)) | ((word) << (32-(bits))))

/*Define the SHA SIGMA and sigma macros*/
#define SHA256_SIGMA0(word)		(SHA256_ROTR( 2,word) ^ SHA256_ROTR(13,word) ^ SHA256_ROTR(22,word))
#define SHA256_SIGMA1(word)		(SHA256_ROTR( 6,word) ^ SHA256_ROTR(11,word) ^ SHA256_ROTR(25,word))
#define SHA256_sigma0(word)		(SHA256_ROTR( 7,word) ^ SHA256_ROTR(18,word) ^ SHA256_SHR( 3,word))
#define SHA256_sigma1(word)		(SHA256_ROTR(17,word) ^ SHA256_ROTR(19,word) ^ SHA256_SHR(10,word))

/*add "length" to the length*/
static uint32_t addTemp;
#define SHA224_256AddLength(context, length)				\
	(addTemp = (context)->Length_Low, (context)->Corrupted =	\
	(((context)->Length_Low += (length)) < addTemp) &&			\
	(++(context)->Length_High == 0) ? 1 : 0)

   /*
    * SHA224_256ProcessMessageBlock
    *
    * Description:
    *   This function will process the next 512 bits of the message
    *   stored in the Message_Block array.
    *
    * Parameters:
    *   context: [in/out]
    *     The SHA context to update
    *
    * Returns:
    *   Nothing.
    *
    * Comments:
    *   Many of the variable names in this code, especially the
    *   single character names, were used because those were the
    *   names used in the publication.
    */
static void SHA256ProcessMessageBlock(SHA256Context *context)
{
	/*Constants defined in FIPS-180-2, section 4.2.2*/
	static const uint32_t K[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 
		0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
		0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 
		0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
		0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 
		0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
		0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 
		0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 
		0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
		0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 
		0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	
	 int        t, t4;                   /* Loop counter */
	 uint32_t   temp1, temp2;            /* Temporary word value */
	 uint32_t   W[64];                   /* Word sequence */
	 uint32_t   A, B, C, D, E, F, G, H;  /* Word buffers */



	/*Initialize the first 16 words in the array W*/
	for (t = t4 = 0; t < 16; t++, t4 += 4){
		W[t] = (context->Message_Block[t4] << 24) |
			(context->Message_Block[t4 + 1] << 16) |
			(context->Message_Block[t4 + 2] << 8) |
			(context->Message_Block[t4 + 3]);
	}

	for (t = 16; t < 64; t++){
		W[t] = SHA256_sigma1(W[t-2]) + W[t-7] + SHA256_sigma0(W[t-15]) + W[t-16];
	}
	 A = context->Intermediate_Hash[0];
	 B = context->Intermediate_Hash[1];
	 C = context->Intermediate_Hash[2];
	 D = context->Intermediate_Hash[3];
	 E = context->Intermediate_Hash[4];
	 F = context->Intermediate_Hash[5];
	 G = context->Intermediate_Hash[6];
	 H = context->Intermediate_Hash[7];
	for (t = 0; t < 64; t++) {
		temp1 = H + SHA256_SIGMA1(E) + SHA_Ch(E,F,G) + K[t] + W[t];
		temp2 = SHA256_SIGMA0(A) + SHA_Maj(A,B,C);
		H = G;
		G = F;
		F = E;
		E = D + temp1;
		D = C;
		C = B;
		B = A;
		A = temp1 + temp2;
	}
	 
	 context->Intermediate_Hash[0] += A;
	 context->Intermediate_Hash[1] += B;
	 context->Intermediate_Hash[2] += C;
	 context->Intermediate_Hash[3] += D;
	 context->Intermediate_Hash[4] += E;
	 context->Intermediate_Hash[5] += F;
	 context->Intermediate_Hash[6] += G;
	 context->Intermediate_Hash[7] += H;

	 context->Message_Block_Index = 0;
 }


/*
    * SHA224_256Reset
    *
    * Description:
    *   This helper function will initialize the SHA256Context in
    *   preparation for computing a new SHA256 message digest.
    *
    * Parameters:
    *   context: [in/out]
    *     The context to reset.
    *   H0
    *     The initial hash value to use.
    *
    * Returns:
    *   sha Error Code.
    */
static int SHA256Reset(SHA256Context *context)
{
	static uint32_t SHA256_H0[SHA256HashSize/4] = {
		0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 
		0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
	};
	 if (!context)
	   return shaNull;

	 context->Length_Low           = 0;
	 context->Length_High          = 0;
	 context->Message_Block_Index  = 0;

	 context->Intermediate_Hash[0] = SHA256_H0[0];
	 context->Intermediate_Hash[1] = SHA256_H0[1];
	 context->Intermediate_Hash[2] = SHA256_H0[2];
	 context->Intermediate_Hash[3] = SHA256_H0[3];
	 context->Intermediate_Hash[4] = SHA256_H0[4];
	 context->Intermediate_Hash[5] = SHA256_H0[5];
	 context->Intermediate_Hash[6] = SHA256_H0[6];
	 context->Intermediate_Hash[7] = SHA256_H0[7];

	 context->Computed  = 0;
	 context->Corrupted = 0;

	 return shaSuccess;
}

 /*
    * SHA256Input
    *
    * Description:
    *   This function accepts an array of octets as the next portion
    *   of the message.
    *
    * Parameters:
    *   context: [in/out]
    *     The SHA context to update
    *   message_array: [in]
    *     An array of characters representing the next portion of
    *     the message.
    *   length: [in]
    *     The length of the message in message_array
    *
    * Returns:
    *   sha Error Code.
    */
static int SHA256Input(SHA256Context *context, const uint8_t *message_array, unsigned int length)
{
	if (!length)	return shaSuccess;
	if (!context || !message_array)		return shaNull;
	if (context->Computed)
	{
		context->Corrupted = shaStateError;
		return shaStateError;
	}
	if (context->Corrupted)		return context->Corrupted;
	while (length-- && !context->Corrupted)
	{
		context->Message_Block[context->Message_Block_Index++] = (*message_array & 0xFF);
		if (!SHA224_256AddLength(context, 8) && (context->Message_Block_Index == SHA256_Message_Block_Size))
			SHA256ProcessMessageBlock(context);
		message_array++;
	}
	return shaSuccess;
}

   /*
    * SHA256Reset
    *
    * Description:
    *   This function will initialize the SHA256Context in preparation
    *   for computing a new SHA256 message digest.
    *
    * Parameters:
    *   context: [in/out]
    *   The context to reset.
    *
    * Returns:
    *   sha Error Code.
    */
static int SHA256Result(SHA256Context *context, uint8_t Message_Digest[])
{
	int i;
	int HashSize = SHA256HashSize;

	if (!context || !Message_Digest)	return shaNull;

	if (context->Corrupted)		return context->Corrupted;

	if (!context->Computed)
	{/*SHA224_256Finalize(context, 0x80);*/
		uint8_t Pad_Byte = 0x80;
		{/*SHA224_256PadMessage(context, Pad_Byte);*/
			/*Check to see if the current message block is too small to hold the initial padding bits and length. If so, we will pad the block, process it, and then continue padding into a second block.*/
			if (context->Message_Block_Index >= (SHA256_Message_Block_Size-8))
			{
				context->Message_Block[context->Message_Block_Index++] = Pad_Byte;
				while (context->Message_Block_Index < SHA256_Message_Block_Size)
					context->Message_Block[context->Message_Block_Index++] = 0;
				SHA256ProcessMessageBlock(context);
			}
			else
				context->Message_Block[context->Message_Block_Index++] = Pad_Byte;

			while (context->Message_Block_Index < (SHA256_Message_Block_Size-8))
				context->Message_Block[context->Message_Block_Index++] = 0;

			/*Store the message length as the last 8 octets*/
			//memcpy_int32_reverse(context->Message_Block+56, &context->Length_High, 2);
			context->Message_Block[56] = (uint8_t)(context->Length_High >> 24);
			context->Message_Block[57] = (uint8_t)(context->Length_High >> 16);
			context->Message_Block[58] = (uint8_t)(context->Length_High >> 8);
			context->Message_Block[59] = (uint8_t)(context->Length_High);
			context->Message_Block[60] = (uint8_t)(context->Length_Low >> 24);
			context->Message_Block[61] = (uint8_t)(context->Length_Low >> 16);
			context->Message_Block[62] = (uint8_t)(context->Length_Low >> 8);
			context->Message_Block[63] = (uint8_t)(context->Length_Low);

			SHA256ProcessMessageBlock(context);
		}

		/*message may be sensitive, so clear it out*/
		memset(context->Message_Block, 0, SHA256_Message_Block_Size);
		context->Length_Low = context->Length_High = 0;/*and clear length*/
		context->Computed = 1;
	}

	for (i = 0; i < HashSize; ++i)
		Message_Digest[i] = (uint8_t)(context->Intermediate_Hash[i>>2] >> 8 * ( 3 - ( i & 0x03 ) ));

	return shaSuccess;
}





  /*
    *  hmacReset
    *
    *  Description:
    *      This function will initialize the hmacContext in preparation
    *      for computing a new HMAC message digest.
    *
    *  Parameters:
    *      context: [in/out]
    *          The context to reset.
    *      whichSha: [in]
    *          One of SHA1, SHA224, SHA256, SHA384, SHA512
    *      key: [in]
    *          The secret shared key.
    *      key_len: [in]
    *          The length of the secret shared key.
    *
    *  Returns:
    *      sha Error Code.
    *
    */static int hmacReset(HMACContext* ctx, const unsigned char *key, int key_len)
{
	int i, blocksize, hashsize;
	unsigned char k_ipad[USHA_Max_Message_Block_Size];/*inner padding - key XORd with ipad*/
	unsigned char tempkey[USHAMaxHashSize];/*temporary buffer when keylen > blocksize*/
	if (!ctx) return shaNull;

	blocksize = ctx->blockSize = SHA256_Message_Block_Size;
	hashsize = ctx->hashSize = SHA256HashSize;
	if (key_len > blocksize)/*If key is longer than the hash blocksize, reset it to key = HASH(key).*/
	{
		SHA256Context tctx;
		int err = SHA256Reset(&tctx) || SHA256Input(&tctx, key, key_len) || SHA256Result(&tctx, tempkey);
		if (err != shaSuccess) return err;

		key = tempkey;
		key_len = hashsize;
	}
	/*The HMAC transform looks like:SHA(K XOR opad, SHA(K XOR ipad, text)),	where K is an n byte key. ipad is the byte 0x36 repeated blocksize times opad is the byte 0x5c repeated blocksize times and text is the data being protected.*/
	for (i = 0; i < key_len; i++)
	{/*store key into the pads, XOR'd with ipad and opad values*/
		k_ipad[i] = key[i] ^ 0x36;
		ctx->k_opad[i] = key[i] ^ 0x5c;
	}
	for ( ; i < blocksize; i++)
	{/*remaining pad bytes are '\0' XOR'd with ipad and opad values*/
		k_ipad[i] = 0x36;
		ctx->k_opad[i] = 0x5c;
	}
	/*perform inner hash*/ /*init context for 1st pass*/
	return SHA256Reset(&ctx->shaContext) || SHA256Input(&ctx->shaContext, k_ipad, blocksize);/*and start with inner pad*/
}

   /*
    * HMACResult
    *
    * Description:
    *   This function will return the N-byte message digest into the
    *   Message_Digest array provided by the caller.
    *   NOTE: The first octet of hash is stored in the 0th element,
    *      the last octet of hash in the Nth element.
    *
    * Parameters:
    *   context: [in/out]
    *     The context to use to calculate the HMAC hash.
    *   digest: [out]
    *     Where the digest is returned.
    *   NOTE 2: The length of the hash is determined by the value of
    *      whichSha that was passed to hmacReset().
    *
    * Returns:
    *   sha Error Code.
    *
    */
 static int hmacResult(HMACContext *ctx, uint8_t *digest)
{
	if (!ctx) return shaNull;
	/*finish up 1st pass*/ /*(Use digest here as a temporary buffer.)*/
	return SHA256Result(&ctx->shaContext, digest) || SHA256Reset(&ctx->shaContext)/*perform outer SHA*/ /*init context for 2nd pass*/
		|| SHA256Input(&ctx->shaContext, ctx->k_opad, ctx->blockSize)/*start with outer pad*/
		|| SHA256Input(&ctx->shaContext, digest, ctx->hashSize)/*then results of 1st hash*/
		|| SHA256Result(&ctx->shaContext, digest);/*finish up 2nd pass*/
}

 /*
    *  hmac
    *
    *  Description:
    *      This function will compute an HMAC message digest.
    *
    *  Parameters:
    *      whichSha: [in]
    *          One of SHA1, SHA224, SHA256, SHA384, SHA512
    *      key: [in]
    *          The secret shared key.
    *      key_len: [in]
    *          The length of the secret shared key.
    *      message_array: [in]
    *          An array of characters representing the message.
    *      length: [in]
    *          The length of the message in message_array
    *      digest: [out]
    *          Where the digest is returned.
    *          NOTE: The length of the digest is determined by
    *              the value of whichSha.
    *
    *  Returns:
    *      sha Error Code.
    *
    */
static int hmac(const unsigned char* text, int text_len, const unsigned char *key, int key_len, uint8_t digest[USHAMaxHashSize])
{
	HMACContext ctx;
	return hmacReset(&ctx, key, key_len) || SHA256Input(&ctx.shaContext, text, text_len) || hmacResult(&ctx, digest);
}





/*define of usefully function*/
int mhash_sha256(const unsigned char* data, unsigned length, unsigned char* digest)
{
	SHA256Context ctx;
	SHA256Reset(&ctx);
	SHA256Input(&ctx, data, length);
	SHA256Result(&ctx, digest);
	return SHA256HashSize;
}
int hmac_sha256(const unsigned char* text, int text_len, 
		const unsigned char* key, unsigned key_len, 
		unsigned char* digest, unsigned digest_length)
{
	unsigned char tmp[USHAMaxHashSize] = {0};
	int ret = hmac(text,text_len, key,key_len, tmp);
	if (0 == ret)	memcpy(digest, tmp, digest_length);
	return ret;
}

void KD_hmac_sha256(const unsigned char* text, unsigned text_len, 
					const unsigned char* key, unsigned key_len, unsigned char* out, unsigned out_len)
{
	unsigned i;
	unsigned length = out_len;
	unsigned SHA256_DIGEST_SIZE = SHA256HashSize;
	for (i=0; length/SHA256_DIGEST_SIZE; i++,length-=SHA256_DIGEST_SIZE)
	{
		hmac_sha256(text, text_len, key, key_len, &out[i*SHA256_DIGEST_SIZE], SHA256_DIGEST_SIZE);
		text = &out[i*SHA256_DIGEST_SIZE];
		text_len= SHA256_DIGEST_SIZE;
	}
	if(length>0){	
		hmac_sha256(text, text_len, key, key_len, &out[i*SHA256_DIGEST_SIZE], length);
	}
}

#if 0
int sha256_test(void)
{
	int rv = 0;
	u8 vector1[]={00, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 
			0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
	u8 degist[128]={0,};

	mhash_sha256(vector1, sizeof(vector1), degist);
	wapi_hex_dump("SHA256 out", degist, sizeof(degist));
	return 0;
}


int hmac_sha256_test(void)
{
	int rv = 0;
	u8 degist[128]={0,};
	char *v_text1="abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	u8 v_key[]={0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
			0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 
			0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 
			0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};
	
	rv = hmac_sha256(v_text1, strlen(v_text1), v_key, sizeof(v_key), degist, 32);
	wapi_hex_dump("HMAC_SHA256 out", degist, sizeof(degist));
	return rv;
}
int kd_hmac_sha256_test(int outl)
{
	int rv = 0;
	u8 degist[128]={0,};
	char passwd[]="11111111";
#if 0	
	char *v_text1="pairwise key expansion for infrastructure unicast";
	u8 v_key[]={0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
			0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 
			0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 
			0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};
#endif
	if(outl >128)
		outl = 128;
	KD_hmac_sha256((u8*)PSK_TEXT, strlen(PSK_TEXT), 
					passwd, strlen(passwd), degist, outl);
	//KD_hmac_sha256(v_text1, strlen(v_text1), v_key, sizeof(v_key),degist, outl);
	wapi_hex_dump("KD_HMAC_SHA256 out", degist, sizeof(degist));

	return 0;
}

int main_test(int argc, char *argv[])
{
	//sha256_test();
	//hmac_sha256_test();
	kd_hmac_sha256_test(atoi(argv[1]));
	return 0;
}
#endif


