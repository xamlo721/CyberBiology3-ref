#include "World.h"
#include "../entity/Organics.h"

int World::seed;

//SINGLETON
World* World::instance = 0;

World::World() {

    //Clear array
    memset(allCells, 0, sizeof(Point*) * FieldCellsWidth * FieldCellsHeight);



        //Spawn objects
    #ifdef SpawnControlGroupAtStart
        SpawnControlGroup();
    #endif

    #ifdef SpawnOneAtStart
        Bot* tmpBot = new Bot(80, 60, MaxPossibleEnergyForABot);

        AddObject(tmpBot);
    #endif

        Object::SetPointers((Object***)allCells);

}


Point World::FindFreeNeighbourCell(int X, int Y)
{
    //If this cell is empty
    if (allCells[X][Y] == NULL) {
        return { X,Y };
    }

    //Form an array of nearby free cells

    int tx;
    Point tmpArray[9];
    int i = 0;

    for (int cx = -1; cx < 2; ++cx) {
        for (int cy = -1; cy < 2; ++cy)
        {
            tx = ValidateX(X + cx);

            if (IsInBounds(tx, Y + cy))
            {
                if (allCells[tx][Y + cy] == NULL)
                {
                    tmpArray[i++].Set(tx, Y + cy);
                }
            }
        }
    }

    //Get random free cell from array
    if (i > 0)
    {
        return tmpArray[RandomVal(i)];
    }

    //No free cells nearby
    return { -1, -1 };
}





Point World::FindRandomNeighbourBot(int X, int Y)
{
    int tx;
    Point tmpArray[9];
    int i = 0;

    for (int cx = -1; cx < 2; ++cx)
    {
        for (int cy = -1; cy < 2; ++cy)
        {
            tx = ValidateX(X + cx);

            if (IsInBounds(tx, Y + cy))
            {
                if (allCells[tx][Y + cy] != NULL)
                {
                    if (allCells[tx][Y + cy]->type == EnumObjectType::bot)
                        tmpArray[i++].Set(tx, Y + cy);
                }
            }
        }
    }

    //Get random bot from array
    if (i > 0)
    {
        return tmpArray[RandomVal(i)];
    }

    //No free cells nearby
    return { -1, -1 };
}

int World::MoveObject(int fromX, int fromY, int toX, int toY)
{
    if (!IsInBounds(toX, toY))
        return -2;

    if (allCells[toX][toY])
        return -1;

    Object* tmpObj = allCells[fromX][fromY];

    if (tmpObj)
    {
        allCells[toX][toY] = tmpObj;
        allCells[fromX][fromY] = NULL;

        tmpObj->x = toX;
        tmpObj->y = toY;

        return 0;
    }

    return -3;
}

int World::MoveObject(Object* obj, int toX, int toY)
{
    return MoveObject(obj->x, obj->y, toX, toY);
}





bool World::AddObject(Object* obj)
{
    if (allCells[obj->x][obj->y])
        return false;

    allCells[obj->x][obj->y] = obj;

    return true;
}

void World::RemoveObject(int X, int Y)
{
    Object* tmpO = allCells[X][Y];

    if (tmpO)
    {
        delete tmpO;

        allCells[X][Y] = NULL;
    }
}

void World::RemoveAllObjects()
{
    for (int cx = 0; cx < FieldCellsWidth; ++cx)
    {
        for (int cy = 0; cy < FieldCellsHeight; ++cy)
        {
            RemoveObject(cx, cy);
        }
    }
}

void World::RemoveBot(int X, int Y, int energyVal)
{
    RemoveObject(X, Y);

    if (RandomPercentX10(params.adaptation_organicSpawnRate))
    {
        if (energyVal > 0)
            AddObject(new Organics(X, Y, energyVal));
    }
}

void World::RepaintBot(Bot* b, Color newColor, int differs)
{
    Object* tmpObj;

    for (uint ix = 0; ix < FieldCellsWidth; ++ix)
    {
        for (uint iy = 0; iy < FieldCellsHeight; ++iy)
        {
            tmpObj = allCells[ix][iy];

            if (tmpObj)
            {
                if (tmpObj->type == EnumObjectType::bot)
                {
                    if (((Bot*)tmpObj)->FindKinship(b) >= (NumberOfMutationMarkers - differs))
                    {
                        ((Bot*)tmpObj)->SetColor(newColor);
                    }
                }
            }
        }

    }
}


//Is cell out if bounds?
bool World::IsInBounds(int X, int Y) {
    return ((X >= 0) && (Y >= 0) && (X < FieldCellsWidth) && (Y < FieldCellsHeight));
}

bool World::IsInBounds(Point p) {
    return IsInBounds(p.x, p.y);
}

bool World::IsInWater(int Y) {
    return (Y >= (FieldCellsHeight - params.oceanLevel));
}

bool World::IsInMud(int Y) {
    return (Y >= (FieldCellsHeight - params.mudLevel));
}


Object* World::GetObjectLocalCoords(int X, int Y)
{
    return allCells[X][Y];
}




uint World::GetNumObjects() {
    return objectsTotal;
}

uint World::GetNumBots() {
    return botsTotal;
}

uint World::GetNumApples() {
    return applesTotal;
}

uint World::GetNumOrganics() {
    return organicsTotal;
}





int World::ValidateX(int X)
{
    if (X < 0)
    {
        return X + FieldCellsWidth;
    }
    else if (X >= FieldCellsWidth)
    {
        return (X - FieldCellsWidth);
    }

    return X;
}


bool World::ValidateObjectExistance(Object* obj)
{
    for (uint ix = 0; ix < FieldCellsWidth; ++ix)
    {
        for (uint iy = 0; iy < FieldCellsHeight; ++iy)
        {
            if (allCells[ix][iy] == obj)
                return true;
        }
    }

    return false;
}

