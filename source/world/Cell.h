#pragma once

#include "../MyTypes.h"
#include "../world/EnumCellMaterial.h"
#include "../entity/Object.h"
#include "../entity/EnumObjectType.h"
#include <shared_mutex>

//���� ������ ����. ������ �������� � ��� 8 �����, �� �� �����
class Cell {

	private:



		//��� ����� �� ������
		EnumObjectType::ObjectTypes objectType; //� ����������� �� �����, ��� �������� ���

		Object* object;

		//�� ���� ������� ���� ������
		EnumCellMaterial::CellMaterial material; //� ����������� �� �����, ��� �������� ���

		//������� �� ������
		Uint8 energy;
		abool isLocked;

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

		inline void lock() {
			while (isLocked) {
				Sleep(1);
				std::this_thread::yield();
			}
			isLocked = true;
		}

		inline void unlock() {
			isLocked = false;
		}

		//��� ����� �� ������
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

