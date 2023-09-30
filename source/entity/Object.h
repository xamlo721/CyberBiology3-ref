#pragma once
//#pragma message("	Object_h")

#include "../Settings.h"
#include "../MyTypes.h"
#include "EnumObjectType.h"

#include "../renderer/IRenderableObject.h"


//Base class for any object on field
//TODO: Он не должен быть наследован оттуда
class Object : public virtual IRenderableObject {

	private:

		//Prev. tick frame number
		uint lastTickFrame = 0;

		//Static pointers to field class and cells array
		static Object*** static_pCells;

	protected:

		//Time in ticks since object was created
		uint lifetime = 0;


		//Pointers to Field class and cells array
		Object* (*pCells)[FieldCellsWidth][FieldCellsHeight];

	public:

		//X coordinate, corrected with Field::RenderX
		int screenX;

		int x, y;

		//If an object stores energy it's here
		int energy;

		EnumObjectType::ObjectTypes type;



		Object(int xCoord, int yCoord, EnumObjectType::ObjectTypes type, bool isShrinkedObject);

		/*This function returns 1 when the object is destroyed.
		You should call it on every simulation tick before you
		call same function in derived class
		Returns:
		0 - all fine
		1 - object destroyed
		2 - nothing to do(last tick frame matches current frame)*/
		virtual int tick();	

		uint GetLifetime();
		void SetLifetime(uint);

		virtual Color GetColor() override;


		static uint currentFrame;

		static void SetPointers(Object*** cells);


};