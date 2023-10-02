#include "World.h"

uint World::seed;

//SINGLETON
World* World::instance = 0;

World::World() {

    //Clear array
    memset(worldEntityMap, NULL, sizeof(Cell*) * FieldCellsWidth * FieldCellsHeight);

    this->generateWorldBorder();
}

void World::generateWorldBorder() {

    //Горизонтальные стены
    for (int i = 0; i < FieldCellsWidth; i++) {
        worldMap[i][0].objectType = EnumObjectType::WorldBorder;
        worldMap[i][FieldCellsHeight - 1].objectType = EnumObjectType::WorldBorder;
    }

    //Вертикальные стены 
    for (int i = 0; i < FieldCellsHeight; i++) {
        worldMap[0][i].objectType = EnumObjectType::WorldBorder;
        worldMap[FieldCellsWidth - 1][i].objectType = EnumObjectType::WorldBorder;
    }
}


bool World::addObjectSafetly(Object* obj) {

    //World thread sync
    while (isLocked) {
        std::this_thread::yield();
    } 

    //1) WORLD LOCK
    isLocked = true;

    //Cell thread sync
    while (worldMap[obj->x][obj->y].isLocked) {
        std::this_thread::yield();
    }

    //2) CELL LOCK
    worldMap[obj->x][obj->y].isLocked = true;

    //3) World UNLOCK
    isLocked = false;

    bool returnValue = this->addObjectUnsafetly(obj);

    //4) CELL UNLOCK
    worldMap[obj->x][obj->y].isLocked = false;

    return returnValue;
}

bool World::addObjectUnsafetly(Object* obj) {
    //Если там занято, о чём речь?
    if (worldMap[obj->x][obj->y].objectType != EnumObjectType::Empty) {
        return false;
    }

    //Пометить, что теперь там бот
    worldMap[obj->x][obj->y].objectType = EnumObjectType::Bot;

    if (obj->type == EnumObjectType::Bot) {
        worldEntityMap[obj->x][obj->y] = (Bot*)obj; //unsafe cast
        tempEntityes.push_back((Bot*)obj);
    }
}

void World::removeObjectSafetly(int X, int Y) {

    {//World thread sync
        while (isLocked) {
            std::this_thread::yield();
        }

        //1) WORLD LOCK
        isLocked = true;
    }

    {//Cell thread sync
        while (worldMap[X][Y].isLocked) {
            std::this_thread::yield();
        }
        //2) CELL LOCK
        worldMap[X][Y].isLocked = true;
    }

    //3) World UNLOCK
    isLocked = false;

    {//processing
        this->removeObjectUnsafetly(X, Y);

    }


    //4) CELL UNLOCK
    worldMap[X][X].isLocked = false;

}
#include <iostream>

void World::removeObjectUnsafetly(int X, int Y) {
    if (worldMap[X][Y].objectType != EnumObjectType::Empty) {
        Object* tmpO = worldEntityMap[X][Y];


        entityes.remove(tmpO);
        tempEntityes.remove(tmpO);

        delete tmpO;

        worldMap[X][Y].objectType = EnumObjectType::Empty;
        worldEntityMap[X][Y] = NULL;
        std::cout << "delete object X " << X << ", Y " << Y;
    }
}

bool World::moveObject(Object* obj, int toX, int toY) {


    {//World thread sync
        while (isLocked) {
            std::this_thread::yield();
        }

        //1) WORLD LOCK
        isLocked = true;
    }



    if (!IsInBounds(toX, toY)) {
        isLocked = false;
        return false;
    }

    {//Cell thread sync
        while (worldMap[obj->x][obj->y].isLocked) {
            std::this_thread::yield();
        }
        //2) CELL LOCK
        worldMap[toX][toY].isLocked = true;
    }



    if (worldMap[toX][toY].objectType != EnumObjectType::Empty) {
        isLocked = false;
        worldMap[toX][toY].isLocked = false;
        return false;
    }
    

    {//Cell thread sync
        while (worldMap[obj->x][obj->y].isLocked) {
            std::this_thread::yield();
        }

        //2) CELL LOCK
        worldMap[obj->x][obj->y].isLocked = true;
    }


    //3) World UNLOCK
    isLocked = false;

    //Processing
    {
        Object* tmpObj = worldEntityMap[obj->x][obj->y];

        //Пометить, что теперь там нет бота
        worldMap[obj->x][obj->y].objectType = EnumObjectType::Empty;
        worldEntityMap[obj->x][obj->y] = NULL;

        //Пометить, что теперь там бот
        worldMap[toX][toY].objectType = EnumObjectType::Bot;
        worldEntityMap[toX][toY] = tmpObj;

        tmpObj->x = toX;
        tmpObj->y = toY;

    }

    //4) CELL UNLOCK
    worldMap[obj->x][obj->y].isLocked = false;
    worldMap[toX][toY].isLocked = false;

    return true;
}

CellCluster* World::getObjectsArround(Object* obj) {

    {//World thread sync
        while (isLocked) {
            std::this_thread::yield();
        }

        //1) WORLD LOCK
        isLocked = true;
    }

    CellCluster* cluster = new CellCluster();
    //-1 to +1
    for (int nI = 0, i = - visibleDistance; i <= visibleDistance;  nI++, i++) {

        //-1 to +1
        for (int nJ = 0, j = - visibleDistance; j <= visibleDistance; nJ++, j++) {

            cluster->area[nI][nJ] = &worldMap[obj->x + i][obj->y + j];
            cluster->objects[nI][nJ] = worldEntityMap[obj->x + i][obj->y + j];

        }

    }


    //3) World UNLOCK
    isLocked = false;

    return cluster;
}


void World::RemoveAllObjects() {

    for (int cx = 0; cx < FieldCellsWidth; ++cx)
    {
        for (int cy = 0; cy < FieldCellsHeight; ++cy)
        {
            removeObjectSafetly(cx, cy);
        }
    }

}

//Is cell out if bounds?
bool World::IsInBounds(int X, int Y) {
    return ((X >= 0) && (Y >= 0) && (X < FieldCellsWidth) && (Y < FieldCellsHeight));
}

bool World::IsInBounds(Point p) {
    return IsInBounds(p.x, p.y);
}

bool World::IsInWater(int Y) {
    return (Y >= (FieldCellsHeight - params.oceanLevel));
}

bool World::IsInMud(int Y) {
    return (Y >= (FieldCellsHeight - params.mudLevel));
}


Object* World::GetObjectLocalCoords(int X, int Y) {
    return worldEntityMap[X][Y];
}




uint World::GetNumObjects() {
    return objectsTotal;
}

uint World::GetNumBots() {
    return botsTotal;
}





int World::ValidateX(int X) {

    if (X < 0) {
        return X + FieldCellsWidth;
    }
    
    if (X >= FieldCellsWidth) {
        return (X - FieldCellsWidth);
    }

    return X;
}

//Что это за штука для gui и почему она тут?
bool World::ValidateObjectExistance(Object* obj) {

    for (uint ix = 0; ix < FieldCellsWidth; ++ix) {

        for (uint iy = 0; iy < FieldCellsHeight; ++iy) {

            if (worldEntityMap[ix][iy] == obj) {
                return true;
            }

        }
    
    }

    return false;
}

