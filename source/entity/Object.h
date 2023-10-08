#pragma once
//#pragma message("	Object_h")

#include "../Settings.h"
#include "../MyTypes.h"
#include "../renderer/IRenderableObject.h"
#include "../logic/ITickable.h"

#include "EnumObjectType.h"


//Base class for any object on field
//TODO: Он не должен быть наследован оттуда
class Object : public virtual IRenderableObject, public ITickable {

	public: //protected:

		//Time in ticks since object was created
		uint lifetime = 0;

		//X coordinate, corrected with Field::RenderX
		int screenX;

		int x, y;

		//If an object stores energy it's here
		int energy;

		EnumObjectType::ObjectTypes type;

		long long lastUpdatedTick = 0;


		//Маркер удаления
		bool isAlive;

	public:

		Object(int xCoord, int yCoord, EnumObjectType::ObjectTypes type);

		uint GetLifetime();

		void SetLifetime(uint);

		virtual Color GetColor() override;

		virtual void tick() override {}; //ITickable


};