#include "FieldDynamicParams.h"

#include "../Settings.h"
#include "../MyTypes.h"

void FieldDynamicParams::Reset() {
    oceanLevel = InitialOceanHeight;
    mudLevel = InitialMudLayerHeight;

    adaptation_DeathChance_Winds = 0;
    adaptation_StepsNum_Winds = 2;

    adaptation_landBirthBlock = 0;
    adaptation_seaBirthBlock = 0;
    adaptation_PSInOceanBlock = 0;
    adaptation_PSInMudBlock = 0;
    adaptation_botShouldBeOnLandOnceToMultiply = 0;
    adaptation_botShouldDoPSOnLandOnceToMultiply = 0;
    adaptation_forceBotMovements = 0;

    memset(reserved, 0, sizeof(reserved));
}

FieldDynamicParams::FieldDynamicParams() {
    Reset();
}
