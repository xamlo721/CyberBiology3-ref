#include "MoveAction.h"

#include "../../world/World.h"

void MoveAction::onActivate(Bot* object, CellCluster* cluster) {

    if (object->TakeEnergy(MoveCost)) {
        object->isAlive = false;
        return;
    }
    //Place object in a new place
    int tmpY = object->y;

    World::INSTANCE()->moveObject(object, object->lookAt.x, object->lookAt.y);


}
