#include "World.h"

uint World::seed;

//SINGLETON
World* World::instance = 0;

World::World() {
    this->generateWorldBorder();
}

inline void World::lockMap() {
    mapMutex.lock();
}

inline void World::unlockMap() {
    mapMutex.unlock();
}

void World::generateWorldBorder() {
    auto lck = std::scoped_lock{ clusterMutex };

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
    auto lck = std::scoped_lock{ clusterMutex };

    this->lockMap();

    world.lockCell(obj->x, obj->y);

    this->unlockMap();
    
    //processing
    bool returnValue = world.addObjectUnsafetly(obj);

    world.unlockCell(obj->x, obj->y);

    return returnValue;
}


void World::removeObjectSafetly(int X, int Y) {
    auto lck = std::scoped_lock{ clusterMutex };

    this->lockMap();

    world.lockCell(X, Y);

    this->unlockMap();

    {//processing
        world.removeObjectUnsafetly(X, Y);

    }

    world.unlockCell(X, Y);


}

bool World::moveObject(Object* obj, int toX, int toY) {
    auto lck = std::scoped_lock{ clusterMutex };

    if (!IsInBounds(toX, toY)) {
        return false;
    }

    this->lockMap();

    world.lockCell(toX, toY);


    if (!world.getCellPointer(toX, toY)->isEmpty()) {
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
    auto lck = std::scoped_lock{ clusterMutex };




    this->lockMap();

    {//check deads

        for (int i = 0; i < FieldCellsWidth; i++) {
            for (int j = 0; j < FieldCellsHeight; j++) {
                world.lockCell(i, j);
                Object* obj = world.getObject(i, j);
                if (world.getCellPointer(i, j)->isBot() && obj->isAlive == false) {
                    world.setEmpty(i, j);
                    delete obj;

                }
                world.unlockCell(i, j);

            }
        }
    }
    copyList.clear();


    this->unlockMap();


}

CellCluster* World::getLockedCluster(Object* obj) {
    //auto lck = std::scoped_lock{ clusterMutex };


    Cell* clusterArea[areaSize][areaSize];

    //-1 to +1
    for (int nI = 0, i = - 1; i <= 1;  nI++, i++) {

        //-1 to +1
        for (int nJ = 0, j = - 1; j <= 1; nJ++, j++) {

            //int validateXCoord = (obj->x + i) % (FieldCellsWidth);
            //int validateYCoord = (obj->y + j) % (FieldCellsHeight);
            //Cell* pCell = world.getCellPointer(validateXCoord, validateYCoord);
            
            Cell* pCell = world.getCellPointer((obj->x + i), (obj->y + j));

            clusterArea[nI][nJ] = pCell;
        }

    }
    this->lockMap();
    CellCluster* cluster = new CellCluster(clusterArea);
    this->unlockMap();

    return cluster;
}



void World::RemoveAllObjects() {

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
    //auto lck = std::scoped_lock{ clusterMutex };
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

std::vector<Cell*> World::getObjectsForRenderer() {
    auto lck = std::scoped_lock{ clusterMutex };

    copyList.clear();

    for (uint ix = 0; ix < FieldCellsWidth; ++ix) {

        for (uint iy = 0; iy < FieldCellsHeight; ++iy) {

            if (world.getCellPointer(ix, iy)->isBot()) {
                copyList.push_back(world.getCellPointer(ix, iy));
            }

        }

    }

    return copyList;
}
