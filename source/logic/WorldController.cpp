#include "WorldController.h"
#include "../entity/Object.h"

#include "../entity/actions/RotateAction.h"
#include "../entity/actions/AttackAction.h"
#include "../entity/actions/DivideAction.h"
#include "../entity/actions/MoveAction.h"
#include "../entity/actions/PhotosintesisAction.h"

//SINGLETON
WorldController* WorldController::instance = 0;

WorldController::WorldController() : MyThreadLoop(NumThreads) {
    gameWorld = World::INSTANCE();
}

void WorldController::ObjectTick(Bot* tmpObj) {
    
    //Object destroyed
    if (!tmpObj->isAlive) {
        return;
    }
    
    CellCluster* cluster = this->gameWorld->getLockedCluster(tmpObj);
    {

        tmpObj->tick();

        BrainInput input;
        {
            Cell* visibleCell = cluster->getCellByLocalCoord(tmpObj->lookAt.x, tmpObj->lookAt.y);

            //Destination cell is empty
            if (visibleCell->objectType == EnumObjectType::Empty) {
                //0 if empty
                input.vision = 0.0f;
            } else if (visibleCell->objectType == EnumObjectType::Bot)  {
                    //0.5 if someone is in that cell
                    input.vision = 1.0f;
            }

            input.age = (tmpObj->lifetime * 1.0f) / (MaxBotLifetime * 1.0f);

            //input.rotation = (actions.desired_rotation == (direction * .1f))?1.0f:0.0f;
            input.rotation = (tmpObj->direction * 1.0f) / 7.0f;
        }
        tmpObj->actions = tmpObj->think(input);


        //TODO: Сделать правила для вызова Actions и перевести их на автомат

        /*
        //Multiply first
        if (tmpObj->actions.divide > 0) {
            //FIXME: Этот дурдом с созданием объектов решается статическим списком Action в классе бота
            DivideAction action;
            action.onActivate(((Bot*)tmpObj), cluster);
        }
        
        //Then attack
        if (tmpObj->actions.attack > 0) {
            AttackAction action;
            action.onActivate(((Bot*)tmpObj), cluster);

        }
         
        //Rotate after
        if (tmpObj->actions.desired_rotation != (((Bot*)tmpObj)->direction * .1f)) {
            RotateAction action;
            action.onActivate(((Bot*)tmpObj), cluster);
        }
        
        //Move
        if (tmpObj->actions.move > 0) {
            MoveAction action;
            action.onActivate(((Bot*)tmpObj), cluster);
        }
        
        //Photosynthesis    
        if (tmpObj->actions.photosynthesis > 0) {
            PhotosintesisAction action;
            action.onActivate(((Bot*)tmpObj), cluster);
        }*/
        
        
    }
    delete cluster;

}

std::mutex mutex;

void WorldController::onTickStated() {
    auto lck = std::scoped_lock{ mutex };
    gameWorld->startStep();
}

void WorldController::processTick(int threadIndex) {
    int startedHeight = (FieldCellsHeight / NumThreads) * threadIndex;
    int endHeight = (FieldCellsHeight / NumThreads) * (threadIndex + 1);

    //Ну тогда оно должно делиться на цело
    for (int widthIndex = 0; widthIndex < FieldCellsWidth; widthIndex++) {

        for (int heightIndex = startedHeight; heightIndex < endHeight; heightIndex++) {


            Cell* cell = gameWorld->getCellPointer(widthIndex, heightIndex);

            if (cell->objectType == EnumObjectType::Bot && cell->object != NULL) {

                Bot* tmpObj = (Bot *)gameWorld->GetObjectLocalCoords(widthIndex, heightIndex);

                if (tmpObj->x > 1000 || tmpObj->x < -1000) {
                    throw WorldController();
                }



                ObjectTick(tmpObj);
                //Sleep(10);
            }
        }
    }
}

void WorldController::onTickEnded() {
    auto lck = std::scoped_lock{ mutex };
    gameWorld->stopStep();
}


//Tick function
void WorldController::tick(uint thisFrame) {


    //gameWorld->startStep();

    //StartThreads();
    //{
    //    uint threadsReady;

    //    for (;;)
    //    {

    //        threadsReady = 0;

    //        repeat(NumThreads)
    //        {
    //            if (threadGoMarker[i] == true)
    //                threadsReady++;
    //        }

    //        if (threadsReady == NumThreads) {
    //            break;
    //        }

    //        std::this_thread::yield();

    //    }
    //}
    //gameWorld->stopStep();

}



WorldController::~WorldController() {

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
