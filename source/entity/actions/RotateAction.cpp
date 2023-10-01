#include "RotateAction.h"


void RotateAction::onActivate(Bot* object) {


    //If dies of low energy
    if (object->TakeEnergy(RotateCost)) {
        //return 1;
        return;
    }
    
    //int direction = (tmpOut.desired_rotation * 10.0f);
    int direction = (object->direction * 10.0f);

    direction = direction % 8;

    if (direction < 0) {
        direction = 8 + direction;
    }

    int delta = (direction - direction);

    if (delta < 0) {
        if (delta > 4)
            --direction;
        else
            ++direction;

    } else if (delta > 0) {

        if (delta > 4)
            ++direction;
        else
            --direction;
    }

    object->direction = direction % 8;
}
