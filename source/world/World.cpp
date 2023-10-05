#include "World.h"

uint World::seed;

//SINGLETON
World* World::instance = 0;

World::World() {

    this->generateWorldBorder();
}


void World::generateWorldBorder() {
    auto lck = std::lock_guard{ world.worldMutex };
    //Горизонтальные стены
    for (int i = 0; i < FieldCellsWidth; i++) {
        world.worldMap[i][0].objectType = EnumObjectType::WorldBorder;
        world.worldMap[i][FieldCellsHeight - 1].objectType = EnumObjectType::WorldBorder;
    }

    //Вертикальные стены 
    for (int i = 0; i < FieldCellsHeight; i++) {
        world.worldMap[0][i].objectType = EnumObjectType::WorldBorder;
        world.worldMap[FieldCellsWidth - 1][i].objectType = EnumObjectType::WorldBorder;
    }

}


bool World::addObjectSafetly(Object* obj) {
    auto lck = std::lock_guard{ world.worldMutex };

    //World thread sync
    while (isLocked) {
        std::this_thread::yield();
    } 

    //1) WORLD LOCK
    isLocked = true;

    //Cell thread sync
    while (world.worldMap[obj->x][obj->y].isLocked) {
        std::this_thread::yield();
    }

    //2) CELL LOCK
    world.worldMap[obj->x][obj->y].isLocked = true;

    //3) World UNLOCK
    isLocked = false;

    bool returnValue = world.addObjectUnsafetly(obj);
    
    //4) CELL UNLOCK
    world.worldMap[obj->x][obj->y].isLocked = false;

    return returnValue;
}


void World::removeObjectSafetly(int X, int Y) {
    auto lck = std::lock_guard{ world.worldMutex };

    {//World thread sync
        while (isLocked) {
            std::this_thread::yield();
        }

        //1) WORLD LOCK
        isLocked = true;
    }

    {//Cell thread sync
        while (world.worldMap[X][Y].isLocked) {
            std::this_thread::yield();
        }
        //2) CELL LOCK
        world.worldMap[X][Y].isLocked = true;
    }

    //3) World UNLOCK
    isLocked = false;

    {//processing
        world.removeObjectUnsafetly(X, Y);

    }


    //4) CELL UNLOCK
    world.worldMap[X][X].isLocked = false;

}

bool World::moveObject(Object* obj, int toX, int toY) {
    auto lck = std::lock_guard{ world.worldMutex };


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
        while (world.worldMap[obj->x][obj->y].isLocked) {
            std::this_thread::yield();
        }
        //2) CELL LOCK
        world.worldMap[toX][toY].isLocked = true;
    }



    if (world.worldMap[toX][toY].objectType != EnumObjectType::Empty) {
        isLocked = false;
        world.worldMap[toX][toY].isLocked = false;
        return false;
    }
    

    {//Cell thread sync
        while (world.worldMap[obj->x][obj->y].isLocked) {
            std::this_thread::yield();
        }

        //2) CELL LOCK
        world.worldMap[obj->x][obj->y].isLocked = true;
    }


    //3) World UNLOCK
    isLocked = false;

    //Processing
    {
        Object* tmpObj = world.worldMap[obj->x][obj->y].object;

        //Пометить, что теперь там нет бота
        world.worldMap[obj->x][obj->y].objectType = EnumObjectType::Empty;
        world.worldMap[obj->x][obj->y].object = NULL;

        //Пометить, что теперь там бот
        world.worldMap[toX][toY].objectType = EnumObjectType::Bot;
        world.worldMap[toX][toY].object = tmpObj;

        tmpObj->x = toX;
        tmpObj->y = toY;

    }

    //4) CELL UNLOCK
    world.worldMap[obj->x][obj->y].isLocked = false;
    world.worldMap[toX][toY].isLocked = false;

    return true;
}

//Swap entityes and tempEntityes
void World::startStep() {
    auto lck = std::lock_guard{ world.worldMutex };

    if (isProcessing) {
        return;
    }
    isProcessing = true;


    {//World thread sync
        while (isLocked) {
            std::this_thread::yield();
        }

        //1) WORLD LOCK
        isLocked = true;
    }
    //world.worldMutex.lock();

    //world.entityes.clear();//just for my paranoia

    //for (Object* obj : world.tempEntityes) {
    //    world.entityes.push_back(obj);
    //}
    //world.worldMutex.unlock();

    //3) World UNLOCK
    isLocked = false;

}

Object* World::getNextUnprocessedObject() {
    auto lck = std::lock_guard{ world.worldMutex };

    if (world.entityes.empty()) {
        return NULL;
    }

    Object* returnValue = world.entityes.getRandomItem();

    Color c = returnValue->GetColor();

    world.tempEntityes.push_back(returnValue);

    return returnValue;
}

bool World::hasUnprocessedObject() {
    return !(world.entityes.empty());
}

void World::stopStep() {
    auto lck = std::lock_guard{ world.worldMutex };

    if (!isProcessing) {
        return;
    }
    isProcessing = false;

    {//World thread sync
        while (isLocked) {
            std::this_thread::yield();
        }

        //1) WORLD LOCK
        isLocked = true;
    }

    //this->world.entityes.moveTo(world.tempEntityes);
    this->world.tempEntityes.moveTo(world.entityes);


    //3) World UNLOCK
    isLocked = false;

}

CellCluster* World::getObjectsArround(Object* obj) {
    auto lck = std::lock_guard{ world.worldMutex };

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

            cluster->area[nI][nJ] = &world.worldMap[obj->x + i][obj->y + j];
            //Color c = world.worldMap[obj->x + i][obj->y + j].object->GetColor();

        }

    }


    //3) World UNLOCK
    isLocked = false;

    return cluster;
}

void World::updateCluster(CellCluster* cluster) {
    auto lck = std::lock_guard{ world.worldMutex };

    for (int i = 0; i < areaSize; i++) {
        for (int j = 0; j < areaSize; j++) {

            Cell * updatedCell = cluster->area[i][j];
            //if (updatedCell->objectType == EnumObjectType::Bot) {
            //    world.tempEntityes.push_back(updatedCell->object);

            //}

        }
    }

}


void World::RemoveAllObjects() {
    auto lck = std::lock_guard{ world.worldMutex };


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
        auto lck = std::lock_guard{ world.worldMutex };

    return world.worldMap[X][Y].object;
}




uint World::GetNumObjects() {
    return world.entityes.size();
}

uint World::GetNumBots() {
    return world.tempEntityes.size();
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

            if (world.worldMap[ix][iy].object == obj) {
                return true;
            }

        }
    
    }

    return false;
}

std::vector<Object*> World::getObjectsForRenderer() {
    auto lck = std::lock_guard{ world.worldMutex };

    std::vector<Object*> copyList;


    for (Object* o : world.entityes) {
        Color c = o->GetColor();
        copyList.push_back(o);
    }


    return copyList;
}
