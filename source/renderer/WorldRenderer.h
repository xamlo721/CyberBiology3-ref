#pragma once

#include "SDL.h"

#include "EnumRenderTypes.h"

#include "../Settings.h"
#include "../MyTypes.h"
#include "../entity/Object.h"

#include "../world/FieldDynamicParams.h"

class WorldRenderer {

    public:

        static int renderX;

        //Rectangles
        const SDL_Rect mainRect = { 
            FieldX , 
            FieldY, 
            FieldWidth, 
            FieldHeight 
        };

        SDL_Rect oceanRect = { 
            FieldX , 
            FieldY + (FieldHeight - (InitialOceanHeight * FieldCellSize)),
            FieldWidth, 
            InitialOceanHeight * FieldCellSize 
        };

        SDL_Rect mudLayerRect = { 
            FieldX , 
            FieldY + (FieldHeight - (InitialMudLayerHeight * FieldCellSize)),
            FieldWidth, InitialMudLayerHeight * FieldCellSize 
        };

        //Draw simulation field with all its objects
        void draw(FieldDynamicParams params);


        void shiftRenderPoint(int cx);

        //Is cell out of bounds, given absolute screen space coordinates
        bool IsInBoundsScreenCoords(int X, int Y);



};

