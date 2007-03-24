#include <math.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "test_text.h"

class Matrix4x4 {
public:
	float m[16];

	Matrix4x4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float x, float n, float o, float p)
	{
		m[0] = a; m[1] = e; m[2] = i; m[3] = x;
		m[4] = b; m[5] = f; m[6] = j; m[7] = n;
		m[8] = c; m[9] = g; m[10] = k; m[11] = o;
		m[12] = d; m[13] = h; m[14] = l; m[15] = p;
	}
};


#define MAX_CHARS		128
#define MAX_IMG_WIDTH	1024
#define SIZE_PIXELS(x)	((x) / 64)

struct Font {
	unsigned int tex_id;
	float scale;	// this compensates if a higher res font is loaded in its stead, with a new CreateFont
	float line_adv;	// vertical advance to go to the next line
	struct {
		Vector2 pos, size;		// glyph position (from origin) and size in normalized coords [0, 1]
		float advance;			// advance in normalized coords
		Vector2 tc_pos, tc_sz;	// tex coord box pos/size
	} glyphs[MAX_CHARS];
};

static void BlitFontGlyph(Font *fnt, int x, int y, FT_GlyphSlot glyph, unsigned int *img, int xsz, int ysz);
static void CleanUp();

static FT_Library ft;
static Vector2 text_pos;
static float text_size = 1.0;
static Color text_color;
static Font *act_fnt;

#if !defined(GLIBC) && !defined(__GLIBC__)
float log2(float x) {
	float res = 0.0;
	while(x > 1.0) {
		x /= 2.0;
		res += 1.0;
	}
	return res;
}
#endif	// _MSC_VER


static inline int NextPow2(int x)
{
	float lg2 = log2((float)x);
	return (int)pow(2.0f, (int)ceil(lg2));
}


unsigned int CreateFont(const char *fname, int font_size)
{
	if(!ft) {
		if(FT_Init_FreeType(&ft) != 0) {
			fprintf(stderr, "failed to initialize freetype\n");
			return 0;
		}
		atexit(CleanUp);
	}

	FT_Face face;
	if(FT_New_Face(ft, fname, 0, &face) != 0) {
		fprintf(stderr, "failed to load font: %s\n", fname);
		return 0;
	}

	FT_Set_Pixel_Sizes(face, 0, font_size);

	Font *fnt = new Font;
	int max_width = MAX_CHARS * SIZE_PIXELS(face->bbox.xMax - face->bbox.xMin);
	int foo_xsz = MAX_IMG_WIDTH;
	int foo_ysz = SIZE_PIXELS(face->bbox.yMax - face->bbox.yMin) * max_width / foo_xsz;

	int tex_xsz = NextPow2(foo_xsz);
	int tex_ysz = NextPow2(foo_ysz);

	unsigned int *img;
	img = new unsigned int[tex_xsz * tex_ysz];
	memset(img, 0, tex_xsz * tex_ysz * sizeof *img);

	extern int xsz, ysz;
	int vport_xsz = xsz, vport_ysz = ysz;

	int max_glyph_y = 0;
	int max_glyph_x = 0;
	for(int i=0; i<MAX_CHARS; i++) {
		FT_Load_Char(face, i, 0);
		int width = SIZE_PIXELS(face->glyph->metrics.width);
		int height = SIZE_PIXELS(face->glyph->metrics.height);
		
		if(height > max_glyph_y) {
			max_glyph_y = height;
		}

		if(width > max_glyph_x) {
			max_glyph_x = width;
		}
	}

	int gx = 0, gy = 0;
	for(int i=0; i<MAX_CHARS; i++) {
		FT_Load_Char(face, i, FT_LOAD_RENDER);
		FT_GlyphSlot g = face->glyph;

		int gwidth = SIZE_PIXELS(g->metrics.width);
		int gheight = SIZE_PIXELS(g->metrics.height);

		if(gx > MAX_IMG_WIDTH - gwidth) {
			gx = 0;
			gy += max_glyph_y;
		}

		BlitFontGlyph(fnt, gx, gy, g, img, tex_xsz, tex_ysz);
		fnt->scale = 1.0;
		fnt->line_adv = (float)SIZE_PIXELS(g->metrics.vertAdvance) / (float)vport_ysz;
		fnt->glyphs[i].tc_pos.x = (float)gx / (float)tex_xsz;
		fnt->glyphs[i].tc_pos.y = (float)gy / (float)tex_ysz;
		fnt->glyphs[i].tc_sz.x = (float)gwidth / (float)tex_xsz;
		fnt->glyphs[i].tc_sz.y = (float)gheight / (float)tex_ysz;
		fnt->glyphs[i].size.x = (float)gwidth / (float)vport_xsz;
		fnt->glyphs[i].size.y = (float)gheight / (float)vport_ysz;
		fnt->glyphs[i].pos.x = (float)SIZE_PIXELS(g->metrics.horiBearingX) / (float)vport_xsz;
		fnt->glyphs[i].pos.y = -(float)SIZE_PIXELS(g->metrics.horiBearingY) / (float)vport_ysz;
		fnt->glyphs[i].advance = (float)SIZE_PIXELS(g->metrics.horiAdvance) / (float)vport_xsz;

		gx += gwidth;
	}

	FT_Done_Face(face);

	glGenTextures(1, &fnt->tex_id);
	glBindTexture(GL_TEXTURE_2D, fnt->tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, tex_xsz, tex_ysz, 0, GL_BGRA, GL_UNSIGNED_BYTE, img);
	delete [] img;

	act_fnt = fnt;

	return 1;
}

