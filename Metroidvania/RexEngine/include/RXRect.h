#ifndef RXRECT_H
#define RXRECT_H

#include <minmax.h>

struct RXRect {
	int x = 0, y = 0, w = 0, h = 0;
};

inline bool RXRectDoesCollide(const RXRect* A, const RXRect* B)
{
	int Amin, Amax, Bmin, Bmax;

	if (A == nullptr || B == nullptr)
		return false;

	if (A->w == 0 || B->w == 0 || A->h == 0 || B->h == 0)
		return false;

	/* Horizontal intersection */
	Amin = A->x;
	Amax = Amin + A->w;
	Bmin = B->x;
	Bmax = Bmin + B->w;
	if (Bmin > Amin) {
		Amin = Bmin;
	}
	if (Bmax < Amax) {
		Amax = Bmax;
	}
	if (Amax <= Amin) {
		return false;
	}
	/* Vertical intersection */
	Amin = A->y;
	Amax = Amin + A->h;
	Bmin = B->y;
	Bmax = Bmin + B->h;
	if (Bmin > Amin) {
		Amin = Bmin;
	}
	if (Bmax < Amax) {
		Amax = Bmax;
	}
	if (Amax <= Amin) {
		return false;
	}
	return true;
}


inline bool RXRectCollision(const RXRect* a, const RXRect* b, RXRect* result = 0) {
	if (a == nullptr || b == nullptr)
	{
		return false;
	}

	if (a->w == 0 || b->w == 0 || a->h == 0 || b->h == 0)
		return false;

	//if ((a->x <= b->x && a->x + a->w >= b->x) || (a->x >= b->x && b->x + b->w >= a->x) &&
	//	(a->y <= b->y && a->y + a->h >= b->y) || (a->y >= b->y && b->y + b->h >= a->y))

	if(RXRectDoesCollide(a,b))
	{
		if (result != nullptr)
		{
			//result->x = max(a->x, b->x);
			//result->y = max(a->y, b->y);
			
			int min_A, min_B, max_A, max_B;

			min_A = a->x;
			max_A = a->x + a->w;
			min_B = b->x;
			max_B = b->x + b->w;
			if (min_B > min_A)
			{
				min_A = min_B;
			}

			result->x = min_A;
			if (max_B < max_A)
			{
				max_A = max_B;
			}
			result->w = max_A - min_A;


			min_A = a->y;
			max_A = a->y + a->h;
			min_B = b->y;
			max_B = b->y + b->h;
			if (min_B > min_A)
			{
				min_A = min_B;
			}

			result->y = min_A;
			if (max_B < max_A)
			{
				max_A = max_B;
			}
			result->h = max_A - min_A;


			//if (a->x + a->w >= b->x + b->w)
			//{
			//	result->w = b->x + b->w - result->x;
			//}
			//else
			//{
			//	result->w = a->x + a->w - result->x;
			//}

			//if (a->y + a->h >= b->y + b->h)
			//{
			//	result->h = b->y + b->h - result->y;
			//}
			//else
			//{
			//	result->h = a->y + a->h - result->y;
			//}
		}
		return true;
	}
	else
	{
		return false;
	}
}

#endif