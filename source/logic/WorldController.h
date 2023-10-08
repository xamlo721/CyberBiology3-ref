#pragma once

#include "../MyTypes.h"

#include "../Settings.h"

#include "../world/World.h"
#include "../renderer/WorldRenderer.h"
#include "../renderer/ObjectRenderer.h"

#include "MyThreadLoop.h"




class WorldController : public virtual MyThreadLoop {

    private:
        static WorldController* instance;
        WorldController() ;

    public:
        static WorldController* INSTANCE() {
            if (instance == 0) {
                instance = new WorldController();
            }
            return instance;
        }

    public:

        World * gameWorld;
        WorldRenderer worldRenderer;
        ObjectRenderer objectRenderer;

        ~WorldController();

        //Spawn group of random bots
        void SpawnControlGroup();

        virtual void onTickStated() override;

        virtual void processTick(int threadIndex, long long poolTick) override;

        virtual void onTickEnded() override;

        //Tick function
        void tick(uint thisFrame);

        //Tick function for every object
        void ObjectTick(Bot* tmpObj);

        //Transform absolute screen coords to cell position on field
        Point ScreenCoordsToLocal(int X, int Y);
};

