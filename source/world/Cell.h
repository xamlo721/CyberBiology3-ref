#pragma once

#include "../MyTypes.h"
#include "../world/EnumCellMaterial.h"
#include "../entity/Object.h"
#include "../entity/EnumObjectType.h"
#include <shared_mutex>

//Одна клетка поля. Должна занимать в озу 8 байта, ну да ладно
class Cell {

	private:



		//Кто стоит на клетке
		EnumObjectType::ObjectTypes objectType; //Я использовал не класс, для экономии озу

		Object* object;

		//Из чего сделана сама клетка
		EnumCellMaterial::CellMaterial material; //Я использовал не класс, для экономии озу

		//Энергия на клетке
		Uint8 energy;
		std::shared_mutex isLocked;

	public:

		Cell() {
			objectType = EnumObjectType::Empty;
			material = EnumCellMaterial::common;
			energy = 0;
			object = NULL;
		}

		Cell(EnumObjectType objectType, EnumCellMaterial material, Uint8 energy, Object* object) {
			objectType = objectType;
			material = material;
			energy = energy;
			object = object;
		}

		void lock() {
			isLocked.lock();
		}

		void unlock() {
			isLocked.unlock();
		}

		//Кто стоит на клетке
		EnumObjectType::ObjectTypes getObjectType() {
			return objectType;
		}

		Object* getObjectPointer() {
			return object;
		}

		EnumCellMaterial::CellMaterial getMaterial() {
			return material;
		}

		Uint8 getEnergy() {
			return energy;
		}

		void setEmpty() {
			this->object = NULL;
			this->objectType = EnumObjectType::Empty;
		}

		void setObject(Object* obj) {
			this->object = obj;
			this->objectType = EnumObjectType::Bot;
		}

		void setWall() {
			this->object = NULL;
			this->objectType = EnumObjectType::WorldBorder;
		}

		bool isEmpty() {
			return objectType == EnumObjectType::Empty;
		}
		bool isBorder() {
			return objectType == EnumObjectType::WorldBorder;

		}
		bool isBot() {
			return objectType == EnumObjectType::Bot;
		}


};

