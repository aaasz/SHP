/*********************************************************************
* Copyright (c) 2004-2005, Impulse Accelerated Technologies, Inc.
* All Rights Reserved.
*
* co_math.h: Math functions.
*
*********************************************************************/

#ifndef CO_MATH_H
#define CO_MATH_H

#include "co_types.h"

#ifndef __uClinux__
#define abs(n) ((n) < 0 ? -(n) : (n))
#endif

typedef int8 co_int1;
typedef int8 co_int2;
typedef int8 co_int3;
typedef int8 co_int4;
typedef int8 co_int5;
typedef int8 co_int6;
typedef int8 co_int7;
typedef int8 co_int8;
typedef int16 co_int9;
typedef int16 co_int10;
typedef int16 co_int11;
typedef int16 co_int12;
typedef int16 co_int13;
typedef int16 co_int14;
typedef int16 co_int15;
typedef int16 co_int16;
typedef int32 co_int17;
typedef int32 co_int18;
typedef int32 co_int19;
typedef int32 co_int20;
typedef int32 co_int21;
typedef int32 co_int22;
typedef int32 co_int23;
typedef int32 co_int24;
typedef int32 co_int25;
typedef int32 co_int26;
typedef int32 co_int27;
typedef int32 co_int28;
typedef int32 co_int29;
typedef int32 co_int30;
typedef int32 co_int31;
typedef int32 co_int32;
typedef int32 co_int64[2];
typedef int32 co_int128[4];
typedef int32 co_int256[8];

typedef uint8 co_uint1;
typedef uint8 co_uint2;
typedef uint8 co_uint3;
typedef uint8 co_uint4;
typedef uint8 co_uint5;
typedef uint8 co_uint6;
typedef uint8 co_uint7;
typedef uint8 co_uint8;
typedef uint16 co_uint9;
typedef uint16 co_uint10;
typedef uint16 co_uint11;
typedef uint16 co_uint12;
typedef uint16 co_uint13;
typedef uint16 co_uint14;
typedef uint16 co_uint15;
typedef uint16 co_uint16;
typedef uint32 co_uint17;
typedef uint32 co_uint18;
typedef uint32 co_uint19;
typedef uint32 co_uint20;
typedef uint32 co_uint21;
typedef uint32 co_uint22;
typedef uint32 co_uint23;
typedef uint32 co_uint24;
typedef uint32 co_uint25;
typedef uint32 co_uint26;
typedef uint32 co_uint27;
typedef uint32 co_uint28;
typedef uint32 co_uint29;
typedef uint32 co_uint30;
typedef uint32 co_uint31;
typedef uint32 co_uint32;
typedef uint32 co_uint64[2];
typedef uint32 co_uint128[4];
typedef uint32 co_uint256[8];

