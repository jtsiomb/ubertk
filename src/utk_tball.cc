#include <math.h>
#include "utk_tball.h"
#include "utk_common.h"

namespace utk {

struct vec3_t {
	float x, y, z;
};

static vec3_t get_primary_ray(int x, int y, int w, int h)
{
	vec3_t pt;
	float aspect = (float)w / (float)h;

	pt.x = ((float)x / (float)w) - 0.5;
	pt.y = -(((float)y / (float)h) - 0.5) / aspect;
	pt.z = 1.0;

	return pt;
}

#define SQ(x)	((x) * (x))

// XXX: assumes rays start from origin
bool ray_sphere(vec3_t spos, float srad, vec3_t rdir, vec3_t *sp)
{
	float a, b, c, d, sqrt_d, t1, t2, dist;

	a = SQ(rdir.x) + SQ(rdir.y) + SQ(rdir.z);
	b = 2.0 * rdir.x * -spos.x + 2.0 * rdir.y * -spos.y + 2.0 * rdir.z * -spos.z;
	c = SQ(spos.x) + SQ(spos.y) + SQ(spos.z) + 2.0 * (-spos.x - spos.y - spos.z) - SQ(srad);

	if((d = SQ(b) - 4.0 * a * c) < 0.0) return false;

	sqrt_d = sqrt(d);
	t1 = (-b + sqrt_d) / (2.0 * a);
	t2 = (-b - sqrt_d) / (2.0 * a);

	if(t1 < 0.00001 && t2 < 0.00001) return false;

	if(t1 < 0.00001) t1 = t2;
	if(t2 < 0.00001) t2 = t1;
	dist = t1 < t2 ? t1 : t2;

	sp->x = rdir.x * dist;
	sp->y = rdir.y * dist;
	sp->z = rdir.z * dist;
	return true;
}

#define PI		3.141592653f
#define TWO_PI	6.283185307f

void TrackBall::update()
{
	vec3_t ldir = {-0.577, 0.577, -0.577};
	vec3_t sph_pos = {0, 0, 10.5};
	float sph_rad = 1.0;

	unsigned int *pptr = pixels;
	for(int y=0; y<img_h; y++) {
		for(int x=0; x<img_w; x++) {

			vec3_t sp, norm, ray_dir = get_primary_ray(x, y, img_w, img_h);

			if(ray_sphere(sph_pos, sph_rad, ray_dir, &sp)) {
				norm.x = sp.x - sph_pos.x;
				norm.y = sp.y - sph_pos.y;
				norm.z = sp.z - sph_pos.z;
				float nlen = sqrt(SQ(norm.x) + SQ(norm.y) + SQ(norm.z));
				norm.x /= nlen;
				norm.y /= nlen;
				norm.z /= nlen;

				float dot = ldir.x * norm.x + ldir.y * norm.y + ldir.z * norm.z;
				dot = dot < 0.0 ? 0.0 : (dot > 1.0 ? 1.0 : dot);

				float theta = atan2(norm.z, norm.x) - this->theta;
				float phi = acos(norm.y) - this->phi;

				while(theta < 0.0) theta += TWO_PI;
				while(phi < 0.0) phi += PI;

				int tx = (int)(12.0f * fmod(theta, TWO_PI) / TWO_PI);
				int ty = (int)(6.0 * fmod(phi, PI) / PI);

				int cint = (int)(50.0 * dot);
				if((tx % 2) == (ty % 2)) {
					cint = (int)(255.0 * dot);
				}

				*pptr++ = PACK_RGBA(cint, cint, cint, 0xff);
			} else {
				*pptr++ = 0;
			}
		}
	}
}

void TrackBall::on_click(int x, int y)
{

}
void TrackBall::on_motion(int x, int y)
{

}
void TrackBall::on_drag(int dx, int dy)
{
	theta += (3.14159f / 2.0f) * ((float) dx / (float) img_w);
	phi += (3.14159f / 2.0f) * ((float) dy / (float) img_h);
}

TrackBall::TrackBall(int w, int h, utk::Callback cb) : Image(w, h, cb)
{
	phi = theta = 0;
	update();
}
TrackBall::~TrackBall()
{

}

float TrackBall::get_phi() const
{
	return phi;
}

float TrackBall::get_theta() const
{
	return theta;
}


} // ena namespace utk
