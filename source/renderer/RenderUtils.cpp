#include "RenderUtils.h"

#include "WorldRenderer.h"

uint RenderUtils::CalcScreenX(Object* gameObject) {

    //��� ������� ���� ���� �����������
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

SDL_Rect RenderUtils::CalcObjectRectShrinked(Object* gameObject, int shrink) {

    return SDL_Rect{
        FieldX + gameObject->screenX * FieldCellSize + shrink,
        FieldY + gameObject->y * FieldCellSize + shrink,
        FieldCellSize - 2 * shrink,
        FieldCellSize - 2 * shrink
    };
}


