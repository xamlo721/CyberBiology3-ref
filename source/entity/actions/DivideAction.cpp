#include "DivideAction.h"

#include "../../MyTypes.h"

void DivideAction::onActivate(Bot* object, CellCluster* cluster) {

    if (object->energy <= 1 + GiveBirthCost) {
        return;
    }

    Point freeSpace = cluster->FindFreeNeighbourCell();

    if (freeSpace.x != 0 && freeSpace.y != 0) {
        object->TakeEnergy(object->energy / 2 + GiveBirthCost);
        Point clusterBootCoords = Point(0, 0);

        cluster->cell(freeSpace)->setObject(new Bot(freeSpace.x + object->x, freeSpace.y + object->y, object->energy, object, RandomPercent(10)));

        return;
    }


    if (object->energy <= 0) {
        object->isAlive = false;
        return;
    }

}


