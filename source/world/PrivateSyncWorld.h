#pragma once

#include "Cell.h"
#include "../Settings.h"
#include "../entity/Object.h"

class PrivateSyncWorld {

    private:



    public:
        //Игровое поле
        Cell worldMap[FieldCellsWidth][FieldCellsHeight];

        //Список объектов ожидающих обновления
        std::list<Object*> entityes;

        //Список обновленных объектов
        std::list<Object*> tempEntityes;


        PrivateSyncWorld();

        //thread-unsafetly
        bool addObjectUnsafetly(Object* obj);

        bool removeObjectUnsafetly(int X, int Y);




};

