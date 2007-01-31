// utk_tball.cc

#include "utk_tball.h"
#include <math.h>

namespace utk {

// x, y, z are [-1, 1]
static float get_depth(float x, float y);
static float get_phi(float x, float y, float z);
static float get_theta(float x, float y, float z);

static inline bool raytrace_sphere(float x, float y, float &phi, float &theta)
{
	// eye point
	//Vector3 eye = Vector3(0, 0, -2);
	float eye_x, eye_y, eye_z;
	eye_x = eye_y = 0;
	eye_z = -1;

	//Vector3 ray_dir = Vector3(x, y, 0) - eye;
	float rd_x, rd_y, rd_z;
	rd_x = x;
	rd_y = y;
	rd_z = 0;
	rd_x -= eye_x;
	rd_y -= eye_y;
	rd_z -= eye_z;
	
	//ray_dir.Normalize();
	float rd_l = sqrt(rd_x*rd_x + rd_y*rd_y + rd_z*rd_z);
	rd_x /= rd_l;
	rd_y /= rd_l;
	rd_z /= rd_l;

	// closest point to the center
	//Vector3 eye_to_center = Vector3(0, 0, 0) - eye;
	float ec_x, ec_y, ec_z;
	ec_x = -eye_x;
	ec_y = -eye_y;
	ec_z = -eye_z;
	
	//float dist = dot_product(ray_dir, eye_to_center);
	float dist = ec_x * rd_x + ec_y * rd_y + ec_z * rd_z;
	
	//Vector3 closest_point = eye + dist * ray_dir;
	float cp_x, cp_y, cp_z;
	cp_x = rd_x * dist + eye_x;
	cp_y = rd_y * dist + eye_y;
	cp_z = rd_z * dist + eye_z;


	//float closest_length = closet_point.length();
	float cp_l = sqrt(cp_x * cp_x + cp_y * cp_y + cp_z * cp_z);

	if (cp_l > 1.0f) return false;

	// how deep is the closest point
	//float deep = sqrt(closest_length * closest_length + 1.0f * 1.0f);
	float deep = sqrt(cp_l * cp_l + 1.0f);

	// sphere point
	//Vector3 sp = closest_point - deep * ray_dir;
	float sp_x, sp_y, sp_z;
	sp_x = cp_x - deep * rd_x;
	sp_y = cp_y - deep * rd_y;
	sp_z = cp_z - deep * rd_z;

	phi = get_phi(sp_x, sp_y, sp_z);
	theta = get_theta(sp_x, sp_y, sp_z);
	return true;
}

void TrackBall::update()
{
	float x, y, phi, theta;
	unsigned int *dst = pixels;
	for (int j=0; j<img_h; j++)
	{
		for (int i=0; i<img_w; i++)
		{
			x = (float) i / (float) (img_w - 1);
			x = (x * 2) - 1;
			y = (float) j / (float) (img_h - 1);
			y = (y * 2) - 1;
		

			


			if (!raytrace_sphere(x, y, phi, theta))
			{
				*dst++ = 0;
				continue;
			}

			phi = fmod(phi, 3.14159f) + this->phi;
			theta = fmod(theta, 2.0f * 3.14159f) + this->theta;

			phi = 6 * (phi / (3.14159));
			theta = 12 * (theta / (3.14159 * 2));

			int p = phi;
			p %= 12;
			int t = theta;
			t %= 6;

			//printf("%f, %f\n", phi, theta);
			unsigned int color = 0xFF000000;
			if ((p + t) % 2) color = 0xFFFFFFFF;

			*dst++ = color;
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

static float get_depth(float x, float y)
{
	float d = (float) hypot(x, y);
	if (d > 1.0f) return 0.0f;

	return cos(d);
}

static float get_phi(float x, float y, float z)
{
	return acos(y / sqrt(x*x + y*y + z*z));
}

static float get_theta(float x, float y, float z)
{
	return atan2(z, x);
}















} // ena namespace utk