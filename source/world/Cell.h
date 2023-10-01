#pragma once


#include "../entity/EnumObjectType.h"
#include "../MyTypes.h"
#include "../world/EnumCellMaterial.h"

class Cell {

	public:

		//��� ����� �� ������
		EnumObjectType objectType;

		//�� ���� ������� ���� ������
		EnumCellMaterial material;

		//������� �� ������
		Uint16 energy;

};

