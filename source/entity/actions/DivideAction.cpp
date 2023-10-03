#include "DivideAction.h"

#include "../../world/World.h"

void DivideAction::onActivate(Bot* object, CellCluster* cluster) {

    if (!object->isAlive) {
        return;
    }

    if (ArtificialSelectionWatcher_OnDivide(object)) {
        return;
    }

    //for (int b = 0; b < numChildren; ++b) {

        if (object->energy <= 1 + GiveBirthCost) {
            return;
        } else {
            Point freeSpace;

            freeSpace = cluster->FindFreeNeighbourCell();
            freeSpace.x += object->x;
            freeSpace.y += object->y;

            if (freeSpace.x != -1) {
#ifndef NewbornGetsHalf
                object->TakeEnergy(EnergyPassedToAChild + GiveBirthCost);

                if ((!RandomPercentX10(World::INSTANCE()->params.adaptation_botShouldBeOnLandOnceToMultiply)))
                    World::INSTANCE()->addObjectSafetly(new Bot(freeSpace.x, freeSpace.y, EnergyPassedToAChild, object, RandomPercent(MutationChancePercent)));
#else
                object->TakeEnergy(object->energy / 2 + GiveBirthCost);

                if ((!RandomPercentX10(World::INSTANCE()->params.adaptation_botShouldBeOnLandOnceToMultiply)))
                    World::INSTANCE()->addObjectSafetly(new Bot(freeSpace.x, freeSpace.y, object->energy, object, RandomPercent(MutationChancePercent)));
#endif

                return;
            }
        }
    //}

    if (object->energy <= 0) {
        object->isAlive = false;
        return;
    }

}

bool DivideAction::ArtificialSelectionWatcher_OnDivide(Bot* object) {

    FieldDynamicParams& params = World::INSTANCE()->params;

    //Is on land
    if (object->y < FieldCellsHeight - params.oceanLevel) {
        if (RandomPercentX10(params.adaptation_landBirthBlock))
            return true;
    }
    //Is in ocean
    else if ((object->y >= FieldCellsHeight - params.oceanLevel) && (object->y < FieldCellsHeight - params.mudLevel))
    {
        if (RandomPercentX10(params.adaptation_seaBirthBlock))
            return true;
    }

    return false;
}


