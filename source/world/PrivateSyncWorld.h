#pragma once

#include "Cell.h"
#include "../Settings.h"
#include "../entity/Object.h"
#include <vector>

class PrivateSyncWorld {

    private:



    public:

        std::mutex worldMutex;

        //Игровое поле
        Cell worldMap[FieldCellsWidth][FieldCellsHeight];

        //Список объектов ожидающих обновления
        std::vector<Object*> entityes;

        //Список обновленных объектов
        std::vector<Object*> tempEntityes;


        PrivateSyncWorld();

        //thread-unsafetly
        bool addObjectUnsafetly(Object* obj);

        bool removeObjectUnsafetly(int X, int Y);




};