#define IADD1(a,b) ((((int32)(a)+(int32)(b))<<31)>>31)
#define IADD2(a,b) ((((int32)(a)+(int32)(b))<<30)>>30)
#define IADD3(a,b) ((((int32)(a)+(int32)(b))<<29)>>29)
#define IADD4(a,b) ((((int32)(a)+(int32)(b))<<28)>>28)
#define IADD5(a,b) ((((int32)(a)+(int32)(b))<<27)>>27)
#define IADD6(a,b) ((((int32)(a)+(int32)(b))<<26)>>26)
#define IADD7(a,b) ((((int32)(a)+(int32)(b))<<25)>>25)
#define IADD8(a,b) ((((int32)(a)+(int32)(b))<<24)>>24)
#define IADD9(a,b) ((((int32)(a)+(int32)(b))<<23)>>23)
#define IADD10(a,b) ((((int32)(a)+(int32)(b))<<22)>>22)
#define IADD11(a,b) ((((int32)(a)+(int32)(b))<<21)>>21)
#define IADD12(a,b) ((((int32)(a)+(int32)(b))<<20)>>20)
#define IADD13(a,b) ((((int32)(a)+(int32)(b))<<19)>>19)
#define IADD14(a,b) ((((int32)(a)+(int32)(b))<<18)>>18)
#define IADD15(a,b) ((((int32)(a)+(int32)(b))<<17)>>17)
#define IADD16(a,b) ((((int32)(a)+(int32)(b))<<16)>>16)
#define IADD17(a,b) ((((int32)(a)+(int32)(b))<<15)>>15)
#define IADD18(a,b) ((((int32)(a)+(int32)(b))<<14)>>14)
#define IADD19(a,b) ((((int32)(a)+(int32)(b))<<13)>>13)
#define IADD20(a,b) ((((int32)(a)+(int32)(b))<<12)>>12)
#define IADD21(a,b) ((((int32)(a)+(int32)(b))<<11)>>11)
#define IADD22(a,b) ((((int32)(a)+(int32)(b))<<10)>>10)
#define IADD23(a,b) ((((int32)(a)+(int32)(b))<<9)>>9)
#define IADD24(a,b) ((((int32)(a)+(int32)(b))<<8)>>8)
#define IADD25(a,b) ((((int32)(a)+(int32)(b))<<7)>>7)
#define IADD26(a,b) ((((int32)(a)+(int32)(b))<<6)>>6)
#define IADD27(a,b) ((((int32)(a)+(int32)(b))<<5)>>5)
#define IADD28(a,b) ((((int32)(a)+(int32)(b))<<4)>>4)
#define IADD29(a,b) ((((int32)(a)+(int32)(b))<<3)>>3)
#define IADD30(a,b) ((((int32)(a)+(int32)(b))<<2)>>2)
#define IADD31(a,b) ((((int32)(a)+(int32)(b))<<1)>>1)
#define IADD32(a,b) ((int32)(a)+(int32)(b))

#define UADD1(a,b) (((uint8)(a)+(uint8)(b))&0x1)
#define UADD2(a,b) (((uint8)(a)+(uint8)(b))&0x3)
#define UADD3(a,b) (((uint8)(a)+(uint8)(b))&0x7)
#define UADD4(a,b) (((uint8)(a)+(uint8)(b))&0xf)
#define UADD5(a,b) (((uint8)(a)+(uint8)(b))&0x1f)
#define UADD6(a,b) (((uint8)(a)+(uint8)(b))&0x3f)
#define UADD7(a,b) (((uint8)(a)+(uint8)(b))&0x7f)
#define UADD8(a,b) ((uint8)(a)+(uint8)(b))
#define UADD9(a,b) (((uint16)(a)+(uint16)(b))&0x1ff)
#define UADD10(a,b) (((uint16)(a)+(uint16)(b))&0x3ff)
#define UADD11(a,b) (((uint16)(a)+(uint16)(b))&0x7ff)
#define UADD12(a,b) (((uint16)(a)+(uint16)(b))&0xfff)
#define UADD13(a,b) (((uint16)(a)+(uint16)(b))&0x1fff)
#define UADD14(a,b) (((uint16)(a)+(uint16)(b))&0x3fff)
#define UADD15(a,b) (((uint16)(a)+(uint16)(b))&0x7fff)
#define UADD16(a,b) ((uint16)(a)+(uint16)(b))
#define UADD17(a,b) (((uint32)(a)+(uint32)(b))&0x1ffff)
#define UADD18(a,b) (((uint32)(a)+(uint32)(b))&0x3ffff)
#define UADD19(a,b) (((uint32)(a)+(uint32)(b))&0x7ffff)
#define UADD20(a,b) (((uint32)(a)+(uint32)(b))&0xfffff)
#define UADD21(a,b) (((uint32)(a)+(uint32)(b))&0x1fffff)
#define UADD22(a,b) (((uint32)(a)+(uint32)(b))&0x3fffff)
#define UADD23(a,b) (((uint32)(a)+(uint32)(b))&0x7fffff)
#define UADD24(a,b) (((uint32)(a)+(uint32)(b))&0xffffff)
#define UADD25(a,b) (((uint32)(a)+(uint32)(b))&0x1ffffff)
#define UADD26(a,b) (((uint32)(a)+(uint32)(b))&0x3ffffff)
#define UADD27(a,b) (((uint32)(a)+(uint32)(b))&0x7ffffff)
#define UADD28(a,b) (((uint32)(a)+(uint32)(b))&0xfffffff)
#define UADD29(a,b) (((uint32)(a)+(uint32)(b))&0x1fffffff)
#define UADD30(a,b) (((uint32)(a)+(uint32)(b))&0x3fffffff)
#define UADD31(a,b) (((uint32)(a)+(uint32)(b))&0x7fffffff)
#define UADD32(a,b) ((uint32)(a)+(uint32)(b))

