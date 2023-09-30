#pragma once

class FieldDynamicParams {

    public:

        int oceanLevel;
        int mudLevel;
        int appleEnergy;

        int adaptation_DeathChance_Winds;
        int adaptation_StepsNum_Winds;

        int adaptation_landBirthBlock;
        int adaptation_seaBirthBlock;
        int adaptation_PSInOceanBlock;
        int adaptation_PSInMudBlock;
        int adaptation_botShouldBeOnLandOnceToMultiply;
        int adaptation_botShouldDoPSOnLandOnceToMultiply;
        int adaptation_forceBotMovements;

        int adaptation_organicSpawnRate;

        bool spawnApples;

        int reserved[38];

        void Reset();

        FieldDynamicParams();

};
