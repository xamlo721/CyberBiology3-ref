
#include "Bot.h"
#include "../world/EnumSeason.h"
#include "../world/World.h"


Bot::Bot(int X, int Y, uint Energy, Bot* prototype, bool mutate) :Object(X, Y, EnumObjectType::Bot), initialBrain(&prototype->initialBrain) {
    energy = Energy;
    stunned = StunAfterBirth;
    fertilityDelay = FertilityDelay;
    energyFromPS = 0;
    energyFromPredation = 0;

    //Copy parent's markers and color
    memcpy(mutationMarkers, prototype->mutationMarkers, sizeof(mutationMarkers));
    nextMarker = prototype->nextMarker;

    color = prototype->color;

    //Random direction
    RandomDirection();

    //Create active brain
    activeBrain.Clone(&initialBrain);
    activeBrain.Optimize();

    addaptation_lastX = X;
}


Bot::Bot(int X, int Y, uint Energy) :Object(X, Y, EnumObjectType::Bot) {

    RandomizeMarkers();

    energy = Energy;
    stunned = StunAfterBirth;
    fertilityDelay = FertilityDelay;
    energyFromPS = 0;
    energyFromPredation = 0;

    //Randomize Bot brain
    initialBrain.Randomize();
    activeBrain.Clone(&initialBrain);
    activeBrain.Optimize();

    //Random color
    RandomizeColor();

    //Random direction
    RandomDirection();

    addaptation_lastX = X;

    //Temporary
    numMovesY = 1000;
}


void Bot::CalculateLookAt()
{
    lookAt = Rotations[direction];

    lookAt.Shift(x, y);

    lookAt.x = World::INSTANCE()->ValidateX(lookAt.x);

    lookAt_x = lookAt.x,
    lookAt_y = lookAt.y;
}


void Bot::RandomizeMarkers()
{
    for (uint i = 0; i < NumberOfMutationMarkers; ++i)
    {
        mutationMarkers[i] = rand();
    }
    nextMarker = 0;
}

Color Bot::GetRandomColor() {
    Color toRet;
    toRet.SetRandom();
    return toRet;
}

void Bot::RandomizeColor() {
    color = GetRandomColor();
}

void Bot::RandomDirection() {
    direction = RandomVal(8);
}

void Bot::ChangeColor(const int str) {
    color.RandomChange(str);
}

BrainInput Bot::FillBrainInput() {

    BrainInput input;

    //If destination is out of bounds
    if (!World::INSTANCE()->IsInBounds(lookAt_x, lookAt_y))
    {
        //1 if unpassable
        input.vision = 1.0f;
    }
    else
    {
        Object* tmpDest = World::INSTANCE()->allCells[lookAt_x][lookAt_y];

        //Destination cell is empty
        if (!tmpDest)
        {
            //0 if empty
            input.vision = 0.0f;
        }
        else
        {
            //Destination not empty
            switch (tmpDest->type)
            {
            case EnumObjectType::Bot:
                //0.5 if someone is in that cell
                input.vision = 1.0f;

                //Calculate how close they are as relatives, based on mutation markers
                input.vision += (1.0f - (FindKinship((Bot*)tmpDest) * 1.0f) / (NumberOfMutationMarkers * 1.0f));
                break;
            }
        }
    }

    input.age = (lifetime * 1.0f) / (MaxBotLifetime * 1.0f);

    //input.rotation = (tmpOut.desired_rotation == (direction * .1f))?1.0f:0.0f;
    input.rotation = (direction * 1.0f) / 7.0f;

    return input;
}

BrainOutput Bot::think(BrainInput input) {

    //Stunned means the creature can not act
    if (stunned) {
        --stunned;

        return BrainOutput::GetEmptyBrain();
    }

    //Clear all neuron values
    activeBrain.Clear();

    //Input data
    {
        //Energy
        activeBrain.allValues[NeuronInputLayerIndex][0] = ((energy * 1.0f) / (MaxPossibleEnergyForABot * 1.0f));

        //Sight
        activeBrain.allValues[NeuronInputLayerIndex][1] = input.vision;

        //Bot age
        activeBrain.allValues[NeuronInputLayerIndex][2] = input.age;

        //Rotation
        activeBrain.allValues[NeuronInputLayerIndex][3] = input.rotation;

        //Height
        activeBrain.allValues[NeuronInputLayerIndex][4] = (y * 1.0f) / (FieldCellsHeight * 1.0f);
    }

    //Compute
    activeBrain.Process();

    BrainOutput toRet = activeBrain.GetOutput();

    //Cannot multipy if not ready
    if (fertilityDelay)
    {
        --fertilityDelay;
        toRet.divide = 0;
    }
    else if (toRet.divide)
    {
        fertilityDelay = FertilityDelay;
    }

    return toRet;
}