#define ISUB1(a,b) ((((int32)(a)-(int32)(b))<<31)>>31)
#define ISUB2(a,b) ((((int32)(a)-(int32)(b))<<30)>>30)
#define ISUB3(a,b) ((((int32)(a)-(int32)(b))<<29)>>29)
#define ISUB4(a,b) ((((int32)(a)-(int32)(b))<<28)>>28)
#define ISUB5(a,b) ((((int32)(a)-(int32)(b))<<27)>>27)
#define ISUB6(a,b) ((((int32)(a)-(int32)(b))<<26)>>26)
#define ISUB7(a,b) ((((int32)(a)-(int32)(b))<<25)>>25)
#define ISUB8(a,b) ((((int32)(a)-(int32)(b))<<24)>>24)
#define ISUB9(a,b) ((((int32)(a)-(int32)(b))<<23)>>23)
#define ISUB10(a,b) ((((int32)(a)-(int32)(b))<<22)>>22)
#define ISUB11(a,b) ((((int32)(a)-(int32)(b))<<21)>>21)
#define ISUB12(a,b) ((((int32)(a)-(int32)(b))<<20)>>20)
#define ISUB13(a,b) ((((int32)(a)-(int32)(b))<<19)>>19)
#define ISUB14(a,b) ((((int32)(a)-(int32)(b))<<18)>>18)
#define ISUB15(a,b) ((((int32)(a)-(int32)(b))<<17)>>17)
#define ISUB16(a,b) ((((int32)(a)-(int32)(b))<<16)>>16)
#define ISUB17(a,b) ((((int32)(a)-(int32)(b))<<15)>>15)
#define ISUB18(a,b) ((((int32)(a)-(int32)(b))<<14)>>14)
#define ISUB19(a,b) ((((int32)(a)-(int32)(b))<<13)>>13)
#define ISUB20(a,b) ((((int32)(a)-(int32)(b))<<12)>>12)
#define ISUB21(a,b) ((((int32)(a)-(int32)(b))<<11)>>11)
#define ISUB22(a,b) ((((int32)(a)-(int32)(b))<<10)>>10)
#define ISUB23(a,b) ((((int32)(a)-(int32)(b))<<9)>>9)
#define ISUB24(a,b) ((((int32)(a)-(int32)(b))<<8)>>8)
#define ISUB25(a,b) ((((int32)(a)-(int32)(b))<<7)>>7)
#define ISUB26(a,b) ((((int32)(a)-(int32)(b))<<6)>>6)
#define ISUB27(a,b) ((((int32)(a)-(int32)(b))<<5)>>5)
#define ISUB28(a,b) ((((int32)(a)-(int32)(b))<<4)>>4)
#define ISUB29(a,b) ((((int32)(a)-(int32)(b))<<3)>>3)
#define ISUB30(a,b) ((((int32)(a)-(int32)(b))<<2)>>2)
#define ISUB31(a,b) ((((int32)(a)-(int32)(b))<<1)>>1)
#define ISUB32(a,b) ((int32)(a)-(int32)(b))

