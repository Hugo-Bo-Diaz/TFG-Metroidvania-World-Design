#ifndef MATH__H
#define MATH__H

#include <math.h>

// credit goes to https://www.geeksforgeeks.org/program-calculate-distance-two-points/
float DistanceBetweenTwoPoints(float x1, float y1, float x2, float y2)
{
	// Calculating distance 
	return sqrt(pow(x2 - x1, 2) +
				pow(y2 - y1, 2) * 1.0);
}


//credit goes to https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
bool LineIntersection(int Ax, int Ay, int Bx, int By, int Cx, int Cy, int Dx, int Dy, float& RetX, float& RetY)
{
	// Line AB represented as a1x + b1y = c1 
	double a1v = By - Ay;
	double b1v = Ax - Bx;
	double c1v = a1v * (Ax) + b1v * (Ay);

	// Line CD represented as a2x + b2y = c2 
	double a2v = Dy - Cy;
	double b2v = Cx - Dx;
	double c2v = a2v * (Cx) + b2v * (Cy);

	double determinant = a1v * b2v - a2v * b1v;

	if (determinant == 0)
	{
		// The lines are parallel. This is simplified 
		// by returning a pair of FLT_MAX 
		return false;
	}
	else
	{
		RetX = (b2v*c1v - b1v * c2v) / determinant;
		RetY = (a1v*c2v - a2v * c1v) / determinant;
		return true;
	}
}

#endif // !1


