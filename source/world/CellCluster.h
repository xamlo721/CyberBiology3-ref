#pragma once

#include "Cell.h"
#include "../entity/Object.h"

#include <list>
#include <mutex>
#include <shared_mutex>

constexpr int visibleDistance = 1;
constexpr int areaSize = ((visibleDistance * 2) + 1);//Обязательно должно быть не чётным


class CellCluster {

	private:
		Cell* area[areaSize][areaSize];
		std::shared_mutex mutex;

	public:
		CellCluster();

		~CellCluster();

		void lock(Cell* area[areaSize][areaSize]);

		void unlock();

		Cell* cell(int localXCoord, int localYCoord) {
			int clusterXCoord = localXCoord + visibleDistance;
			int clusterYCoord = localYCoord + visibleDistance;
			return area[clusterXCoord][clusterYCoord];
		}
		Cell* cell(Point localCoord) {
			int clusterXCoord = localCoord.x + visibleDistance;
			int clusterYCoord = localCoord.y + visibleDistance;
			return area[clusterXCoord][clusterYCoord];
		}
		//Возвращает 8 объектов вокруг искомого
		std::list<Cell*> getObjectsArround();

		//How may free cells are available around a given one
		int getEmptyCellCount();

		//Find empty cell nearby, otherwise return {0, 0}
		Point FindFreeNeighbourCell();

		Point FindRandomNeighbourBot(int X, int Y);

		Cell* getCellByLocalCoord(int localXCoord, int localYCoord);



};

