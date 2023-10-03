#include "AttackAction.h"

#include "../../world/World.h"

void AttackAction::onActivate(Bot* object, CellCluster* cluster) {

    if (!object->isAlive) {
        return;
    }

    //If dies of low energy
    if (object->TakeEnergy(AttackCost)) {
        object->isAlive = false;
        return;

    } 

    if (World::INSTANCE()->IsInBounds(object->lookAt_x, object->lookAt_y)) {



        //If there is an object
        Object* obj = World::INSTANCE()->GetObjectLocalCoords(object->lookAt_x, object->lookAt_y);

        if (!obj) {
            return;
        }

        if (obj->type == EnumObjectType::Bot) {

            //Eat a Bot
            object->GiveEnergy(obj->energy, EnumEnergySource::predation);
            World::INSTANCE()->removeObjectSafetly(object->lookAt_x, object->lookAt_y);
        }
       
    }


}