void DeleteFont(unsigned int fid)
{
}

unsigned int GetFont(const char *fname, int sz)
{
	return 1;
}

bool BindFont(unsigned int fid)
{
	return true;
}

void SetTextPos(const Vector2 &pos)
{
	text_pos = pos;
}

Vector2 GetTextPos()
{
	return text_pos;
}

void TextLineAdvance(int adv)
{
	text_pos.y += (float)adv * act_fnt->line_adv;
}

void TextCRet()
{
	text_pos.x = 0.0;
}

void SetTextSize(float sz)
{
	text_size = sz;
}

float GetTextSize()
{
	return text_size;
}

void SetTextColor(const Color &col)
{
	text_color = col;
}

Color GetTextColor()
{
	return text_color;
}

static void ImOverlay(const Vector2 &v1, const Vector2 &v2, const Color &col, unsigned int tex)
{
	float l = v1.x * 2.0f - 1.0f;
	float r = v2.x * 2.0f - 1.0f;
	float u = -v1.y * 2.0f + 1.0f;
	float d = -v2.y * 2.0f + 1.0f;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, tex);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	glBegin(GL_QUADS);
	glColor4f(col.r, col.g, col.b, col.a);
	glTexCoord2f(0, 0);
	glVertex2f(l, u);
	glTexCoord2f(1, 0);
	glVertex2f(r, u);
	glTexCoord2f(1, 1);
	glVertex2f(r, d);
	glTexCoord2f(0, 1);
	glVertex2f(l, d);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

float PrintChar(char c)
{
	// get texture coordinates for the glyph, and construct the texture matrix
	float tx = act_fnt->glyphs[(int)c].tc_pos.x;
	float ty = act_fnt->glyphs[(int)c].tc_pos.y;
	float sx = act_fnt->glyphs[(int)c].tc_sz.x;
	float sy = act_fnt->glyphs[(int)c].tc_sz.y;

	Matrix4x4 mat(	sx, 0, tx, 0,
					0, sy, ty, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadMatrixf(mat.m);

	Vector2 pos = text_pos + act_fnt->glyphs[(int)c].pos * act_fnt->scale;
	ImOverlay(pos, pos + act_fnt->glyphs[(int)c].size * act_fnt->scale, text_color, act_fnt->tex_id);

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	return act_fnt->glyphs[(int)c].advance * act_fnt->scale;
}

// this function contains the preamble of all block text drawing functions (i.e. not printchar above)
static void PreDraw()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	/*glLoadTransposeMatrixf(OrthoProj(2, 2, 0, 10).m);*/
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, act_fnt->tex_id);

	glBegin(GL_QUADS);
	glColor4f(text_color.r, text_color.g, text_color.b, text_color.a);
}

