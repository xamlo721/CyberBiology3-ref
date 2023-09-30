#include "WorldController.h"
#include "../entity/Apple.h"
#include "../entity/Apple.h"
#include "../entity/Object.h"

//SINGLETON
WorldController* WorldController::instance = 0;

WorldController::WorldController() {
    //Start threads

    //4 threads
    #ifdef UseFourThreads
        repeat(NumThreads)
            threadGoMarker[i] = false;

        for (int i = 0; i < NumThreads; i++) {
            const uint xCoord1 = 0;
            const uint yCoord1 = 0;
            const uint areaWidth1 = FieldCellsWidth / 4;
            const uint areaHeight1 = FieldCellsHeight;
            threads[0] = new std::thread(&Field::ProcessPart_MultipleThreads, this, xCoord, yCoord, areaWidth, areaHeight);

            const uint xCoord2 = FieldCellsWidth / 2;
            const uint yCoord2 = 0;
            const uint areaWidth2 = (FieldCellsWidth / 4) * 3;
            const uint areaHeight2 = FieldCellsHeight;
            threads[0] = new std::thread(&Field::ProcessPart_MultipleThreads, this, xCoord, yCoord, areaWidth, areaHeight);

            const uint xCoord3 = FieldCellsWidth / 4;
            const uint yCoord3 = 0;
            const uint areaWidth3 = FieldCellsWidth / 2;
            const uint areaHeight3 = FieldCellsHeight;
            threads[0] = new std::thread(&Field::ProcessPart_MultipleThreads, this, xCoord, yCoord, areaWidth, areaHeight);

            const uint xCoord4 = (FieldCellsWidth / 4) * 3;
            const uint yCoord4 = 0;
            const uint areaWidth4 = FieldCellsWidth;
            const uint areaHeight4 = FieldCellsHeight;
            threads[0] = new std::thread(&Field::ProcessPart_MultipleThreads, this, xCoord, yCoord, areaWidth, areaHeight);

        }

        threads[0] = new std::thread(&Field::ProcessPart_MultipleThreads, this, 0, 0, FieldCellsWidth / 4, FieldCellsHeight, 0);
        threads[1] = new std::thread(&Field::ProcessPart_MultipleThreads, this, FieldCellsWidth / 2, 0, (FieldCellsWidth / 4) * 3, FieldCellsHeight, 1);
        threads[2] = new std::thread(&Field::ProcessPart_MultipleThreads, this, FieldCellsWidth / 4, 0, FieldCellsWidth / 2, FieldCellsHeight, 2);
        threads[3] = new std::thread(&Field::ProcessPart_MultipleThreads, this, (FieldCellsWidth / 4) * 3, 0, FieldCellsWidth, FieldCellsHeight, 3);

    #endif

        //8 threads
    #ifdef UseEightThreads
        repeat(NumThreads)
            threadGoMarker[i] = false;


        //for (int i = 0; i < NumThreads; i++) {
        //    const uint xCoord = 0;
        //    const uint yCoord = 0;
        //    const uint areaWidth = (FieldCellsWidth / 8) * (i + 1);
        //    const uint areaHeight = FieldCellsHeight;
        //    threads[0] = new std::thread(&WorldController::ProcessPart_AlternativeMultipleThreads, this, xCoord, yCoord, areaWidth, areaHeight, i);
        //}

        threads[0] = new std::thread(&WorldController::ProcessPart_MultipleThreads, this, 0,                         0,  FieldCellsWidth / 8,      FieldCellsHeight, 0);
        threads[1] = new std::thread(&WorldController::ProcessPart_MultipleThreads, this, FieldCellsWidth / 8,       0,  FieldCellsWidth / 4,      FieldCellsHeight, 1);
        threads[2] = new std::thread(&WorldController::ProcessPart_MultipleThreads, this, FieldCellsWidth / 4,       0, (FieldCellsWidth / 8) * 3, FieldCellsHeight, 2);
        threads[3] = new std::thread(&WorldController::ProcessPart_MultipleThreads, this, (FieldCellsWidth / 8) * 3, 0,  FieldCellsWidth / 2,      FieldCellsHeight, 3);
        threads[4] = new std::thread(&WorldController::ProcessPart_MultipleThreads, this, FieldCellsWidth / 2,       0, (FieldCellsWidth / 8) * 5, FieldCellsHeight, 4);
        threads[5] = new std::thread(&WorldController::ProcessPart_MultipleThreads, this, (FieldCellsWidth / 8) * 5, 0, (FieldCellsWidth / 4) * 3, FieldCellsHeight, 5);
        threads[6] = new std::thread(&WorldController::ProcessPart_MultipleThreads, this, (FieldCellsWidth / 4) * 3, 0, (FieldCellsWidth / 8) * 7, FieldCellsHeight, 6);
        threads[7] = new std::thread(&WorldController::ProcessPart_MultipleThreads, this, (FieldCellsWidth / 8) * 7, 0,  FieldCellsWidth,          FieldCellsHeight, 7);
    #endif

}


