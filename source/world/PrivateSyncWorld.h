#pragma once

#include "Cell.h"
#include "../Settings.h"
#include "../entity/Object.h"
#include "../entity/Bot.h"
#include <vector>
#include <shared_mutex>
#include <mutex>

class PrivateSyncWorld {

    private:
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
            worldMap[xCoord][yCoord]->objectType = EnumObjectType::WorldBorder;

        }
        void setEmpty(int xCoord, int yCoord) {
            auto lck = std::unique_lock{ worldMutex };
            worldMap[xCoord][yCoord]->objectType = EnumObjectType::Empty;
            worldMap[xCoord][yCoord]->object = NULL;
        }
        void setObject(int xCoord, int yCoord, Object * obj) {
            auto lck = std::unique_lock{ worldMutex };
            worldMap[xCoord][yCoord]->objectType = EnumObjectType::Bot;
            worldMap[xCoord][yCoord]->object = obj;
        }



        void lockCell(int xCoord, int yCoord) {
            auto lck = std::unique_lock{ worldMutex };
            while (worldMap[xCoord][yCoord]->isLocked) {
                Sleep(1);
                std::this_thread::yield();
            }
            worldMap[xCoord][yCoord]->isLocked = true;
        }
        void unlockCell(int xCoord, int yCoord) {
            auto lck = std::shared_lock{ worldMutex };

            worldMap[xCoord][yCoord]->isLocked = false;

        }
        bool isCellLocked(int xCoord, int yCoord) {
            auto lck = std::unique_lock{ worldMutex };

            return worldMap[xCoord][xCoord]->isLocked;
        }
        



        bool isEmpty(int xCoord, int yCoord) {
            auto lck = std::shared_lock{ worldMutex };
            return worldMap[xCoord][yCoord]->objectType == EnumObjectType::Empty;
        }
        bool isBorder(int xCoord, int yCoord) {
            auto lck = std::shared_lock{ worldMutex };
            return worldMap[xCoord][yCoord]->objectType == EnumObjectType::WorldBorder;

        }
        bool isBot(int xCoord, int yCoord) {
            auto lck = std::shared_lock{ worldMutex };
            return worldMap[xCoord][yCoord]->objectType == EnumObjectType::Bot;
        }


        Object* getObject(int xCoord, int yCoord) {
            auto lck = std::shared_lock{ worldMutex };
            return worldMap[xCoord][yCoord]->object;
        }

        Cell* getCellPointer(int xCoord, int yCoord) {
            auto lck = std::shared_lock{ worldMutex };
            return worldMap[xCoord][yCoord];
        }




};

