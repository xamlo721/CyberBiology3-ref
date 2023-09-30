#include "Organics.h"
#include "../world/World.h"

SDL_Texture* Organics::image = NULL;

Organics::Organics(int X, int Y, int Energy) :Object(X, Y, EnumObjectType::organic_waste, false) {
	energy = Energy;
}


int Organics::tick()
{
    int ret = Object::tick();

    if(ret != 0)
        return ret;

    //Organic waste should fall until it hits an obstacle

    int next_y = y + 1;

    //If not done falling
    if (!doneFalling)
    {
        //What is underneath?
        if (World::INSTANCE()->IsInBounds(x, next_y))
        {
            if ((*pCells)[x][next_y] == NULL)
            {
                //Fall
                World::INSTANCE()->MoveObject(this, x, next_y);
            }
            #ifndef OrganicWasteAlwaysFalls
            else                
                doneFalling = true;
            #endif
        }
        else
        {
            doneFalling = true; //Once done it shouldn't fall anymore
        }
    }

    return 0;
}