#define USUB1(a,b) (((uint8)(a)-(uint8)(b))&0x1)
#define USUB2(a,b) (((uint8)(a)-(uint8)(b))&0x3)
#define USUB3(a,b) (((uint8)(a)-(uint8)(b))&0x7)
#define USUB4(a,b) (((uint8)(a)-(uint8)(b))&0xf)
#define USUB5(a,b) (((uint8)(a)-(uint8)(b))&0x1f)
#define USUB6(a,b) (((uint8)(a)-(uint8)(b))&0x3f)
#define USUB7(a,b) (((uint8)(a)-(uint8)(b))&0x7f)
#define USUB8(a,b) ((uint8)(a)-(uint8)(b))
#define USUB9(a,b) (((uint16)(a)-(uint16)(b))&0x1ff)
#define USUB10(a,b) (((uint16)(a)-(uint16)(b))&0x3ff)
#define USUB11(a,b) (((uint16)(a)-(uint16)(b))&0x7ff)
#define USUB12(a,b) (((uint16)(a)-(uint16)(b))&0xfff)
#define USUB13(a,b) (((uint16)(a)-(uint16)(b))&0x1fff)
#define USUB14(a,b) (((uint16)(a)-(uint16)(b))&0x3fff)
#define USUB15(a,b) (((uint16)(a)-(uint16)(b))&0x7fff)
#define USUB16(a,b) ((uint16)(a)-(uint16)(b))
#define USUB17(a,b) (((uint32)(a)-(uint32)(b))&0x1ffff)
#define USUB18(a,b) (((uint32)(a)-(uint32)(b))&0x3ffff)
#define USUB19(a,b) (((uint32)(a)-(uint32)(b))&0x7ffff)
#define USUB20(a,b) (((uint32)(a)-(uint32)(b))&0xfffff)
#define USUB21(a,b) (((uint32)(a)-(uint32)(b))&0x1fffff)
#define USUB22(a,b) (((uint32)(a)-(uint32)(b))&0x3fffff)
#define USUB23(a,b) (((uint32)(a)-(uint32)(b))&0x7fffff)
#define USUB24(a,b) (((uint32)(a)-(uint32)(b))&0xffffff)
#define USUB25(a,b) (((uint32)(a)-(uint32)(b))&0x1ffffff)
#define USUB26(a,b) (((uint32)(a)-(uint32)(b))&0x3ffffff)
#define USUB27(a,b) (((uint32)(a)-(uint32)(b))&0x7ffffff)
#define USUB28(a,b) (((uint32)(a)-(uint32)(b))&0xfffffff)
#define USUB29(a,b) (((uint32)(a)-(uint32)(b))&0x1fffffff)
#define USUB30(a,b) (((uint32)(a)-(uint32)(b))&0x3fffffff)
#define USUB31(a,b) (((uint32)(a)-(uint32)(b))&0x7fffffff)
#define USUB32(a,b) ((uint32)(a)-(uint32)(b))

#define IMUL1(a,b) ((((int32)(a)*(int32)(b))<<31)>>31)
#define IMUL2(a,b) ((((int32)(a)*(int32)(b))<<30)>>30)
#define IMUL3(a,b) ((((int32)(a)*(int32)(b))<<29)>>29)
#define IMUL4(a,b) ((((int32)(a)*(int32)(b))<<28)>>28)
#define IMUL5(a,b) ((((int32)(a)*(int32)(b))<<27)>>27)
#define IMUL6(a,b) ((((int32)(a)*(int32)(b))<<26)>>26)
#define IMUL7(a,b) ((((int32)(a)*(int32)(b))<<25)>>25)
#define IMUL8(a,b) ((((int32)(a)*(int32)(b))<<24)>>24)
#define IMUL9(a,b) ((((int32)(a)*(int32)(b))<<23)>>23)
#define IMUL10(a,b) ((((int32)(a)*(int32)(b))<<22)>>22)
#define IMUL11(a,b) ((((int32)(a)*(int32)(b))<<21)>>21)
#define IMUL12(a,b) ((((int32)(a)*(int32)(b))<<20)>>20)
#define IMUL13(a,b) ((((int32)(a)*(int32)(b))<<19)>>19)
#define IMUL14(a,b) ((((int32)(a)*(int32)(b))<<18)>>18)
#define IMUL15(a,b) ((((int32)(a)*(int32)(b))<<17)>>17)
#define IMUL16(a,b) ((((int32)(a)*(int32)(b))<<16)>>16)
#define IMUL17(a,b) ((((int32)(a)*(int32)(b))<<15)>>15)
#define IMUL18(a,b) ((((int32)(a)*(int32)(b))<<14)>>14)
#define IMUL19(a,b) ((((int32)(a)*(int32)(b))<<13)>>13)
#define IMUL20(a,b) ((((int32)(a)*(int32)(b))<<12)>>12)
#define IMUL21(a,b) ((((int32)(a)*(int32)(b))<<11)>>11)
#define IMUL22(a,b) ((((int32)(a)*(int32)(b))<<10)>>10)
#define IMUL23(a,b) ((((int32)(a)*(int32)(b))<<9)>>9)
#define IMUL24(a,b) ((((int32)(a)*(int32)(b))<<8)>>8)
#define IMUL25(a,b) ((((int32)(a)*(int32)(b))<<7)>>7)
#define IMUL26(a,b) ((((int32)(a)*(int32)(b))<<6)>>6)
#define IMUL27(a,b) ((((int32)(a)*(int32)(b))<<5)>>5)
#define IMUL28(a,b) ((((int32)(a)*(int32)(b))<<4)>>4)
#define IMUL29(a,b) ((((int32)(a)*(int32)(b))<<3)>>3)
#define IMUL30(a,b) ((((int32)(a)*(int32)(b))<<2)>>2)
#define IMUL31(a,b) ((((int32)(a)*(int32)(b))<<1)>>1)
#define IMUL32(a,b) ((int32)(a)*(int32)(b))

