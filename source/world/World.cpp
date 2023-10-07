#include "World.h"

uint World::seed;

//SINGLETON
World* World::instance = 0;

World::World() {

    this->generateWorldBorder();
}

void World::lockMap() {
    //auto lck = std::lock_guard{ clusterMutex };

    //World thread sync
    while (isLocked) {
        Sleep(1);
        std::this_thread::yield();
    }

    this->isLocked = true;
}

void World::unlockMap() {
    //auto lck = std::lock_guard{ clusterMutex };

    this->isLocked = false;
}

void World::generateWorldBorder() {
    auto lck = std::lock_guard{ clusterMutex };

    //Горизонтальные стены
    for (int i = 0; i < FieldCellsWidth; i++) {
        world.setWall(i, 0);
        world.setWall(i, FieldCellsHeight - 1);
    }

    //Вертикальные стены 
    for (int i = 0; i < FieldCellsHeight; i++) {
        world.setWall(0, i);
        world.setWall(FieldCellsWidth - 1, i);
    }

}


bool World::addObjectSafetly(Object* obj) {
    auto lck = std::lock_guard{ clusterMutex };

    this->lockMap();

    world.lockCell(obj->x, obj->y);

    this->unlockMap();

    //processing
    bool returnValue = world.addObjectUnsafetly(obj);

    world.unlockCell(obj->x, obj->y);

    return returnValue;
}


void World::removeObjectSafetly(int X, int Y) {
    auto lck = std::lock_guard{ clusterMutex };

    this->lockMap();

    world.lockCell(X, Y);

    this->unlockMap();

    {//processing
        world.removeObjectUnsafetly(X, Y);

    }

    world.unlockCell(X, Y);


}

bool World::moveObject(Object* obj, int toX, int toY) {
    auto lck = std::lock_guard{ clusterMutex };

    if (!IsInBounds(toX, toY)) {
        return false;
    }

    this->lockMap();

    world.lockCell(toX, toY);


    if (!world.isEmpty(toX, toY)) {
        this->unlockMap();
        world.unlockCell(toX, toY);
        return false;
    }
    
    world.lockCell(obj->x, obj->y);


    this->unlockMap();


    //Processing
    {
        Object* tmpObj = world.getObject(obj->x, obj->y);

        //Пометить, что теперь там нет бота
        world.setEmpty(obj->x, obj->y);

        //Пометить, что теперь там бот
        world.setObject(toX, toY, tmpObj);

        tmpObj->x = toX;
        tmpObj->y = toY;

    }

    world.unlockCell(toX, toY);    
    world.unlockCell(obj->x, obj->y);


    return true;
}

//Swap entityes and tempEntityes
void World::startStep() {
    auto lck = std::lock_guard{ clusterMutex };

    if (isProcessing) {
        return;
    }
    isProcessing = true;


    //this->lockMap();
    //logic
    //this->unlockMap();


}

void World::stopStep() {
    auto lck = std::lock_guard{ clusterMutex };

    if (!isProcessing) {
        return;
    }
    isProcessing = false;

    this->lockMap();

    copyList.clear();

    this->unlockMap();

}

CellCluster* World::getLockedCluster(Object* obj) {
    auto lck = std::lock_guard{ clusterMutex };

    if (obj->x == (FieldCellsWidth) || obj->y == (FieldCellsHeight)) {
        int c;
        c = 0;
    }

    this->lockMap();

    CellCluster * cluster = new CellCluster();
    //-1 to +1
    for (int nI = 0, i = - 1; i <= 1;  nI++, i++) {

        //-1 to +1
        for (int nJ = 0, j = - 1; j <= 1; nJ++, j++) {

            int validateXCoord = (obj->x + i) % (FieldCellsWidth);
            int validateYCoord = (obj->y + j) % (FieldCellsHeight);
            Cell* pCell = world.getCellPointer(validateXCoord, validateYCoord);
            cluster->area[nI][nJ] = pCell;
        }

    }
    cluster->lock();

    this->unlockMap();

    return cluster;
}

void World::RemoveAllObjects() {
    auto lck = std::lock_guard{ clusterMutex };

    for (int cx = 0; cx < FieldCellsWidth; ++cx) {
        for (int cy = 0; cy < FieldCellsHeight; ++cy) {
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
    auto lck = std::lock_guard{ clusterMutex };
    return world.getObject(X, Y);
}




uint World::GetNumObjects() {
    return FieldCellsWidth * FieldCellsHeight;
}

uint World::GetNumBots() {
    return copyList.size();
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

            if (world.getObject(ix, iy) == obj) {
                return true;
            }

        }
    
    }

    return false;
}

std::vector<Object*> World::getObjectsForRenderer() {
    auto lck = std::lock_guard{ clusterMutex };

    copyList.clear();

    for (uint ix = 0; ix < FieldCellsWidth; ++ix) {

        for (uint iy = 0; iy < FieldCellsHeight; ++iy) {

            if (world.isBot(ix, iy)) {
                copyList.push_back(world.getObject(ix, iy));
            }

        }

    }

    return copyList;
}
