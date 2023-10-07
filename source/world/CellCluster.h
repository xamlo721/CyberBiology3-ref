#pragma once

#include "Cell.h"
#include "../entity/Object.h"

#include <list>
#include <mutex>
#include <shared_mutex>

constexpr int visibleDistance = 1;
constexpr int areaSize = ((visibleDistance * 2) + 1);//����������� ������ ���� �� ������


class CellCluster {

	public :

		Cell* area[areaSize][areaSize];
		std::shared_mutex mutex;

	public:

		//���������� 8 �������� ������ ��������
		std::list<Cell*> getObjectsArround();

		//How may free cells are available around a given one
		int getEmptyCellCount();

		//Find empty cell nearby, otherwise return {-1, -1}
		Point FindFreeNeighbourCell();

		Point FindRandomNeighbourBot(int X, int Y);


		CellCluster();
		~CellCluster();
		//multithreding

		/**
		 * ���������� ���� �������� �������� �� �����������
		 * (� ������� ����� � ������� ��� �� ������� �������������� ������ � �����?)
		 * 
		 * ����� ����� ����� ������ � ��������, ���� �������� �������� � ������� ������������
		 * �������� � ������� ��������� �������� ������ �������.
		 * 
		 * ���� ����������� ������ ������������ �������� �� World::lockMap();
		 * 
		 * 1) ``lock`` world
		 * 2) ����� ������� (����� ���� � ������� ���, ��� ���������, �.� unlockMap �����������
         * 3) ``lock`` cluster
         * 4) ``unlock`` world
         * 5) processing cluster
         * 6) ``unlock`` cluster (async world)
		 * 
		 */
		void lock();

		void unlock();

};