static void PostDraw()
{
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

float PrintString(const char *str, bool standalone)
{
	if(standalone) PreDraw();
	
	float start_x = text_pos.x;
	while(*str) {
		float tx = act_fnt->glyphs[(int)*str].tc_pos.x;
		float ty = act_fnt->glyphs[(int)*str].tc_pos.y;
		float sx = act_fnt->glyphs[(int)*str].tc_sz.x;
		float sy = act_fnt->glyphs[(int)*str].tc_sz.y;

		Vector2 tc1 = Vector2(tx, ty);
		Vector2 tc2 = Vector2(tx + sx, ty + sy);

		Vector2 v1 = text_pos + act_fnt->glyphs[(int)*str].pos * act_fnt->scale * text_size;
		Vector2 v2 = v1 + act_fnt->glyphs[(int)*str].size * act_fnt->scale * text_size;
		float l = v1.x * 2.0f - 1.0f;
		float r = v2.x * 2.0f - 1.0f;
		float u = -v1.y * 2.0f + 1.0f;
		float d = -v2.y * 2.0f + 1.0f;

		glTexCoord2f(tc1.x, tc1.y);
		glVertex2f(l, u);
		glTexCoord2f(tc2.x, tc1.y);
		glVertex2f(r, u);
		glTexCoord2f(tc2.x, tc2.y);
		glVertex2f(r, d);
		glTexCoord2f(tc1.x, tc2.y);
		glVertex2f(l, d);

		text_pos.x += act_fnt->glyphs[(int)*str++].advance * act_fnt->scale * text_size;
	}

	if(standalone) PostDraw();
	return text_pos.x - start_x;
}

void PrintStringLines(const char **str, int lines)
{
	PreDraw();

	while(lines-- > 0) {
		PrintString(*str++, false);
		TextLineAdvance();
		TextCRet();
	}

	PostDraw();
}

static void BlitFontGlyph(Font *fnt, int x, int y, FT_GlyphSlot glyph, unsigned int *img, int xsz, int ysz)
{
	if(glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) {
		fprintf(stderr, "BlitFontGlyph: unsupported pixel mode: %d\n", glyph->bitmap.pixel_mode);
		return;
	}

	unsigned int *dest = img + y * xsz + x;
	unsigned char *src = glyph->bitmap.buffer;

	for(int j=0; j<glyph->bitmap.rows; j++) {
		for(int i=0; i<glyph->bitmap.width; i++) {
			dest[i] = 0x00ffffff | ((unsigned int)src[i] << 24);
		}
		dest += xsz;
		src += glyph->bitmap.pitch;
	}
}

static void CleanUp()
{
	FT_Done_FreeType(ft);
}

float GetMaxDescent()
{
	Font *fnt = act_fnt;

	float max_descent = 0.0f;
	
	for(int i=0; i<MAX_CHARS; i++) {
		float des = fnt->glyphs[i].pos.y + fnt->glyphs[i].size.y;
		if(isprint(i) && des > max_descent) {
			max_descent = des;
		}
	}

	return max_descent;
}

float GetLineAdvance()
{
	return act_fnt->line_adv;
}

float GetTextWidth(const char *str)
{
	float width = 0;
	while(*str) {
		width += act_fnt->glyphs[(int)*str++].advance * act_fnt->scale * text_size;
	}
	return width;
}