#define UMUL1(a,b) (((uint8)(a)*(uint8)(b))&0x1)
#define UMUL2(a,b) (((uint8)(a)*(uint8)(b))&0x3)
#define UMUL3(a,b) (((uint8)(a)*(uint8)(b))&0x7)
#define UMUL4(a,b) (((uint8)(a)*(uint8)(b))&0xf)
#define UMUL5(a,b) (((uint8)(a)*(uint8)(b))&0x1f)
#define UMUL6(a,b) (((uint8)(a)*(uint8)(b))&0x3f)
#define UMUL7(a,b) (((uint8)(a)*(uint8)(b))&0x7f)
#define UMUL8(a,b) ((uint8)(a)*(uint8)(b))
#define UMUL9(a,b) (((uint16)(a)*(uint16)(b))&0x1ff)
#define UMUL10(a,b) (((uint16)(a)*(uint16)(b))&0x3ff)
#define UMUL11(a,b) (((uint16)(a)*(uint16)(b))&0x7ff)
#define UMUL12(a,b) (((uint16)(a)*(uint16)(b))&0xfff)
#define UMUL13(a,b) (((uint16)(a)*(uint16)(b))&0x1fff)
#define UMUL14(a,b) (((uint16)(a)*(uint16)(b))&0x3fff)
#define UMUL15(a,b) (((uint16)(a)*(uint16)(b))&0x7fff)
#define UMUL16(a,b) ((uint16)(a)*(uint16)(b))
#define UMUL17(a,b) (((uint32)(a)*(uint32)(b))&0x1ffff)
#define UMUL18(a,b) (((uint32)(a)*(uint32)(b))&0x3ffff)
#define UMUL19(a,b) (((uint32)(a)*(uint32)(b))&0x7ffff)
#define UMUL20(a,b) (((uint32)(a)*(uint32)(b))&0xfffff)
#define UMUL21(a,b) (((uint32)(a)*(uint32)(b))&0x1fffff)
#define UMUL22(a,b) (((uint32)(a)*(uint32)(b))&0x3fffff)
#define UMUL23(a,b) (((uint32)(a)*(uint32)(b))&0x7fffff)
#define UMUL24(a,b) (((uint32)(a)*(uint32)(b))&0xffffff)
#define UMUL25(a,b) (((uint32)(a)*(uint32)(b))&0x1ffffff)
#define UMUL26(a,b) (((uint32)(a)*(uint32)(b))&0x3ffffff)
#define UMUL27(a,b) (((uint32)(a)*(uint32)(b))&0x7ffffff)
#define UMUL28(a,b) (((uint32)(a)*(uint32)(b))&0xfffffff)
#define UMUL29(a,b) (((uint32)(a)*(uint32)(b))&0x1fffffff)
#define UMUL30(a,b) (((uint32)(a)*(uint32)(b))&0x3fffffff)
#define UMUL31(a,b) (((uint32)(a)*(uint32)(b))&0x7fffffff)
#define UMUL32(a,b) ((uint32)(a)*(uint32)(b))

