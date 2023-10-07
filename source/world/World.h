#pragma once

#include "EnumSeason.h"

#include "../entity/Object.h"
#include "../entity/Bot.h"
#include "../Settings.h"

#include "FieldDynamicParams.h"
#include "../logic/ITickable.h"

#include "Cell.h"
#include "CellCluster.h"


#include "PrivateSyncWorld.h"

class World {

    private:
        static World* instance;

        // !!!!!!!!!!!WARNING!!!!!!!!!!!
        // 
        // Очень важный атомарный флаг - Необходим для предотвращения 
        // коллезий при разметке блокировок кластеров
        // Перед тем как заблокировать кластер, необходимо заблокировать мир
        // Иначе во время коллизий кластеров может случиться dead-lock 
        // 
        // !!!!!!!!!!!WARNING!!!!!!!!!!!
        abool isProcessing = false;

        PrivateSyncWorld world;
        std::vector<Object*> copyList;

        std::mutex clusterMutex;
        std::mutex mapMutex;

        //Needed to calculate number of active objects and bots (calculated on every frame)

    public:

        static World* INSTANCE() {
            if (instance == 0) {
                instance = new World();
            }
            return instance;
        }

	public:
        static uint seed;

        int photosynthesisReward = FoodbaseInitial;
        FieldDynamicParams params;

        World();

        void generateWorldBorder();


        //thread-safetly
        bool addObjectSafetly(Object* obj);

        //Remove object and delete object class
        //thread-safetly
        void removeObjectSafetly(int X, int Y);


        void RemoveAllObjects();

        //thread-safetly
        bool moveObject(Object* obj, int toX, int toY);

        CellCluster* getLockedCluster(Object * obj);

        //Service

        inline void lockMap();

        inline void unlockMap();
        


        void startStep();

        void stopStep();


        //Is cell out if bounds?
        bool IsInBounds(int X, int Y);
        bool IsInBounds(Point p);

        bool IsInWater(int Y);
        bool IsInMud(int Y);

        Object* GetObjectLocalCoords(int X, int Y);



        //How many objects on field, prev. frame
        uint GetNumObjects();
        uint GetNumBots();

        //This function is needed to tile world horizontally (change X = -1 to X = FieldCellsWidth etc.)
        int ValidateX(int X);

        bool ValidateObjectExistance(Object* obj);


        //yes, its achitecture mistacke, dont use it.
        std::vector<Object*> getObjectsForRenderer();
};

