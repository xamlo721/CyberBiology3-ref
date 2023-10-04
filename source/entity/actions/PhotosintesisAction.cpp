#include "PhotosintesisAction.h"

#include "../../world/World.h"

void PhotosintesisAction::onActivate(Bot* object, CellCluster* cluster) {

    //Above water
    if (!World::INSTANCE()->IsInWater(object->y)) {

        int toGive = PhotosynthesisReward_Summer;

        //Give energy depending on a season
        switch (World::INSTANCE()->season) {

            case EnumSeason::summer:
                #ifdef UseSeasons
                    toGive = PhotosynthesisReward_Summer;
                #else
                    toGive = World::INSTANCE()->photosynthesisReward;
                    //toGive = FindHowManyFreeCellsAround(x, y) - 3;
                    //if (toGive < 0) toGive = 0;
                #endif
                break;
            case EnumSeason::autumn: case EnumSeason::spring:
                toGive = PhotosynthesisReward_Autumn;
                break;
            case EnumSeason::winter:
                toGive = PhotosynthesisReward_Winter;
                //toGive = (ticknum % 5 == 0) ? 2 : 1;
                break;
        }

        object->GiveEnergy(toGive, EnumEnergySource::PS);
       
    } else { 
        //Below water

        #ifndef NoPhotosyntesisInOcean

            if (World::INSTANCE()->IsInMud(object->y))
            {
                if (RandomPercentX10(World::INSTANCE()->params.adaptation_PSInMudBlock))
                    return;
            }
            else
            {
                if (RandomPercentX10(World::INSTANCE()->params.adaptation_PSInOceanBlock))
                    return;
            }

            object->GiveEnergy(World::INSTANCE()->photosynthesisReward, EnumEnergySource::PS);

        #endif
    }

}