void WorldController::jumpToFirstBot() {
    Object* obj;

    for (int X = 0; X < FieldCellsWidth; ++X) {
        for (int Y = 0; Y < FieldCellsHeight; ++Y) {
            obj = gameWorld->allCells[X][Y];

            if (obj) {
                if (obj->type == EnumObjectType::bot) {
                    worldRenderer.renderX = X;

                    return;
                }
            }
        }
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



void WorldController::ObjectTick(Object* tmpObj)
{
    int t = tmpObj->tick();

    if (t == 1)
    {
        //Object destroyed
        if (tmpObj->type == EnumObjectType::bot)
            gameWorld->RemoveBot(tmpObj->x, tmpObj->y, tmpObj->energy);
        else
            gameWorld->RemoveObject(tmpObj->x, tmpObj->y);

        return;
    }
}

//tick function for single threaded build
inline void WorldController::tick_single_thread()
{
    Object* tmpObj;

    gameWorld->objectsTotal = 0;
    gameWorld->botsTotal = 0;
    gameWorld->applesTotal = 0;
    gameWorld->organicsTotal = 0;

    for (uint ix = 0; ix < FieldCellsWidth; ++ix)
    {
        for (uint iy = 0; iy < FieldCellsHeight; ++iy)
        {
            tmpObj = gameWorld->allCells[ix][iy];

            if (tmpObj)
            {
                ++gameWorld->objectsTotal;

                if (tmpObj->type == EnumObjectType::bot)
                    ++gameWorld->botsTotal;
                else if (tmpObj->type == EnumObjectType::apple)
                    ++gameWorld->applesTotal;
                else if (tmpObj->type == EnumObjectType::organic_waste)
                    ++gameWorld->organicsTotal;

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
void WorldController::StartThreads()
{
    repeat(NumThreads)
    {
        threadGoMarker[i] = true;
    }
}

//Wait for all threads to finish their calculations
void WorldController::WaitForThreads()
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
    gameWorld->applesTotal = 0;
    gameWorld->organicsTotal = 0;

    auto addToCounters = [&]()
        {
            repeat(NumThreads)
            {
                gameWorld->objectsTotal += counters[i][0];
                gameWorld->botsTotal += counters[i][1];
                gameWorld->applesTotal += counters[i][2];
                gameWorld->organicsTotal += counters[i][3];
            }
        };

    //Clear object counters
    clear_counters();

    //Starting signal for all threads
    StartThreads();

    //Wait for threads to synchronize first time
    WaitForThreads();

    //Add object counters
    addToCounters();

    //Clear object counters
    clear_counters();

    //Starting signal for all threads
    StartThreads();

    //Wait for threads to synchronize second time
    WaitForThreads();

    //Add object counters
    addToCounters();

}

//Tick function
void WorldController::tick(uint thisFrame)
{
    Object::currentFrame = thisFrame;

    if (gameWorld->params.spawnApples)
    {
        if (spawnApplesInterval++ == AppleSpawnInterval)
        {
            SpawnApples();

            spawnApplesInterval = 0;
        }
    }

#ifdef UseOneThread
    tick_single_thread();
#else
    tick_multiple_threads();
#endif
}



void WorldController::SpawnApples()
{
    Object* tmpObj;

    for (uint ix = 0; ix < FieldCellsWidth; ++ix)
    {
        for (uint iy = 0; iy < (FieldCellsHeight - gameWorld->params.oceanLevel); ++iy)
        {

            tmpObj = gameWorld->allCells[ix][iy];

            if (tmpObj == NULL)
            {
                //Take a chance to spawn an apple
                if (RandomPercentX10(SpawnAppleInCellChance))
                {
                    gameWorld->AddObject(new Apple(ix, iy));
                }
            }
        }
    }
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

        if (tmpObj->type == EnumObjectType::bot)
            ++counters[index][1];
        else if (tmpObj->type == EnumObjectType::apple)
            ++counters[index][2];
        else if (tmpObj->type == EnumObjectType::organic_waste)
            ++counters[index][3];

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

        if (tmpObj->type == EnumObjectType::bot)
            ++counters[index][1];
        else if (tmpObj->type == EnumObjectType::apple)
            ++counters[index][2];
        else if (tmpObj->type == EnumObjectType::organic_waste)
            ++counters[index][3];

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