bool Bot::ArtificialSelectionWatcher_OnTick()
{
    //Winds
    if (World::INSTANCE()->IsInWater(y))
    {
        if (addaptation_lastX < x)
            ++adaptation_numRightSteps;

        addaptation_lastX = x;

        if (adaptationCounter++ == World::INSTANCE()->params.adaptation_StepsNum_Winds)
        {
            if (adaptation_numRightSteps == 0)
            {
                if (RandomPercentX10(World::INSTANCE()->params.adaptation_DeathChance_Winds))
                    return true;
            }

            adaptation_numRightSteps = 0;
            adaptationCounter = 0;
        }
    }

    //Divers
    FieldDynamicParams& params = World::INSTANCE()->params;

    //Force movements Y
    if (lifetime > 3)
    {
        if (numMovesY * 3 < (lifetime - 3))
        {
            if (RandomPercentX10(params.adaptation_forceBotMovements))
                return true;
        }
    }

    return false;
}

void Bot::tick() {

    ++lifetime;

    energy -= EveryTickEnergyPenalty;

    if (ArtificialSelectionWatcher_OnTick())
        //return 1;
        return;

    if (((energy) <= 0) || (lifetime >= MaxBotLifetime))
        //return 1;
        return;

    BrainOutput tmpOut;

    CalculateLookAt();

    if (!World::INSTANCE()->IsInWater(y))
        wasOnLand = true;

    //Fill brain input structure
    BrainInput input = FillBrainInput();

    //Bot brain does its stuff
    tmpOut = think(input);

    //Multiply first
    if (tmpOut.divide > 0)
    {

    }

    //Then attack
    if (tmpOut.attack > 0)  {

    } else {
        //Rotate after
        if (tmpOut.desired_rotation != (direction * .1f))
        {

        }

        //Move
        if (tmpOut.move > 0)
        {

        }
        //Photosynthesis
        else if (tmpOut.photosynthesis > 0) {

        }
    }

    //return 0;
    return;
}


void Bot::GiveEnergy(int num, EnumEnergySource::EnergySource src)
{
    energy += num;

    if (energy > MaxPossibleEnergyForABot)
    {
    #ifdef BotDiesIfEnergyOverflow
        energy = 0;
        return;
    #else
        energy = MaxPossibleEnergyForABot;
    #endif
    }

    if (src == EnumEnergySource::PS)
    {
        energyFromPS += num;
    }
    else if (src == EnumEnergySource::predation)
    {
        energyFromPredation += num;
    }
    else if (src == EnumEnergySource::organics)
    {
        energyFromOrganics += num;
    }
}

int Bot::GetEnergyFromPS()
{
    return energyFromPS;
}

int Bot::GetEnergyFromKills()
{
    return energyFromPredation;
}


int* Bot::GetMarkers()
{
    return mutationMarkers;
}



bool Bot::TakeEnergy(int val)
{
    energy -= val;

    return energy <= 0;
}


/*Get neuron summary(info)
Format: (all integers)
-simple neurons
-radial basis neurons
-random neurons
-memory neurons (if any)
-total connections
-dead end neurons
-total neurons
*/
Bot::summary_return Bot::GetNeuronSummary()
{
    int toRet[6] = { 0,0,0,0,0,0 };
    Neuron* n;

    for (uint xi = 1; xi < NumNeuronLayers; ++xi)
    {
        for (uint yi = 0; yi < NeuronsInLayer; ++yi)
        {
            n = &activeBrain.allNeurons[xi][yi];

            switch (n->type)
            {
            case NeuronType::basic:
                toRet[0]++;
                break;
            case NeuronType::radialbasis:
                toRet[1]++;
                break;
            case NeuronType::random:
                toRet[2]++;
                break;
            case NeuronType::memory:
                toRet[3]++;
                break;
            }

            toRet[4] += n->numConnections;

            if (n->numConnections == 0)
                toRet[5]++;
        }
    }

    return { toRet[0], toRet[1], toRet[2], toRet[3], toRet[4], toRet[5], NumNeuronLayers * NeuronsInLayer };
}


int Bot::FindKinship(Bot* stranger)
{
    int numMarkers = 0;

    for (uint i = 0; i < NumberOfMutationMarkers; ++i)
    {
        if (mutationMarkers[i] == stranger->mutationMarkers[i])
            ++numMarkers;
    }

    if (numMarkers >= (NumberOfMutationMarkers - HowMuchDifferenseCantBeTold))
        numMarkers = NumberOfMutationMarkers;

    return numMarkers;
}

void Bot::SetColor(Color newColor) {
    color = newColor;
}

void Bot::SetColor(Uint8 r, Uint8 g, Uint8 b) {
    color.r = r;
    color.g = g;
    color.b = b;
}




BotNeuralNet* Bot::GetActiveBrain()
{
    return &activeBrain;
}

BotNeuralNet* Bot::GetInitialBrain()
{
    return &initialBrain;
}
