#pragma once

#include "Cell.h"
#include "../Settings.h"
#include "../entity/Object.h"
#include <vector>

class PrivateSyncWorld {

    private:



    public:

        std::mutex worldMutex;

        //������� ����
        Cell worldMap[FieldCellsWidth][FieldCellsHeight];

        //������ �������� ��������� ����������
        std::vector<Object*> entityes;

        //������ ����������� ��������
        std::vector<Object*> tempEntityes;


        PrivateSyncWorld();

        //thread-unsafetly
        bool addObjectUnsafetly(Object* obj);

        bool removeObjectUnsafetly(int X, int Y);




};

