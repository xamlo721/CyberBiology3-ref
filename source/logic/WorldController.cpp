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
    //for (int i = 0; i < NumThreads; i++) {
    //    threadGoMarker[i] = false;

    //    const uint xCoord = 0;
    //    const uint yCoord = 0;
    //    const uint areaWidth = (FieldCellsWidth / NumThreads) * (i + 1);
    //    const uint areaHeight = FieldCellsHeight;
    //    //threads[i] = new std::thread(&WorldController::ProcessPart_AlternativeMultipleThreads, this, xCoord, yCoord, areaWidth, areaHeight, i);
    //}

}

void WorldController::ObjectTick(Object* tmpObj) {
    
    //Object destroyed
    if (!tmpObj->isAlive) {
        return;
    }


    CellCluster* cluster = this->gameWorld->getObjectsArround(tmpObj);
    //1) 
    World::lock();
    //2)
    cluster->lock();
    //3)
    World::unlock();
    
    {
        /*This function returns 1 when the object is destroyed. OUTDATED!!!!!!
         You should call it on every simulation tick before you
         call same function in derived class
         Returns:
         0 - all fine
         1 - object destroyed
         2 - nothing to do(last tick frame matches current frame)*/

         
        ((Bot*)tmpObj)->tick();
        BrainOutput actions = ((Bot*)tmpObj)->tmpOut;


        //TODO: Сделать правила для вызова Actions и перевести их на автомат


        //Multiply first
        if (actions.divide > 0) {
            //FIXME: Этот дурдом с созданием объектов решается статическим списком Action в классе бота
            DivideAction action;
            action.onActivate(((Bot*)tmpObj), cluster);

        }

        //Then attack
        if (actions.attack > 0) {
            AttackAction action;
            action.onActivate(((Bot*)tmpObj), cluster);

        }
        else {

            //Rotate after
            if (actions.desired_rotation != (((Bot*)tmpObj)->direction * .1f)) {
                RotateAction action;
                action.onActivate(((Bot*)tmpObj), cluster);
            }

            //Move
            if (actions.move > 0) {
                MoveAction action;
                action.onActivate(((Bot*)tmpObj), cluster);
            }
            //Photosynthesis
            else if (actions.photosynthesis > 0) {
                PhotosintesisAction action;
                action.onActivate(((Bot*)tmpObj), cluster);
            }

        }
        
    }
   
    gameWorld->updateCluster(cluster);
    cluster->unlock();
    delete cluster;

}

//tick function for single threaded build
void WorldController::tick_single_thread() {

    while (gameWorld->hasUnprocessedObject()) {

        Object* tmpObj = gameWorld->getNextUnprocessedObject();

        if (tmpObj) {

            ObjectTick(tmpObj);
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
inline void WorldController::tick_multiple_threads() {

    //auto clear_counters = [&]()
    //    {
    //        repeat(NumThreads)
    //        {
    //            counters[i][0] = 0;
    //            counters[i][1] = 0;
    //            counters[i][2] = 0;
    //            counters[i][3] = 0;
    //        }
    //    };

    //gameWorld->objectsTotal = 0;
    //gameWorld->botsTotal = 0;

    //auto addToCounters = [&]()
    //    {
    //        repeat(NumThreads)
    //        {
    //            gameWorld->objectsTotal += counters[i][0];
    //            gameWorld->botsTotal += counters[i][1];
    //        }
    //    };

    ////Clear object counters
    //clear_counters();

    ////Starting signal for all threads
    //StartThreads();

    ////Wait for threads to synchronize first time
    //waitAllThreads();

    ////Add object counters
    //addToCounters();

    ////Clear object counters
    //clear_counters();

    ////Starting signal for all threads
    //StartThreads();

    ////Wait for threads to synchronize second time
    //waitAllThreads();

    ////Add object counters
    //addToCounters();

}

//Tick function
void WorldController::tick(uint thisFrame) {

    gameWorld->startStep();

    tick_single_thread();

    gameWorld->stopStep();

}


void WorldController::PauseThreads() {
    pauseThreads = true;
}

void WorldController::UnpauseThreads()
{
    pauseThreads = false;
}

void WorldController::SpawnControlGroup()
{
    for (int i = 0; i < ControlGroupSize; ++i)
    {
        Bot* tmpBot = new Bot(RandomVal(FieldCellsWidth-2), RandomVal(FieldCellsHeight), MaxPossibleEnergyForABot);

        if (!gameWorld->addObjectSafetly(tmpBot))
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
