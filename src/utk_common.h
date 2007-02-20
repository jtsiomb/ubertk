#ifndef UTK_MACROS_H_
#define UTK_MACROS_H_

#define MIN(a, b)		((a) < (b) ? (a) : (b))
#define MAX(a, b)		((a) > (b) ? (a) : (b))
#define CLAMP(x, a, b)	MIN(MAX(x, a), b)

#ifdef __cplusplus
extern "C" {
#endif
	
void utk_error(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif	/* UTK_MACROS_H_ */
