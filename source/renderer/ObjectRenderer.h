#pragma once

#include "../entity/Object.h"
#include "EnumRenderTypes.h"
#include "objectRenderes/IObjectRenderer.h"

class ObjectRenderer {

	protected:



	public:

		static void drawObjects(std::list<Object*> entityes, IObjectRenderer * worker);
		
		static void drawOne(Object* object, IObjectRenderer* worker);

};

