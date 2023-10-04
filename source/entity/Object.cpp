#include "Object.h"

Object::Object(int xCoord, int yCoord, EnumObjectType::ObjectTypes type)
	: x(xCoord), y(yCoord), type(type)  {

	energy = 0;
	screenX = 0;

	this->color.r = 255;
	this->color.g = 0;
	this->color.b = 255;
}

Color Object::GetColor() {
	return this->color;
}

uint Object::GetLifetime() {
	return lifetime;
}

void Object::SetLifetime(uint val) {
	lifetime = val;
}

