#include "WorldController.h"
#include "../entity/Object.h"

#include "../entity/actions/RotateAction.h"
#include "../entity/actions/AttackAction.h"
#include "../entity/actions/DivideAction.h"
#include "../entity/actions/MoveAction.h"
#include "../entity/actions/PhotosintesisAction.h"

//SINGLETON
WorldController* WorldController::instance = 0;

WorldController::WorldController() {
    //Start threads
    for (int i = 0; i < NumThreads; i++) {
        threadGoMarker[i] = false;

        const uint xCoord = 0;
        const uint yCoord = 0;
        const uint areaWidth = (FieldCellsWidth / NumThreads) * (i + 1);
        const uint areaHeight = FieldCellsHeight;
        threads[0] = new std::thread(&WorldController::ProcessPart_AlternativeMultipleThreads, this, xCoord, yCoord, areaWidth, areaHeight, i);
    }

}


int WorldController::FindHowManyFreeCellsAround(int X, int Y)
{
    int toRet = 0;

    //If cell itself is empty
    if (gameWorld->allCells[X][Y] == NULL) {
        ++toRet;
    }

    //Parse all cells
    int tx;

    for (int cx = -1; cx < 2; ++cx)
    {
        for (int cy = -1; cy < 2; ++cy)
        {

            tx = gameWorld->ValidateX(X + cx);

            if (gameWorld->IsInBounds(tx, Y + cy))
            {
                if (gameWorld->allCells[tx][Y + cy] == NULL)
                {
                    ++toRet;
                }
            }
        }
    }

    return toRet;
}



void WorldController::ObjectTick(Object* tmpObj) {
    
    //TODO: Lock 3x3 

    //TODO: remove dead object

        //Object destroyed
        //if (tmpObj->type == EnumObjectType::Bot)
        //    gameWorld->RemoveBot(tmpObj->x, tmpObj->y, tmpObj->energy);
        //else
        //    gameWorld->RemoveObject(tmpObj->x, tmpObj->y);

        //return;

     ((Bot*)tmpObj)->tick();
     BrainOutput actions = ((Bot*)tmpObj)->tmpOut;


     //TODO: ������� ������� ��� ������ Actions � ��������� �� �� �������


     //Multiply first
     if (actions.divide > 0) {
         //FIXME: ���� ������ � ��������� �������� �������� ����������� ������� Action � ������ ����
         DivideAction action;
         action.onActivate(((Bot*)tmpObj));

     }

     //Then attack
     if (actions.attack > 0) {
         AttackAction action;
         action.onActivate(((Bot*)tmpObj));

     } else {

         //Rotate after
         if (actions.desired_rotation != (((Bot*)tmpObj)->direction * .1f)) {
             RotateAction action;
             action.onActivate(((Bot*)tmpObj));
         }

         //Move
         if (actions.move > 0) {
             MoveAction action;
             action.onActivate(((Bot*)tmpObj));
         }
         //Photosynthesis
         else if (actions.photosynthesis > 0) {
             PhotosintesisAction action;
             action.onActivate(((Bot*)tmpObj));
         }

     }


}

//tick function for single threaded build
inline void WorldController::tick_single_thread()
{
    Object* tmpObj;

    gameWorld->objectsTotal = 0;
    gameWorld->botsTotal = 0;

    for (uint ix = 0; ix < FieldCellsWidth; ++ix)
    {
        for (uint iy = 0; iy < FieldCellsHeight; ++iy)
        {
            tmpObj = gameWorld->allCells[ix][iy];

            if (tmpObj)
            {
                ++gameWorld->objectsTotal;

                if (tmpObj->type == EnumObjectType::Bot)
                    ++gameWorld->botsTotal;

                ObjectTick(tmpObj);
            }
        }

    }
}

//Wait for a signal 
inline void WorldController::ThreadWait(const uint index)
{
    for (;;)
    {
        if (threadGoMarker[index])
            return;

        std::this_thread::yield();

        if (pauseThreads)
        {
            //Delay so it would not eat too many resourses while on pause
            SDL_Delay(1);
        }
    }
}


//Start all threads
void WorldController::StartThreads() {
    repeat(NumThreads)
    {
        threadGoMarker[i] = true;
    }
}

//Wait for all threads to finish their calculations
void WorldController::waitAllThreads()
{
    uint threadsReady;

    for (;;)
    {

        threadsReady = 0;

        repeat(NumThreads)
        {
            if (threadGoMarker[i] == false)
                threadsReady++;
        }

        if (threadsReady == NumThreads)
            break;

        std::this_thread::yield();

    }
}

