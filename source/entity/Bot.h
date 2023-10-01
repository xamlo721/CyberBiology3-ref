#pragma once
//#pragma message("	Bot_h")


#include "Object.h"

#include "../logic/BotNeuralNet.h"

#include "../logic/EnumEnergySource.h"
#include "../logic/ITickable.h"


//Rotations array, contains where a Bot would look with every
//position of its head
const Point Rotations[] =
{
    {0,-1},
    {1,-1},
    {1,0},
    {1,1},
    {0,1},
    {-1,1},
    {-1,0},
    {-1,-1}
};



//Preset colors
const Uint8 presetColors[][4] =
{
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255},
    {223, 52, 210},
    {200, 14, 84},
    {60, 60, 163},
    {160, 160, 200},
    {0, 255, 255},
    {100, 170, 170},
    {80, 90, 90},
    {70, 200, 160},
    {0, 130, 86},
    {0, 133, 0},
    {140, 255, 0},
    {136, 160, 103},
    {200, 255, 0},
    {160, 180, 100},
    {255, 255, 0},
    {255, 190, 0},
    {170, 150, 85},
    {255, 120, 0},
    {240, 200, 200}
};



class Bot final: public Object , public ITickable {

    public:

        //Rotation, see Rotations[]
        uint direction = 0;

        //That is what a Bot is looking at
        Point lookAt;

        int lookAt_x, lookAt_y;

        void CalculateLookAt();


        //Active brain - one that is used during simulation, may be changed or optimized
        BotNeuralNet activeBrain;
        //Bot gets initial brain from his parent, it is his original genes
        BotNeuralNet initialBrain;

        //if this is not 0, Bot does nothing at his turn
        int stunned;

        //How long a Bot should wait before multiply
        int fertilityDelay;


        //Energy acquired from different sources
        int energyFromPS = 0;
        int energyFromPredation = 0;
        int energyFromOrganics = 0;

        //Mutation markers used to decide how close two bots are to each other as relatives
        int mutationMarkers[NumberOfMutationMarkers];
        uint nextMarker;

        void RandomizeMarkers();
        void RandomizeColor();
        void RandomDirection();

        //Shift color a little (-10 to +10)
        void ChangeColor(const int str = 10);

        //Create brain input data
        BrainInput FillBrainInput();

        //----------------------------------------------------------------------------------------------
        //These functions are used for experiments such as adaptation,
        //you are supposed to call them in tick() function, or do not use

        int adaptation_numTicks = 0;
        int adaptation_numRightSteps = 0;
        int addaptation_lastX;

        int adaptationCounter = 0;

        //How many times Bot used attack, move and PS commands
        uint numAttacks = 0;
        uint numMovesY = 0;
        uint numPSonLand = 0;

        //Bot visited land
        bool wasOnLand = false;

        bool ArtificialSelectionWatcher_OnTick();

        //----------------------------------------------------------------------------------------------
        BrainOutput tmpOut;

    public:

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
        struct summary_return
        {
            int simple, radialBasis, random, memory, connections, deadend, neurons;
        };



        //Inherit from a parent
        Bot(int X, int Y, uint Energy, Bot* prototype, bool mutate = false);

        //New Bot
        Bot(int X, int Y, uint Energy = MaxPossibleEnergyForABot);


        //Use neural network (feed forward)
        BrainOutput think(BrainInput input);

        //Bot tick function, it should always call parents tick function first
        virtual void tick()  override; //ITickable

        void GiveEnergy(int num, EnumEnergySource::EnergySource src = EnumEnergySource::unknown);

        //Return current energy amount from different sources
        int GetEnergyFromPS();
        int GetEnergyFromKills();

        int* GetMarkers();

        BotNeuralNet* GetActiveBrain();
        BotNeuralNet* GetInitialBrain();

        //Take away Bot energy, return true if 0 or below (Bot dies)
        bool TakeEnergy(int val);

        summary_return GetNeuronSummary();

        /*Find out how close these two are as relatives,
        returns number of matching mutation markers*/
        int FindKinship(Bot* stranger);

        void SetColor(Color);
        void SetColor(Uint8, Uint8, Uint8);

        static Color GetRandomColor();



    private:



};
