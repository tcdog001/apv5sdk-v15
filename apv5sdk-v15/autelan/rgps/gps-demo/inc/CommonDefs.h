#ifndef COMMONDEFS_H
#define COMMONDEFS_H
#include "comdef.h"
// --------------------------------------------------------------------------
//
// CommonDefs.h
//
/// Typedefs for primitive types used throughout WCEditor code
///
/// @file
//
// Copyright (c) 2007 - 2010 by QUALCOMM Incorporated. All Rights Reserved.
//
// All data and information contained in or disclosed by this document
// is confidential and proprietary information of QUALCOMM Incorporated
// and all rights therein are expressly reserved.  By accepting this
// material the recipient agrees that this material and the information
// contained therein is held in confidence and in trust and will not be
// used, copied, reproduced in whole or in part, nor its contents
// revealed in any manner to others without the express written
// permission of QUALCOMM Incorporated.
// --------------------------------------------------------------------------

//typedef signed char int8;
//typedef unsigned char uint8;
typedef unsigned char bool;

#define true 1
#define false 0

#define qmi_sync_timeout 90000

#if 0
typedef signed long int __int64;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef  __int64 int64;


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long int uint64;


typedef uint8 byte;
typedef uint16 word;
typedef uint32 dword;



#ifdef QWORD_IS_STRUCT
//For clients who prefer to think of qwords
// as a struct of 2 integers
typedef struct tagQword
{
   dword lo;
   dword hi;
} qword;

#else

//For clients who prefer to think of qwords
// as a 64-bit integer
typedef uint64 qword;

#endif

#define IPv6_ADDRESS_LENGTH    16

typedef struct IPV6ADDR
{
public: uint16 ipv6_address[8];
	IPV6ADDR(){}
	IPV6ADDR(int val){
		memset(ipv6_address,val,IPv6_ADDRESS_LENGTH);
	}
} IPv6Addr;

typedef qword time_type;
typedef unsigned char boolean;

// if condition true, assert in debug mode, return given value in release
// in debug mode, if assert is ignored, given value is returned
#define ASSERT_AND_RETURN_IF(c,r) { if (c) { assert(FALSE); return r; } }

// --------------------------------------------------------------------------
// DeleteObjectFunctor - a function object used for deleting a container of
// newed up pointers (e.g. via std::for_each).
// --------------------------------------------------------------------------
struct DeleteObjectFunctor
{
	template<typename T>
	void operator()(const T* ptr) const
	{
		delete ptr;
	}
};

// --------------------------------------------------------------------------
// define transmit/receive types !!! move to tlv.h at hierarchy change
// --------------------------------------------------------------------------
enum TxRxType
{
    TX_TYPE = 0,
    RX_TYPE = 1
};
#endif
#endif
