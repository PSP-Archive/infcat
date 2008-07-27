//! @file Cat_MD5.h
// MD5の計算

// RFC1321 "APPENDIX A - Reference Implementation"を元に修正したものです。

/* MD5C.H - RSA Data Security, Inc., MD5 message-digest algorithm */
/*
	Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
	rights reserved.

	License to copy and use this software is granted provided that it
	is identified as the "RSA Data Security, Inc. MD5 Message-Digest
	Algorithm" in all material mentioning or referencing this software
	or this function.

	License is also granted to make and use derivative works provided
	that such works are identified as "derived from the RSA Data
	Security, Inc. MD5 Message-Digest Algorithm" in all material
	mentioning or referencing the derived work.

	RSA Data Security, Inc. makes no representations concerning either
	the merchantability of this software or the suitability of this
	software for any particular purpose. It is provided "as is"
	without express or implied warranty of any kind.

	These notices must be retained in any copies of any part of this
	documentation and/or software.
*/

#ifndef INCL_Cat_MD5_h
#define INCL_Cat_MD5_h

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//! MD5計算用のコンテキスト
typedef struct{
	uint32_t	state[4];
	uint32_t	count[2];
	uint8_t		buffer[64];
} MD5_CTX;

//! MD5 initialization. Begins an MD5 operation, writing a new context.
/*!
	@param[in,out]	context	context
*/
extern void Cat_MD5Init( MD5_CTX* context );

//! MD5 block update operation.
/*!
	Continues an MD5 message-digest operation, processing another message block,
	and updating the context.

	@param[in,out]	context		context
	@param[in]		input		input block
	@param[in]		inputLen	length of input block
*/
extern void Cat_MD5Update( MD5_CTX* context, const uint8_t* input, size_t inputLen );

//! MD5 finalization.
/*!
	Ends an MD5 message-digest operation, writing the the message digest and zeroizing the context.

	@param[in,out]	context	context
	@param[out]		digest	message digest
*/
extern void Cat_MD5Final( MD5_CTX* context, uint8_t digest[16] );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_MD5_h
