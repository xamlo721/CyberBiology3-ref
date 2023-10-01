#pragma once


#include "../entity/EnumObjectType.h"
#include "../MyTypes.h"
#include "../world/EnumCellMaterial.h"

class Cell {

	public:

		//Кто стоит на клетке
		EnumObjectType objectType;

		//Из чего сделана сама клетка
		EnumCellMaterial material;

		//Энергия на клетке
		Uint16 energy;

};

