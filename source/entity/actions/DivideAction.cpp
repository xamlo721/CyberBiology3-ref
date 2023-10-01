#include "DivideAction.h"

#include "../../world/World.h"

void DivideAction::onActivate(Bot* object) {

    if (ArtificialSelectionWatcher_OnDivide()) {
        return;
    }

    for (int b = 0; b < numChildren; ++b) {

        if (object->energy <= 1 + GiveBirthCost) {
            return;
        } else {
            Point freeSpace;

            freeSpace = World::INSTANCE()->FindFreeNeighbourCell(x, y);

            if (freeSpace.x != -1) {
#ifndef NewbornGetsHalf
                object->TakeEnergy(EnergyPassedToAChild + GiveBirthCost);

                if ((!RandomPercentX10(World::INSTANCE()->params.adaptation_botShouldBeOnLandOnceToMultiply)) || (object->wasOnLand))
                    World::INSTANCE()->AddObject(new Bot(freeSpace.x, freeSpace.y, EnergyPassedToAChild, this, RandomPercent(MutationChancePercent)));
#else
                object->TakeEnergy(object->energy / 2 + GiveBirthCost);

                if ((!RandomPercentX10(World::INSTANCE()->params.adaptation_botShouldBeOnLandOnceToMultiply)) || (object->wasOnLand))
                    World::INSTANCE()->AddObject(new Bot(freeSpace.x, freeSpace.y, object->energy, object, RandomPercent(MutationChancePercent)));
#endif

                return;
            }
        }
    }

    if (object->energy <= 0) {
        //return 1;
        return;
    }

}

bool DivideAction::ArtificialSelectionWatcher_OnDivide() {

    FieldDynamicParams& params = World::INSTANCE()->params;

    //Is on land
    if (y < FieldCellsHeight - params.oceanLevel)
    {
        if (RandomPercentX10(params.adaptation_landBirthBlock))
            return true;
    }
    //Is in ocean
    else if ((y >= FieldCellsHeight - params.oceanLevel) && (y < FieldCellsHeight - params.mudLevel))
    {
        if (RandomPercentX10(params.adaptation_seaBirthBlock))
            return true;
    }

    //Force photosynthesis on land
    if (numPSonLand < 4)
    {
        if (RandomPercentX10(params.adaptation_botShouldDoPSOnLandOnceToMultiply))
            return true;
    }

    return false;
}


