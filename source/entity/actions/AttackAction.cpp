#include "AttackAction.h"

void AttackAction::onActivate(Bot* object, CellCluster* cluster) {

    //If dies of low energy
    if (object->TakeEnergy(AttackCost)) {
        object->isAlive = false;
        return;

    } 
    Point clusterLookAtCoords = validateBotLookAtCoordinates(object->lookAt);

    if (cluster->area[clusterLookAtCoords.x][clusterLookAtCoords.y]->objectType != EnumObjectType::Bot) {
        return;
    }

    //If there is an object
    Object* obj = cluster->area[clusterLookAtCoords.x][clusterLookAtCoords.y]->object;

    //Eat a Bot
    object->GiveEnergy(obj->energy, EnumEnergySource::predation);

    obj->isAlive = false;

    cluster->area[clusterLookAtCoords.x][clusterLookAtCoords.y]->objectType = EnumObjectType::Empty;

}
