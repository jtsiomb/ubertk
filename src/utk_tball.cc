// utk_tball.cc

#include "utk_tball.h"
#include <math.h>

namespace utk {

// x, y, z are [-1, 1]
static float get_depth(float x, float y);
static float get_phi(float x, float y, float z);
static float get_theta(float x, float y, float z);

void TrackBall::update()
{
	float x, y, z, phi, theta;
	for (int j=0; j<img_h; j++)
	{
		for (int i=0; i<img_w; i++)
		{
			x = (float) i / (float) (img_w - 1);
			x = (x * 2) - 1;
			y = (float) j / (float) (img_h - 1);
			y = (y * 2) - 1;
			z = get_depth(x, y);

			
			float len = sqrt(x*x + y*y + z*z);
			x /= len;
			y /= len;
			z /= len;
			

			float lx, ly, lz;
			lx = -sqrt(3.0);
			ly = sqrt(3.0);
			lz = sqrt(3.0);

			float dot = x * lx + y * ly + z * lz;
			int dif = 255 * dot;

			unsigned int ccc = 0xFF000000 | (dif << 16) | (dif << 8) | dif;  


			if (z < 0.001f)
			{
				pixels[i + img_w * j] = 0xff;
				continue;
			}

			phi = get_phi(x, y, z) + this->phi;
			theta = get_theta(x, y, z) + this->theta;

			phi = fmod(phi, 3.14159f);
			theta = fmod(theta, 2.0f * 3.14159f);

			phi = 6 * (phi / (3.14159));
			theta = 12 * (theta / (3.14159 * 2));

			int p = phi;
			p %= 12;
			int t = theta;
			t %= 6;

			//printf("%f, %f\n", phi, theta);
			unsigned int color = 0xFF000000;
			if ((p + t) % 2) color = ccc;

			pixels[i + img_w * j] = color;
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