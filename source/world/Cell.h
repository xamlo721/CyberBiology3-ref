#pragma once

#include "../MyTypes.h"
#include "../world/EnumCellMaterial.h"
#include "../entity/Object.h"
#include "../entity/EnumObjectType.h"


//���� ������ ����. ������ �������� � ��� 8 �����, �� �� �����
class Cell {

	public:

		//��� ����� �� ������
		EnumObjectType::ObjectTypes objectType; //� ����������� �� �����, ��� �������� ���

		//�� ���� ������� ���� ������
		EnumCellMaterial::CellMaterial material; //� ����������� �� �����, ��� �������� ���

		//������� �� ������
		Uint8 energy;

		abool isLocked;

		Object* object;

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

