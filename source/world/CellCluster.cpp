#include "CellCluster.h"

CellCluster::CellCluster(Cell* area[areaSize][areaSize]) {

    for (int cx = 0; cx < areaSize; ++cx) {
        for (int cy = 0; cy < areaSize; ++cy) {
            this->area[cx][cy] = area[cx][cy];
            this->area[cx][cy]->lock(); //Collision detected - wait another thread
        }
    }
}

CellCluster::~CellCluster() {

    for (int cx = 0; cx < areaSize; ++cx) {
        for (int cy = 0; cy < areaSize; ++cy) {
            area[cx][cy]->unlock();
        }
    }

}


//���������� 8 �������� ������ ��������
std::list<Cell*> CellCluster::getObjectsArround() {

    std::list<Cell*> toRet;

    for (int cx = 0; cx < areaSize; ++cx) {

        for (int cy = 0; cy < areaSize; ++cy) {

            if (cx / 2 == cy / 2 == areaSize) {
                //��������� ������ ���� �� ������
                continue;
            }

            if (area[cx][cy]->isBot()) {
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

            if (area[cx][cy]->isEmpty()) {
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

    for (int cx = -visibleDistance, arrayX = 0; cx < 2 * visibleDistance; cx++, arrayX++) {
        for (int cy = -visibleDistance, arrayY = 0; cy < 2 * visibleDistance; cy++, arrayY++) {

            if (area[arrayX][arrayY]->isEmpty()) {
                tmpArray[i++].Set(cx, cy);
            }

        }
    }

    //Get random free cell from array
    if (i > 0) {
        return tmpArray[RandomVal(i)];
    }

    //No free cells nearby
    return { 0, 0 };
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

            if (area[cx][cy]->isEmpty()) {
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

Cell* CellCluster::getCellByLocalCoord(int localXCoord, int localYCoord) {
    int validateXCoord = localXCoord + visibleDistance;
    int validateYCoord = localYCoord + visibleDistance;

    return this->area[validateXCoord][validateYCoord];
}