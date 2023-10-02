#pragma once


#include "../entity/EnumObjectType.h"
#include "../MyTypes.h"
#include "../world/EnumCellMaterial.h"


//Одна клетка поля. Должна занимать в озу 4 байта, ну да ладно
class Cell {

	public:

		//Кто стоит на клетке
		EnumObjectType::ObjectTypes objectType; //Я использовал не класс, для экономии озу

		//Из чего сделана сама клетка
		EnumCellMaterial::CellMaterial material; //Я использовал не класс, для экономии озу

		//Энергия на клетке
		Uint8 energy;

		abool isLocked;

		Cell() {
			objectType = EnumObjectType::Empty;
			material = EnumCellMaterial::common;
			energy = 0;
			isLocked = false;
		}

		Cell(EnumObjectType objectType, EnumCellMaterial material, Uint8 energy) {

			objectType = objectType;
			material = material;
			energy = energy;
			isLocked = false;
		}

};

