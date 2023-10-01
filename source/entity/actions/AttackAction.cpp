#include "AttackAction.h"

#include "../../world/World.h"

void AttackAction::onActivate(Bot* object) {

    //If dies of low energy
    if (object->TakeEnergy(AttackCost)) {
        //return 1;
        return;

    } 

    if (World::INSTANCE()->IsInBounds(object->lookAt_x, object->lookAt_y)) {

        //If there is an object
        Object* obj = World::INSTANCE()->allCells[object->lookAt_x][object->lookAt_y];

        if (!obj) {
            return;
        }

        if (obj->type == EnumObjectType::Bot) {

            //Eat a Bot
            object->GiveEnergy(obj->energy, EnumEnergySource::predation);
            World::INSTANCE()->RemoveBot(object->lookAt_x, object->lookAt_y);

            ++object->numAttacks;
        }
       
    }


}
