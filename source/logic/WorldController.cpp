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
        threads[i] = new std::thread(&WorldController::tick_multiple_threads, this, i);
    }
    this->pauseThreads = true;
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
        ((Bot*)tmpObj)->tick();
        
        
        BrainOutput actions = ((Bot*)tmpObj)->tmpOut;


        //TODO: Сделать правила для вызова Actions и перевести их на автомат

        /*
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
        if (actions.photosynthesis > 0) {
            PhotosintesisAction action;
            action.onActivate(((Bot*)tmpObj), cluster);
        }
        */
        
    }
   
    gameWorld->updateCluster(cluster);
    cluster->unlock();
    delete cluster;

}

//Start all threads
void WorldController::StartThreads() {
    repeat(NumThreads)
    {
        threadGoMarker[i] = true;
    }
}

void WorldController::PauseThreads() {
    pauseThreads = true;
}

void WorldController::UnpauseThreads() {
    pauseThreads = false;
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
            if (threadGoMarker[i] == true)
                threadsReady++;
        }

        if ((threadsReady == NumThreads) && (!pauseThreads))
            break;
        Sleep(1);
        std::this_thread::yield();

    }
}

//Multithreaded tick function
inline void WorldController::tick_multiple_threads(int threadIndex) {

    while (!terminateThreads) {

        ///Синхронизация начала тика
        threadGoMarker[threadIndex] = false;
        gameWorld->startStep();
        threadGoMarker[threadIndex] = true;
        waitAllThreads();


        ///Синхронизация завершения тика
        threadGoMarker[threadIndex] = false;

        while (gameWorld->hasUnprocessedObject()) {

            Object* tmpObj = gameWorld->getNextUnprocessedObject();

            if (tmpObj) {

                ObjectTick(tmpObj);
                Sleep(100);
            }
        }
        gameWorld->stopStep();

        threadGoMarker[threadIndex] = true;
        //Wait for threads to synchronize first time
        waitAllThreads();
    }


}

//Tick function
void WorldController::tick(uint thisFrame) {


    gameWorld->startStep();

    StartThreads();
    {
        uint threadsReady;

        for (;;)
        {

            threadsReady = 0;

            repeat(NumThreads)
            {
                if (threadGoMarker[i] == true)
                    threadsReady++;
            }

            if (threadsReady == NumThreads) {
                break;
            }

            std::this_thread::yield();

        }
    }
    gameWorld->stopStep();

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



Point WorldController::ScreenCoordsToLocal(int X, int Y) {
    X -= FieldX;
    Y -= FieldY;

    X /= FieldCellSize;
    Y /= FieldCellSize;

    X += worldRenderer.renderX;

    X = gameWorld->ValidateX(X);

    return { X, Y };
}


void WorldController::SpawnControlGroup() {

    for (int i = 0; i < ControlGroupSize; ++i) {

        Bot* tmpBot = new Bot(RandomVal(FieldCellsWidth - 2), RandomVal(FieldCellsHeight), MaxPossibleEnergyForABot);

        if (!gameWorld->addObjectSafetly(tmpBot))
            delete tmpBot;
    }
}
