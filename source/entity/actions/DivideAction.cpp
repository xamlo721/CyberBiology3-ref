#include "DivideAction.h"

#include "../../MyTypes.h"

void DivideAction::onActivate(Bot* object, CellCluster* cluster) {

    if (object->energy <= 1 + GiveBirthCost) {
        return;
    }

    Point freeSpace = cluster->FindFreeNeighbourCell();

    if (freeSpace.x != -1) {
        object->TakeEnergy(object->energy / 2 + GiveBirthCost);

        cluster->area[freeSpace.x][freeSpace.y]->object = new Bot(freeSpace.x + object->x, freeSpace.y + object->y, object->energy, object, RandomPercent(10));

        return;
    }


    if (object->energy <= 0) {
        object->isAlive = false;
        return;
    }

}


