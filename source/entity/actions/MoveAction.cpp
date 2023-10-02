#include "MoveAction.h"

#include "../../world/World.h"

void MoveAction::onActivate(Bot* object, CellCluster* cluster) {

    if (!object->isAlive) {
        return;
    }

    if (object->TakeEnergy(MoveCost)) {
        object->isAlive = false;
        return;
    }
    //Place object in a new place
    int tmpY = object->y;

    if (World::INSTANCE()->moveObject(object, object->lookAt_x, object->lookAt_y) == 0)
    {
        if (object->lookAt_y != tmpY)
            ++object->numMovesY;
    }


}