#define IDIV1(a,b) ((((int32)(a)/(int32)(b))<<31)>>31)
#define IDIV2(a,b) ((((int32)(a)/(int32)(b))<<30)>>30)
#define IDIV3(a,b) ((((int32)(a)/(int32)(b))<<29)>>29)
#define IDIV4(a,b) ((((int32)(a)/(int32)(b))<<28)>>28)
#define IDIV5(a,b) ((((int32)(a)/(int32)(b))<<27)>>27)
#define IDIV6(a,b) ((((int32)(a)/(int32)(b))<<26)>>26)
#define IDIV7(a,b) ((((int32)(a)/(int32)(b))<<25)>>25)
#define IDIV8(a,b) ((((int32)(a)/(int32)(b))<<24)>>24)
#define IDIV9(a,b) ((((int32)(a)/(int32)(b))<<23)>>23)
#define IDIV10(a,b) ((((int32)(a)/(int32)(b))<<22)>>22)
#define IDIV11(a,b) ((((int32)(a)/(int32)(b))<<21)>>21)
#define IDIV12(a,b) ((((int32)(a)/(int32)(b))<<20)>>20)
#define IDIV13(a,b) ((((int32)(a)/(int32)(b))<<19)>>19)
#define IDIV14(a,b) ((((int32)(a)/(int32)(b))<<18)>>18)
#define IDIV15(a,b) ((((int32)(a)/(int32)(b))<<17)>>17)
#define IDIV16(a,b) ((((int32)(a)/(int32)(b))<<16)>>16)
#define IDIV17(a,b) ((((int32)(a)/(int32)(b))<<15)>>15)
#define IDIV18(a,b) ((((int32)(a)/(int32)(b))<<14)>>14)
#define IDIV19(a,b) ((((int32)(a)/(int32)(b))<<13)>>13)
#define IDIV20(a,b) ((((int32)(a)/(int32)(b))<<12)>>12)
#define IDIV21(a,b) ((((int32)(a)/(int32)(b))<<11)>>11)
#define IDIV22(a,b) ((((int32)(a)/(int32)(b))<<10)>>10)
#define IDIV23(a,b) ((((int32)(a)/(int32)(b))<<9)>>9)
#define IDIV24(a,b) ((((int32)(a)/(int32)(b))<<8)>>8)
#define IDIV25(a,b) ((((int32)(a)/(int32)(b))<<7)>>7)
#define IDIV26(a,b) ((((int32)(a)/(int32)(b))<<6)>>6)
#define IDIV27(a,b) ((((int32)(a)/(int32)(b))<<5)>>5)
#define IDIV28(a,b) ((((int32)(a)/(int32)(b))<<4)>>4)
#define IDIV29(a,b) ((((int32)(a)/(int32)(b))<<3)>>3)
#define IDIV30(a,b) ((((int32)(a)/(int32)(b))<<2)>>2)
#define IDIV31(a,b) ((((int32)(a)/(int32)(b))<<1)>>1)
#define IDIV32(a,b) ((int32)(a)/(int32)(b))

