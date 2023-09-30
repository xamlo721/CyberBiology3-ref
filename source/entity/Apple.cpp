#include "Apple.h"

#include "../world/World.h"

SDL_Texture* Apple::image = NULL;


Apple::Apple(int X, int Y, int Energy) : Object(X, Y, EnumObjectType::apple, false) {
    energy = Energy;

    if (energy == 0)
        energy = World::INSTANCE()->params.appleEnergy;

}
