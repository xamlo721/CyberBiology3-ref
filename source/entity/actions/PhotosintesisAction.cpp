#include "PhotosintesisAction.h"

void PhotosintesisAction::onActivate(Bot* object, CellCluster* cluster) {


    int toGive = PhotosynthesisReward_Summer;

    object->GiveEnergy(toGive, EnumEnergySource::PS);


}