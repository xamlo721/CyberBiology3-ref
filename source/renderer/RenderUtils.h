#pragma once

#include "../MyTypes.h"
#include "../entity/Object.h"

class RenderUtils {

    public:

        static uint CalcScreenX(Object* gameObject);

        static SDL_Rect CalcObjectRect(Object* gameObject);

        static SDL_Rect CalcObjectRectShrinked(Object* gameObject, int shrink);



};

