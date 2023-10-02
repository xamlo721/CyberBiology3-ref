#pragma once

#include "Cell.h"
#include "../Settings.h"
#include "../entity/Object.h"

class PrivateSyncWorld {

    private:



    public:
        //������� ����
        Cell worldMap[FieldCellsWidth][FieldCellsHeight];

        //������ �������� ��������� ����������
        std::list<Object*> entityes;

        //������ ����������� ��������
        std::list<Object*> tempEntityes;


        PrivateSyncWorld();

        //thread-unsafetly
        bool addObjectUnsafetly(Object* obj);

        bool removeObjectUnsafetly(int X, int Y);




};