#define UDIV1(a,b) (((uint8)(a)/(uint8)(b))&0x1)
#define UDIV2(a,b) (((uint8)(a)/(uint8)(b))&0x3)
#define UDIV3(a,b) (((uint8)(a)/(uint8)(b))&0x7)
#define UDIV4(a,b) (((uint8)(a)/(uint8)(b))&0xf)
#define UDIV5(a,b) (((uint8)(a)/(uint8)(b))&0x1f)
#define UDIV6(a,b) (((uint8)(a)/(uint8)(b))&0x3f)
#define UDIV7(a,b) (((uint8)(a)/(uint8)(b))&0x7f)
#define UDIV8(a,b) ((uint8)(a)/(uint8)(b))
#define UDIV9(a,b) (((uint16)(a)/(uint16)(b))&0x1ff)
#define UDIV10(a,b) (((uint16)(a)/(uint16)(b))&0x3ff)
#define UDIV11(a,b) (((uint16)(a)/(uint16)(b))&0x7ff)
#define UDIV12(a,b) (((uint16)(a)/(uint16)(b))&0xfff)
#define UDIV13(a,b) (((uint16)(a)/(uint16)(b))&0x1fff)
#define UDIV14(a,b) (((uint16)(a)/(uint16)(b))&0x3fff)
#define UDIV15(a,b) (((uint16)(a)/(uint16)(b))&0x7fff)
#define UDIV16(a,b) ((uint16)(a)/(uint16)(b))
#define UDIV17(a,b) (((uint32)(a)/(uint32)(b))&0x1ffff)
#define UDIV18(a,b) (((uint32)(a)/(uint32)(b))&0x3ffff)
#define UDIV19(a,b) (((uint32)(a)/(uint32)(b))&0x7ffff)
#define UDIV20(a,b) (((uint32)(a)/(uint32)(b))&0xfffff)
#define UDIV21(a,b) (((uint32)(a)/(uint32)(b))&0x1fffff)
#define UDIV22(a,b) (((uint32)(a)/(uint32)(b))&0x3fffff)
#define UDIV23(a,b) (((uint32)(a)/(uint32)(b))&0x7fffff)
#define UDIV24(a,b) (((uint32)(a)/(uint32)(b))&0xffffff)
#define UDIV25(a,b) (((uint32)(a)/(uint32)(b))&0x1ffffff)
#define UDIV26(a,b) (((uint32)(a)/(uint32)(b))&0x3ffffff)
#define UDIV27(a,b) (((uint32)(a)/(uint32)(b))&0x7ffffff)
#define UDIV28(a,b) (((uint32)(a)/(uint32)(b))&0xfffffff)
#define UDIV29(a,b) (((uint32)(a)/(uint32)(b))&0x1fffffff)
#define UDIV30(a,b) (((uint32)(a)/(uint32)(b))&0x3fffffff)
#define UDIV31(a,b) (((uint32)(a)/(uint32)(b))&0x7fffffff)
#define UDIV32(a,b) ((uint32)(a)/(uint32)(b))

#define FXCONST32(a,DW) ((uint32)((a)<<DW))
#define FXCONST32FL(f,DW) ((uint32)((f)*(1<<DW)))
#define FXADD32(a,b,DW) ((uint32)((a)+(b)))
#define FXSUB32(a,b,DW) ((uint32)((a)-(b)))
#define FX2REAL32(a,DW) ((float)(a)/(float)(1<<DW) - ((((a)&0x80000000)==0) ? 0 : (float)(1<<(32-DW))))

#define FXCONST16(a,DW) ((uint16)((a)<<DW))
#define FXCONST16FL(f,DW) ((uint16)((f)*(1<<DW)))
#define FXADD16(a,b,DW) ((uint16)((a)+(b)))
#define FXSUB16(a,b,DW) ((uint16)((a)-(b)))
#define FXMUL16(a,b,DW) ((uint16)(((((uint32)((int16)(a))*(uint32)((int16)(b)))+((uint32)1<<((DW)-1)))>>DW)))
#define FXDIV16(a,b,DW) ((uint16)((((((uint32)((int16)(a)))<<((DW)+1))/((uint32)((int16)(b)))+(uint32)1)>>1)))

#define FXCONST8(a,DW) ((uint8)((a)<<DW))
#define FXCONST8FL(f,DW) ((uint8)((f)*(1<<DW)))
#define FXADD8(a,b,DW) ((uint8)((a)+(b)))
#define FXSUB8(a,b,DW) ((uint8)((a)-(b)))
#define FXMUL8(a,b,DW) ((uint8)(((((uint16)((int8)(a))*(uint16)((int8)(b)))+((uint16)1<<((DW)-1)))>>DW)))
#define FXDIV8(a,b,DW) ((uint8)((((((uint16)((int8)(a)))<<((DW)+1))/((uint16)((int8)(b)))+(uint16)1)>>1)))

#endif  // CO_MATH_H
