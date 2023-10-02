#include "RenderUtils.h"

#include "WorldRenderer.h"

uint RenderUtils::CalcScreenX(Object* gameObject) {

    //Эту помойку тоже надо рефакторить
    uint renX = WorldRenderer::renderX;
    uint objX = gameObject->x;
    gameObject->screenX = objX - renX;

    if (gameObject->screenX < 0) {
        gameObject->screenX += FieldCellsWidth;
    }

    return gameObject->screenX;
}

SDL_Rect RenderUtils::CalcObjectRect(Object* gameObject) {

    return SDL_Rect{
        FieldX + gameObject->screenX * FieldCellSize,
        FieldY + gameObject->y * FieldCellSize,
        FieldCellSize,
        FieldCellSize
    };
}


