#include "CellCluster.h"

#include "World.h" //�� �� � ����� ����� �����������

//���������� 8 �������� ������ ��������
std::list<Cell*> CellCluster::getObjectsArround() {

    std::list<Cell*> toRet;

    for (int cx = 0; cx < areaSize; ++cx) {

        for (int cy = 0; cy < areaSize; ++cy) {

            if (cx / 2 == cy / 2 == areaSize) {
                //��������� ������ ���� �� ������
                continue;
            }

            if (area[cx][cy]->objectType == EnumObjectType::Bot) {
                toRet.push_back(area[cx][cy]);
            }
            
        }

    }
    return toRet;

}

//How may free cells are available around a given one
int CellCluster::getEmptyCellCount() {

    int toRet = 0;

    for (int cx = 0; cx < areaSize; ++cx) {

        for (int cy = 0; cy < areaSize; ++cy) {

            if (area[cx][cy]->objectType == EnumObjectType::Empty) {
                ++toRet;
            }
        }

    }

    return toRet;
}

Point CellCluster::FindFreeNeighbourCell() {

    //If this cell is empty
    if (area[0][0] == NULL) {
        return {0, 0};
    }
    
    //Form an array of nearby free cells

    Point tmpArray[areaSize * areaSize];
    int i = 0;

    for (int cx = -1; cx < 2; ++cx) {
        for (int cy = -1; cy < 2; ++cy) {

            if (area[cx][cy] == NULL) {
                tmpArray[i++].Set(cx, cy);
            }

        }
    }

    //Get random free cell from array
    if (i > 0) {
        return tmpArray[RandomVal(i)];
    }

    //No free cells nearby
    return { -1, -1 };
}




Point CellCluster::FindRandomNeighbourBot(int X, int Y) {

    //If this cell is empty
    if (area[0][0] == NULL) {
        return { 0, 0 };
    }

    //Form an array of nearby free cells

    Point tmpArray[areaSize * areaSize];
    int i = 0;

    for (int cx = -1; cx < 2; ++cx) {
        for (int cy = -1; cy < 2; ++cy) {

            if (area[cx][cy] == NULL) {
                tmpArray[i++].Set(cx, cy);
            }

        }
    }

    //Get random free cell from array
    if (i > 0) {
        return tmpArray[RandomVal(i)];
    }

    //No free cells nearby
    return { -1, -1 };
}




/**
 * ���������� ���� �������� �������� �� �����������
 * (� ������� ����� � ������� ��� �� ������� �������������� ������ � �����?)
 *
 * ����� ����� ����� ������ � ��������, ���� �������� �������� � ������� ������������
 * �������� � ������� ��������� �������� ������ �������.
 *
 * ���� ����������� ������ ������������ �������� �� World::Lock();
 *
 * 1) ``lock`` world
 * 2) ����� ������� (����� ���� � ������� ���, ��� ���������, �.� unlock �����������
 * 3) ``lock`` cluster (sync world)
 * 4) ``unlock`` world
 * 5) processing cluster
 * 6) ``unlock`` cluster (async world)
 *
 */
void CellCluster::lock() {

    for (int cx = 0; cx < areaSize; ++cx) {
        for (int cy = 0; cy < areaSize; ++cy) {


            //Collision detected - wait another thread
            while (area[cx][cy]->isLocked) {
                std::this_thread::yield();
            }

            area[cx][cy]->isLocked = true;
        }

    }
}


void CellCluster::unlock() {

    for (int cx = 0; cx < areaSize; ++cx) {

        for (int cy = 0; cy < areaSize; ++cy) {

            area[cx][cy]->isLocked = false;

        }

    }

}