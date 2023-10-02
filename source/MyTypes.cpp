
#include "MyTypes.h"


void Color::SetRandom() {
	r = RandomVal(256);
	g = RandomVal(256);
	b = RandomVal(256);
}

Color::Color() {}

Point::Point(int X, int Y) :x(X), y(Y) {}

Point::Point() :x(-1), y(-1) {}

void Point::Shift(int X, int Y)  {
	x += X;
	y += Y;
}

void Point::Set(int X, int Y)  {
	x = X;
	y = Y;
}

bool Rect::IsInBounds(Point p) {
	return ((p.x >= x) && (p.y >= y) && (p.x <= x + w) && (p.y <= y + h));
}

float RandomFloatInRange(float min, float max) { 
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))); 
}
