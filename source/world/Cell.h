#pragma once

#include "../MyTypes.h"
#include "../world/EnumCellMaterial.h"
#include "../entity/Object.h"
#include "../entity/EnumObjectType.h"


//Одна клетка поля. Должна занимать в озу 8 байта, ну да ладно
class Cell {

	public:

		//Кто стоит на клетке
		EnumObjectType::ObjectTypes objectType; //Я использовал не класс, для экономии озу

		abool isLocked;


		Object* object;

		//Из чего сделана сама клетка
		EnumCellMaterial::CellMaterial material; //Я использовал не класс, для экономии озу

		//Энергия на клетке
		Uint8 energy;


		Cell() {
			objectType = EnumObjectType::Empty;
			material = EnumCellMaterial::common;
			energy = 0;
			isLocked = false;
			object = NULL;
		}

		Cell(EnumObjectType objectType, EnumCellMaterial material, Uint8 energy, Object* object) {
			objectType = objectType;
			material = material;
			energy = energy;
			isLocked = false;
			object = object;
		}

};

