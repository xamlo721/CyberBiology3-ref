#pragma once


#include "../entity/EnumObjectType.h"
#include "../MyTypes.h"
#include "../world/EnumCellMaterial.h"


//���� ������ ����. ������ �������� � ��� 4 �����, �� �� �����
class Cell {

	public:

		//��� ����� �� ������
		EnumObjectType::ObjectTypes objectType; //� ����������� �� �����, ��� �������� ���

		//�� ���� ������� ���� ������
		EnumCellMaterial::CellMaterial material; //� ����������� �� �����, ��� �������� ���

		//������� �� ������
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

