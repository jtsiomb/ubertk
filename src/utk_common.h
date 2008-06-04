/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2008 John Tsiombikas <nuclear@member.fsf.org>,
                          Michael Georgoulopoulos <mgeorgoulopoulos@gmail.com>,
				          Kostas Michalopoulos <badsector@slashstone.com>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/
#ifndef UTK_MACROS_H_
#define UTK_MACROS_H_

#define MIN(a, b)		((a) < (b) ? (a) : (b))
#define MAX(a, b)		((a) > (b) ? (a) : (b))
#define CLAMP(x, a, b)	MIN(MAX(x, a), b)

#if !defined(BYTE_ORDER) && !defined(__BYTE_ORDER)
#	if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
#		if  defined(__i386__) || defined(__ia64__) || \
			defined(WIN32) || defined(__WIN32__) || \
    		(defined(__alpha__) || defined(__alpha)) || \
		     defined(__arm__) || \
		    (defined(__mips__) && defined(__MIPSEL__)) || \
		     defined(__SYMBIAN32__) || \
		     defined(__x86_64__) || \
		     defined(__LITTLE_ENDIAN__)

#			define UTK_LITTLE_ENDIAN
#		else
#			define UTK_BIG_ENDIAN
#		endif	/* platform check */
#	endif	/* !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN) */
#else
#	if (defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN) || (defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN)
#		define UTK_LITTLE_ENDIAN
#	else
#		define UTK_BIG_ENDIAN
#	endif
#endif


/* 32bit color shift values */
#if defined(UTK_LITTLE_ENDIAN)
#define ALPHA_SHIFT32	24
#define RED_SHIFT32		16
#define GREEN_SHIFT32	8
#define BLUE_SHIFT32	0
#else	/* UTK_BIG_ENDIAN */
#define ALPHA_SHIFT32	0
#define RED_SHIFT32		8
#define GREEN_SHIFT32	16
#define BLUE_SHIFT32	24
#endif	/* UTK_LITTLE_ENDIAN */

/* 32bit color mask values */
#define ALPHA_MASK32	(0xff << ALPHA_SHIFT32)
#define RED_MASK32		(0xff << RED_SHIFT32)
#define GREEN_MASK32	(0xff << GREEN_SHIFT32)
#define BLUE_MASK32		(0xff << BLUE_SHIFT32)

#define PACK_RGBA(r, g, b, a) \
	((((r) << RED_SHIFT32) & RED_MASK32) | \
	 (((g) << GREEN_SHIFT32) & GREEN_MASK32) | \
	 (((b) << BLUE_SHIFT32) & BLUE_MASK32) | \
	 (((a) << ALPHA_SHIFT32) & ALPHA_MASK32))


#ifdef __cplusplus
extern "C" {
#endif
	
void utk_error(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif	/* UTK_MACROS_H_ */
