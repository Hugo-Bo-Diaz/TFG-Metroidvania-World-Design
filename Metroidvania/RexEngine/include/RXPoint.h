#ifndef RXPOINT_H
#define RXPOINT_H

#include "RXRect.h"

struct RXPoint
{
	float x, y;

};

inline bool RXPointInRect(RXPoint* p, RXRect* r)
{
	return (p->x > r->x&& p->x < r->x + r->w
		 && p->y > r->y&& p->y < r->y + r->h);

}

#endif