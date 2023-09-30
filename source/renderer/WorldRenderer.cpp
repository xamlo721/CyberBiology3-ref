#include "WorldRenderer.h"
#include "../SDL.h"

int WorldRenderer::renderX = 0;


void WorldRenderer::shiftRenderPoint(int cx) {
    renderX += cx;

    if (renderX < 0) {
        renderX = FieldCellsWidth - 1;
    }
    else if (renderX >= FieldCellsWidth) {
        renderX = 0;
    }
}


void WorldRenderer::draw(FieldDynamicParams params) {

    //Background
    SDL_SetRenderDrawColor(renderer, FieldBackgroundColor);
    SDL_RenderFillRect(renderer, &mainRect);

    //Ocean
#ifdef DrawOcean
    SDL_SetRenderDrawColor(renderer, OceanColor);
    oceanRect.y = (FieldHeight + FieldY) - (params.oceanLevel * FieldCellSize);
    oceanRect.h = params.oceanLevel * FieldCellSize;
    SDL_RenderFillRect(renderer, &oceanRect);
#endif

    //Mud layer
#ifdef DrawMudLayer
    SDL_SetRenderDrawColor(renderer, MudColor);
    mudLayerRect.y = (FieldHeight + FieldY) - (params.mudLevel * FieldCellSize);
    mudLayerRect.h = params.mudLevel * FieldCellSize;
    SDL_RenderFillRect(renderer, &mudLayerRect);
#endif

}


bool WorldRenderer::IsInBoundsScreenCoords(int X, int Y)
{
    return ((X >= mainRect.x) && (X <= mainRect.x + mainRect.w) && (Y >= mainRect.y) && (Y <= mainRect.y + mainRect.h));
}

