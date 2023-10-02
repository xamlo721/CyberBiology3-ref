#pragma once

#include "EnumSeason.h"

#include "../entity/Object.h"
#include "../entity/Bot.h"
#include "../Settings.h"

#include "FieldDynamicParams.h"
#include "../logic/ITickable.h"

#include "Cell.h"
#include "CellCluster.h"

#include <list>


class World {

    private:
        static World* instance;



        // !!!!!!!!!!!WARNING!!!!!!!!!!!
        // 
        // ����� ������ ��������� ���� - ��������� ��� �������������� 
        // �������� ��� �������� ���������� ���������
        // ����� ��� ��� ������������� �������, ���������� ������������� ���
        // ����� �� ����� �������� ��������� ����� ��������� dead-lock 
        // 
        // !!!!!!!!!!!WARNING!!!!!!!!!!!
        abool isLocked = false;


        //Needed to calculate number of active objects and bots (calculated on every frame)
        uint objectsTotal = 0;
        uint botsTotal = 0;

    public:
        //������� ����
        Cell worldMap[FieldCellsWidth][FieldCellsHeight];
        //�� �� ����� ������� ����, ������ � ��������� �� ��
        Object* worldEntityMap[FieldCellsWidth][FieldCellsHeight];

        //������ �������� ��������� ����������
        //static std::list<ITickable*> entityes;
        std::list<Object*> entityes;

        //������ ����������� ��������
        //static std::list<ITickable*> tempEntityes;
        std::list<Object*> tempEntityes;

        static World* INSTANCE() {
            if (instance == 0) {
                instance = new World();
            }
            return instance;
        }

        static void lock() {

            while (instance->isLocked) {
                std::this_thread::yield();
            }
            instance->isLocked = true;

        }

        static void unlock() {
            instance->isLocked = false;
        }

	public:
        static uint seed;

		EnumSeason::Season season;

        int photosynthesisReward = FoodbaseInitial;
        FieldDynamicParams params;

        World();

        void generateWorldBorder();


        //thread-safetly
        bool addObject(Object* obj);

        //Remove object and delete object class
        //thread-safetly
        void removeObject(int X, int Y);
        void RemoveAllObjects();

        //thread-safetly
        bool moveObject(Object* obj, int toX, int toY);

        CellCluster* getObjectsArround(Object * obj);



        //Service

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

};

