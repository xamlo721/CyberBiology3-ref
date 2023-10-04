#include "AttackAction.h"

void AttackAction::onActivate(Bot* object, CellCluster* cluster) {

    //If dies of low energy
    if (object->TakeEnergy(AttackCost)) {
        object->isAlive = false;
        return;

    } 

    if (cluster->area[object->lookAt.x][object->lookAt.y]->objectType != EnumObjectType::Bot) {
        return;
    }

    //If there is an object
    Object* obj = cluster->area[object->lookAt.x][object->lookAt.y]->object;

    //Eat a Bot
    object->GiveEnergy(obj->energy, EnumEnergySource::predation);

    obj->isAlive = false;

    cluster->area[object->lookAt.x][object->lookAt.y]->objectType = EnumObjectType::Empty;

}
