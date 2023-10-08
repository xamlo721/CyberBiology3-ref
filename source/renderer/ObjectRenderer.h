#pragma once

#include "../entity/Object.h"
#include "../world/Cell.h"
#include "EnumRenderTypes.h"
#include "objectRenderes/IObjectRenderer.h"

class ObjectRenderer {

	protected:



	public:

		static void drawObjects(std::vector<Cell*> entityes, IObjectRenderer * worker);
		
		static void drawOne(Cell* object, IObjectRenderer* worker);

};

