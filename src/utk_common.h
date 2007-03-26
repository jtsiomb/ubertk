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
