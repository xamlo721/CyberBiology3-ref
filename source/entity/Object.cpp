#include "Object.h"

#include "Bot.h"

#include "../renderer/WorldRenderer.h"
#include "../logic/WorldController.h"
#include "../SDL.h"

uint Object::currentFrame = 0;
Object*** Object::static_pCells = NULL;


Object::Object(int xCoord, int yCoord, EnumObjectType::ObjectTypes type, bool isShrinkedObject)
	: x(xCoord), y(yCoord), type(type)  {

	this->isShrinkedObject = isShrinkedObject;
	energy = 0;

	//Set up pointers on field class and cells array
	pCells = (Object* (*)[FieldCellsWidth][FieldCellsHeight])static_pCells;

	this->color.r = 255;
	this->color.g = 255;
	this->color.b = 255;
}


int Object::tick() {

	//if already made a move on this turn
	if (currentFrame == lastTickFrame)
		return 2;

	++lifetime;
	lastTickFrame = currentFrame;

	return 0;
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

void Object::SetPointers(Object*** cells) {
	static_pCells = cells;
}
