// utk_hsv.cc
#include <math.h>
#include "utk_common.h"

namespace utk {

inline float min3(float a, float b, float c)
{
	float ret = a;
	if (b < ret) ret = b;
	if (c < ret) ret = c;
	return ret;
}

inline float max3(float a, float b, float c)
{
	float ret = a;
	if (b > ret) ret = b;
	if (c > ret) ret = c;
	return ret;
}

void rgb_to_hsv( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;

	min = min3( r, g, b );
	max = max3( r, g, b );
	*v = max;				// v

	delta = max - min;

	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan

	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;

	// normalize h
	*h /= 360;
}

void hsv_to_rgb( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;

	// convert h to [0-360]
	h *= 360;

	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = (int)floorf( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}

}

unsigned int pack_hsv(float h, float s, float v)
{
	float r, g, b;
	int ir, ig, ib;
	hsv_to_rgb(&r, &g, &b, h, s, v);
	ir = (int)(255.0f * r + 0.5f);
	ig = (int)(255.0f * g + 0.5f);
	ib = (int)(255.0f * b + 0.5f);

	ir = ir < 0 ? 0 : (ir > 255 ? 255 : ir);
	ig = ig < 0 ? 0 : (ig > 255 ? 255 : ig);
	ib = ib < 0 ? 0 : (ib > 255 ? 255 : ib);

	return PACK_RGBA(ir, ig, ib, 0xff);
}

} // end namespace utk
