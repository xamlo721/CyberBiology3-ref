#pragma once

#include "Cell.h"
#include "../entity/Object.h"

#include <list>

constexpr int visibleDistance = 1;
constexpr int areaSize = ((visibleDistance * 2) + 1);//Обязательно должно быть не чётным


class CellCluster {

	public :

		Cell* area[areaSize][areaSize];

	public:

		//Возвращает 8 объектов вокруг искомого
		std::list<Cell*> getObjectsArround();

		//How may free cells are available around a given one
		int getEmptyCellCount();

		//Find empty cell nearby, otherwise return {-1, -1}
		Point FindFreeNeighbourCell();

		Point FindRandomNeighbourBot(int X, int Y);



		//multithreding

		/**
		 * Блокировка всех объектов кластера от модификации
		 * (Я надеюсь никто в здравом уме не полезет модифицировать массив в обход?)
		 * 
		 * Здесь поток может встать в ожидание, если случится коллизия и ожидать освобождения
		 * кластера с которым случилась коллизия другим потоком.
		 * 
		 * Этот блокировщик НЕЛЬЗЯ использовать отдельно от World::Lock();
		 * 
		 * 1) ``lock`` world
		 * 2) Взять кластер (может уйти в синхрон тут, это нормально, т.к unlock асинхронный
         * 3) ``lock`` cluster
         * 4) ``unlock`` world
         * 5) processing cluster
         * 6) ``unlock`` cluster (async world)
		 * 
		 */
		void lock();

		void unlock();

};

