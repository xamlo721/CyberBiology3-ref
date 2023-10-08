#pragma once

#include "Cell.h"
#include "../entity/Object.h"

#include <list>
#include <mutex>
#include <shared_mutex>

constexpr int visibleDistance = 1;
constexpr int areaSize = ((visibleDistance * 2) + 1);//����������� ������ ���� �� ������


class CellCluster {

	public:
		Cell* area[areaSize][areaSize];
		std::shared_mutex mutex;

	public:
		CellCluster(Cell* area[areaSize][areaSize]);
		~CellCluster();

		//���������� 8 �������� ������ ��������
		std::list<Cell*> getObjectsArround();

		//How may free cells are available around a given one
		int getEmptyCellCount();

		//Find empty cell nearby, otherwise return {-1, -1}
		Point FindFreeNeighbourCell();

		Point FindRandomNeighbourBot(int X, int Y);

		Cell* getCellByLocalCoord(int localXCoord, int localYCoord);



};

