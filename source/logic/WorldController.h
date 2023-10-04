#pragma once

#include "../MyTypes.h"

#include "../Settings.h"

#include "../world/World.h"
#include "../renderer/WorldRenderer.h"
#include "../renderer/ObjectRenderer.h"

//Don't touch
#define NumThreads 1


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

        //threads
        abool threadGoMarker[NumThreads];
        std::thread* threads[NumThreads];
        abool threadTerminated[NumThreads];
        abool terminateThreads = false;
        abool pauseThreads = false;


        WorldController();
        ~WorldController();

        //Spawn group of random bots
        void SpawnControlGroup();



        //Start all threads
        void StartThreads();
        void PauseThreads();
        void UnpauseThreads();
        //Wait for all threads to finish their calculations
        void waitAllThreads();

        //Multithreaded tick function
        inline void tick_multiple_threads(int threadIndex);





        //Tick function
        void tick(uint thisFrame);

        //Tick function for every object
        void ObjectTick(Object* tmpObj);

        //Transform absolute screen coords to cell position on field
        Point ScreenCoordsToLocal(int X, int Y);
};

