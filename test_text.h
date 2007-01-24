#ifndef TEXT_H
#define TEXT_H

#if defined(WIN32) || defined(__WIN32__)
#include <windows.h>
#endif

class Vector2 {
public:
	float x, y;

	Vector2(float a = 0.0f, float b = 0.0f) : x(a), y(b) {}
	Vector2 operator+(const Vector2 &v) const {return Vector2(x + v.x, y + v.y);}
	Vector2 operator-(const Vector2 &v) const {return Vector2(x - v.x, y - v.y);}
	Vector2 operator*(float s) const {return Vector2(x * s, y * s);}
};

class Color {
public:
	float r, g, b, a;

	Color() {r = g = b = a = 1.0f;}
	Color(float r, float g, float b, float a = 1.0f) {this->r = r; this->g = g; this->b = b; this->a = a;}
};


unsigned int CreateFont(const char *fname, int font_size);
void DeleteFont(unsigned int fid);
unsigned int GetFont(const char *fname, int sz);
bool BindFont(unsigned int fid);


void SetTextPos(const Vector2 &pos);
Vector2 GetTextPos();

void TextLineAdvance(int adv = 1);
void TextCRet();

void SetTextSize(float sz);
float GetTextSize();

void SetTextColor(const Color &col);
Color GetTextColor();

float PrintChar(char c);
float PrintString(const char *text, bool standalone = true);
void PrintStringLines(const char **str, int lines);

float GetMaxDescent();
float GetLineAdvance();
float GetTextWidth(const char *str);

#endif	// TEXT_H
