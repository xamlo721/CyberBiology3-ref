#pragma once

#include "../MyTypes.h"

#include "../Settings.h"

#include "../world/World.h"
#include "../renderer/WorldRenderer.h"
#include "../renderer/ObjectRenderer.h"

class Object;

//Don't touch
#define NumThreads 1
#ifdef UseFourThreads
#undef NumThreads
#define NumThreads 4
#endif
#ifdef UseEightThreads
#undef NumThreads
#define NumThreads 8
#endif


class WorldController {

    private:
        static WorldController* instance;

    public:
        static WorldController* INSTANCE() {
            if (instance == 0) {
                instance = new WorldController();
            }
            return instance;
        }

    public:

        World * gameWorld = World::INSTANCE();
        WorldRenderer worldRenderer;
        ObjectRenderer objectRenderer;

        //Apple spawn timer
        uint spawnApplesInterval = 0;


        //threads
        abool threadGoMarker[NumThreads];
        std::thread* threads[NumThreads];
        uint counters[NumThreads][4];
        abool threadTerminated[NumThreads];
        abool terminateThreads = false;
        abool pauseThreads = false;


        WorldController();
        ~WorldController();

        //Spawn group of random bots
        void SpawnControlGroup();
        void SpawnApples();

        void PauseThreads();
        void UnpauseThreads();


        //tick function for single threaded build
        inline void tick_single_thread();

        //Process function for multi threaded simulation
        void ProcessPart_MultipleThreads(const uint X1, const uint Y1, const uint X2, const uint Y2, const uint index);

        void ProcessPart_AlternativeMultipleThreads(const uint X1, const uint Y1, const uint X2, const uint Y2, const uint index);




        //Start all threads
        void StartThreads();

        //Wait for all threads to finish their calculations
        void WaitForThreads();

        //Multithreaded tick function
        inline void tick_multiple_threads();

        //Wait for a signal 
        inline void ThreadWait(const uint index);

        void jumpToFirstBot();


        //Tick function
        void tick(uint thisFrame);

        //Tick function for every object
        void ObjectTick(Object* tmpObj);

        //How may free cells are available around a given one
        int FindHowManyFreeCellsAround(int X, int Y);


        //Transform absolute screen coords to cell position on field
        Point ScreenCoordsToLocal(int X, int Y);
};

