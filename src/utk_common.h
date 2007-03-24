#ifndef UTK_MACROS_H_
#define UTK_MACROS_H_

#define MIN(a, b)		((a) < (b) ? (a) : (b))
#define MAX(a, b)		((a) > (b) ? (a) : (b))
#define CLAMP(x, a, b)	MIN(MAX(x, a), b)

/* 32bit color shift values */
#if defined(LITTLE_ENDIAN) || defined(__LITTLE_ENDIAN__)
#define ALPHA_SHIFT32	24
#define RED_SHIFT32		16
#define GREEN_SHIFT32	8
#define BLUE_SHIFT32	0
#else	/* BIG_ENDIAN */
#define ALPHA_SHIFT32	0
#define RED_SHIFT32		8
#define GREEN_SHIFT32	16
#define BLUE_SHIFT32	24
#endif	/* LITTLE_ENDIAN */

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
