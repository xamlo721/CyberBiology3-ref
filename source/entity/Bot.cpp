
#include "Bot.h"

Bot::Bot(int X, int Y, uint Energy, Bot* prototype, bool mutate) :Object(X, Y, EnumObjectType::Bot), initialBrain(&prototype->initialBrain) {
    isAlive = true;
    energy = Energy;

    color = prototype->color;

    //Random direction
    direction = RandomVal(8);

    //Create active brain
    activeBrain.Clone(&initialBrain);
    activeBrain.Optimize();
}


Bot::Bot(int X, int Y, uint Energy) :Object(X, Y, EnumObjectType::Bot) {
    isAlive = true;

    energy = Energy;

    //Randomize Bot brain
    initialBrain.Randomize();
    activeBrain.Clone(&initialBrain);
    activeBrain.Optimize();

    //Random color
    color.SetRandom();

    //Random direction
    direction = RandomVal(8);
}


BrainOutput Bot::think(BrainInput input) {
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

    return toRet;
}

void Bot::tick() {

    ++lifetime;

    energy -= EveryTickEnergyPenalty;

    if (((energy) <= 0) || (lifetime >= MaxBotLifetime)) {
        isAlive = false;
        return;
    }

    lookAt = Rotations[direction];

    return;
}


void Bot::GiveEnergy(int num, EnumEnergySource::EnergySource src)
{
    energy += num;

    if (energy > MaxPossibleEnergyForABot) {
        energy = MaxPossibleEnergyForABot;
    }
}

bool Bot::TakeEnergy(int val) {
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
