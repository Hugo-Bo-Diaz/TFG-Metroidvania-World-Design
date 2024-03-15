#ifndef RXColor_H
#define RXColor_H

#include <cstdint>

struct RXColor{
	RXColor() {};

	unsigned __int8 r = 0, g = 0, b = 0, a = 0;

	RXColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a):r(r), g(g), b(b), a(a){}

	bool operator==(const RXColor& other)
	{
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
};


#endif