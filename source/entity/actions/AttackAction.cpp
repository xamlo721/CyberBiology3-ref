#include "AttackAction.h"

void AttackAction::onActivate(Bot* object, CellCluster* cluster) {

    //If dies of low energy
    if (object->TakeEnergy(AttackCost)) {
        object->isAlive = false;
        return;

    } 

    if (!cluster->cell(object->lookAt)->isBot()) {
        return;
    }

    //If there is an object
    Object* obj = cluster->cell(object->lookAt)->getObjectPointer();

    //Eat a Bot
    object->GiveEnergy(obj->energy, EnumEnergySource::predation);
    cluster->cell(object->lookAt)->setEmpty();

    delete obj;

}
