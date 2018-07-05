/*
ubertk is a flexible GUI toolkit targetted towards graphics applications.
Copyright (C) 2007 - 2013 John Tsiombikas <nuclear@member.fsf.org>,
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

// utk_hsv.cc
#include <stdio.h>
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

	if(!delta) delta = 1.0f;

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

#define RETRGB(red, green, blue) \
	do { \
		*r = (red); \
		*g = (green); \
		*b = (blue); \
		return; \
	} while(0)

void hsv_to_rgb(float *r, float *g, float *b, float h, float s, float v)
{
	if(s == 0.0f) {
		*r = *g = *b = v;
		return;
	}

	float sec = floor(h * (360.0f / 60.0f));
	float frac = (h * (360.0f / 60.0f)) - sec;

	float o = v * (1.0f - s);
	float p = v * (1.0f - s * frac);
	float q = v * (1.0f - s * (1.0f - frac));

	int hidx = (int)sec;
	switch(hidx) {
	default:
	case 0: RETRGB(v, q, o);
	case 1: RETRGB(p, v, o);
	case 2: RETRGB(o, v, q);
	case 3: RETRGB(o, p, v);
	case 4: RETRGB(q, o, v);
	case 5: RETRGB(v, o, p);
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
