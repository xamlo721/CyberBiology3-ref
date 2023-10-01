#pragma once

#include "EnumSeason.h"

#include "../entity/Object.h"
#include "../entity/Bot.h"
#include "../Settings.h"

#include "FieldDynamicParams.h"
#include "../logic/ITickableObject.h"

#include "Cell.h"

#include <map>


class World {

    private:
        static World* instance;
        //Игровое поле
        static Cell worldMap[FieldCellsWidth][FieldCellsHeight];

        //Список объектов ожидающих обновления
        static std::map<Uint32, ITickableObject *> entityes;

        //Список обновленных объектов
        static std::map<Uint32, ITickableObject *> tempEntityes;

    public:
        static World* INSTANCE() {
            if (instance == 0) {
                instance = new World();
            }
            return instance;
        }

	public:
        static int seed;

		EnumSeason::Season season;

        //Needed to calculate number of active objects and bots (calculated on every frame)
        uint objectsTotal = 0;
        uint botsTotal = 0;
        uint applesTotal = 0;
        uint organicsTotal = 0;

        int photosynthesisReward = FoodbaseInitial;

        //All cells as 2d array
        Object* allCells[FieldCellsWidth][FieldCellsHeight];


        FieldDynamicParams params;

        World();


        //Remove object and delete object class
        void RemoveObject(int X, int Y);
        void RemoveAllObjects();

        //Remove a Bot (same as remove object but for a Bot)
        void RemoveBot(int X, int Y, int energyVal = 0);

        //Repaint Bot
        void RepaintBot(Bot* b, Color newColor, int differs = 1);

        //Move objects from one cell to another
        int MoveObject(int fromX, int fromY, int toX, int toY);
        int MoveObject(Object* obj, int toX, int toY);

        bool AddObject(Object* obj);


        //Is cell out if bounds?
        bool IsInBounds(int X, int Y);
        bool IsInBounds(Point p);

        bool IsInWater(int Y);
        bool IsInMud(int Y);

        Object* GetObjectLocalCoords(int X, int Y);

        //Find empty cell nearby, otherwise return {-1, -1}
        Point FindFreeNeighbourCell(int X, int Y);

        Point FindRandomNeighbourBot(int X, int Y);


        //How many objects on field, prev. frame
        uint GetNumObjects();
        uint GetNumBots();
        uint GetNumApples();
        uint GetNumOrganics();

        //This function is needed to tile world horizontally (change X = -1 to X = FieldCellsWidth etc.)
        int ValidateX(int X);

        bool ValidateObjectExistance(Object* obj);

};

