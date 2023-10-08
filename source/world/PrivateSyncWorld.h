#pragma once

#include "Cell.h"
#include "../Settings.h"
#include "../entity/Object.h"
#include "../entity/Bot.h"
#include <vector>
#include <shared_mutex>
#include <mutex>

class PrivateSyncWorld {

    public://fIXME
        //main game map
        Cell * worldMap[FieldCellsWidth][FieldCellsHeight];

        mutable std::shared_mutex  worldMutex;


    public:
        PrivateSyncWorld();


        //thread-unsafetly
        bool addObjectUnsafetly(Object* obj);

        bool removeObjectUnsafetly(int X, int Y);

        void setWall(int xCoord, int yCoord) {
            auto lck = std::unique_lock{ worldMutex };
            worldMap[xCoord][yCoord]->setWall();

        }
        void setEmpty(int xCoord, int yCoord) {
            auto lck = std::unique_lock{ worldMutex };
            worldMap[xCoord][yCoord]->setEmpty();
        }
        void setObject(int xCoord, int yCoord, Object * obj) {
            auto lck = std::unique_lock{ worldMutex };
            worldMap[xCoord][yCoord]->setObject(obj);
        }



        void lockCell(int xCoord, int yCoord) {
            worldMap[xCoord][yCoord]->lock();
        }

        void unlockCell(int xCoord, int yCoord) {
            worldMap[xCoord][yCoord]->unlock();
        }




        Object* getObject(int xCoord, int yCoord) {
            //auto lck = std::shared_lock{ worldMutex };
            return worldMap[xCoord][yCoord]->getObjectPointer();
        }

        Cell* getCellPointer(int xCoord, int yCoord) {
            //auto lck = std::shared_lock{ worldMutex };
            return worldMap[xCoord][yCoord];
        }


};

