#include "AttackAction.h"

#include "../../world/World.h"

void AttackAction::onActivate(Bot* object, CellCluster* cluster) {

    //If dies of low energy
    if (object->TakeEnergy(AttackCost)) {
        object->isAlive = false;
        return;

    } 

    if (World::INSTANCE()->IsInBounds(object->lookAt.x, object->lookAt.y)) {



        //If there is an object
        Object* obj = World::INSTANCE()->GetObjectLocalCoords(object->lookAt.x, object->lookAt.y);

        if (!obj) {
            return;
        }

        if (obj->type == EnumObjectType::Bot) {

            //Eat a Bot
            object->GiveEnergy(obj->energy, EnumEnergySource::predation);
            World::INSTANCE()->removeObjectSafetly(object->lookAt.x, object->lookAt.y);
        }
       
    }


}
