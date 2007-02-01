// utk_hsv.h

#ifndef _UTK_HSV_H_
#define _UTK_HSV_H_

namespace utk {

void rgb_to_hsv( float r, float g, float b, float *h, float *s, float *v );
void hsv_to_rgb( float *r, float *g, float *b, float h, float s, float v );
unsigned int pack_hsv(float h, float s, float v);

} // end namespace utk

#endif // ndef _UTK_HSV_H_