//Multithreaded tick function
inline void WorldController::tick_multiple_threads()
{
    auto clear_counters = [&]()
        {
            repeat(NumThreads)
            {
                counters[i][0] = 0;
                counters[i][1] = 0;
                counters[i][2] = 0;
                counters[i][3] = 0;
            }
        };

    gameWorld->objectsTotal = 0;
    gameWorld->botsTotal = 0;

    auto addToCounters = [&]()
        {
            repeat(NumThreads)
            {
                gameWorld->objectsTotal += counters[i][0];
                gameWorld->botsTotal += counters[i][1];
            }
        };

    //Clear object counters
    clear_counters();

    //Starting signal for all threads
    StartThreads();

    //Wait for threads to synchronize first time
    waitAllThreads();

    //Add object counters
    addToCounters();

    //Clear object counters
    clear_counters();

    //Starting signal for all threads
    StartThreads();

    //Wait for threads to synchronize second time
    waitAllThreads();

    //Add object counters
    addToCounters();

}

//Tick function
void WorldController::tick(uint thisFrame)
{
    tick_single_thread();
    //tick_multiple_threads();
}


void WorldController::PauseThreads()
{
    pauseThreads = true;
}

void WorldController::UnpauseThreads()
{
    pauseThreads = false;
}



//Process function for 4 or 8 threaded simulation
void WorldController::ProcessPart_AlternativeMultipleThreads(const uint X1, const uint Y1, const uint X2, const uint Y2, const uint index) {

    srand(gameWorld->seed + index);
    auto obj_cals = [&](Object* tmpObj) {
        if (tmpObj == NULL) {
            return;

        }

        ++counters[index][0];

        if (tmpObj->type == EnumObjectType::Bot)
            ++counters[index][1];

        ObjectTick(tmpObj);
    };

    for (;;) {

        ThreadWait(index);

        for (int X = X1; X < X1 + ((X2 - X1) / 2); ++X) {
            for (int Y = Y1; Y < Y2; ++Y) {
                obj_cals(gameWorld->allCells[X][Y]);
            }
        }

        threadGoMarker[index] = false;
        ThreadWait(index);
        for (int X = X1 + ((X2 - X1) / 2); X < X2; ++X) {
            for (int Y = Y1; Y < Y2; ++Y) {
                obj_cals(gameWorld->allCells[X][Y]);
            }
        }

        threadGoMarker[index] = false;

        if (terminateThreads) {
            threadTerminated[index] = true;
            return;
        }

    }

}

//Process function for 4 or 8 threaded simulation
void WorldController::ProcessPart_MultipleThreads(const uint X1, const uint Y1, const uint X2, const uint Y2, const uint index) {

    srand(gameWorld->seed + index);
    auto obj_cals = [&](Object* tmpObj) {
        if (tmpObj == NULL) {
            return;

        }

        ++counters[index][0];

        if (tmpObj->type == EnumObjectType::Bot)
            ++counters[index][1];

        ObjectTick(tmpObj);
    };

    for (;;) {
        ThreadWait(index);
        for (int X = X1; X < X1 + ((X2 - X1) / 2); ++X) {
            for (int Y = Y1; Y < Y2; ++Y) {
                obj_cals(gameWorld->allCells[X][Y]);
            }
        }

        threadGoMarker[index] = false;
        ThreadWait(index);
        for (int X = X1 + ((X2 - X1) / 2); X < X2; ++X) {
            for (int Y = Y1; Y < Y2; ++Y) {
                obj_cals(gameWorld->allCells[X][Y]);
            }
        }

        threadGoMarker[index] = false;

        if (terminateThreads) {
            threadTerminated[index] = true;
            return;
        }

    }

}


void WorldController::SpawnControlGroup()
{
    for (int i = 0; i < ControlGroupSize; ++i)
    {
        Bot* tmpBot = new Bot(RandomVal(FieldCellsWidth), RandomVal(FieldCellsHeight), MaxPossibleEnergyForABot);

        if (!gameWorld->AddObject(tmpBot))
            delete tmpBot;
    }
}



WorldController::~WorldController() {
    repeat(NumThreads)
        threadTerminated[i] = false;

    terminateThreads = true;

    for (;;)
    {
        uint tcount = 0;

        repeat(NumThreads)
        {
            if (threadTerminated[i] == true)
                ++tcount;
        }

        if (tcount == NumThreads)
            break;

        repeat(NumThreads)
            threadGoMarker[i] = true;

        pauseThreads = false;

        SDL_Delay(1);
    }

    repeat(NumThreads)
    {
        threads[i]->join();
    }
}



Point WorldController::ScreenCoordsToLocal(int X, int Y)
{
    X -= FieldX;
    Y -= FieldY;

    X /= FieldCellSize;
    Y /= FieldCellSize;

    X += worldRenderer.renderX;

    X = gameWorld->ValidateX(X);

    return { X, Y };
}
