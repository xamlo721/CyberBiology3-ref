#include "MoveAction.h"

#include "../../world/World.h"

void MoveAction::onActivate(Bot* object) {


    if (object->TakeEnergy(MoveCost)) {
        //return 1;
        return;
    }
    //Place object in a new place
    int tmpY = object->y;

    if (World::INSTANCE()->MoveObject(object, object->lookAt_x, object->lookAt_y) == 0)
    {
        if (object->lookAt_y != tmpY)
            ++object->numMovesY;
    }


}