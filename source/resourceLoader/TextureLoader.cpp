#include "TextureLoader.h"

#include "../SDL.h"

MyTexture* TextureLoader::botTexture = 0;


MyTexture* TextureLoader::getBotTexture() {
    return botTexture;
}



void TextureLoader::createBotSprite() {

    SDL_Surface* surf;
    //Create Bot 'body' image (white rectangle)

    //Another surface
    surf = SDL_CreateRGBSurface(0, FieldCellSize, FieldCellSize, 32, R_CHANNEL, G_CHANNEL, B_CHANNEL, A_CHANNEL);

    //Clear surface
    SDL_LockSurface(surf);
    SDL_memset(surf->pixels, 255, surf->w * surf->h * 4);
    SDL_UnlockSurface(surf);

    //Create static texture
    botTexture = new MyTexture("BotBody", SDL_CreateTextureFromSurface(renderer, surf));

    //Free surface
    SDL_FreeSurface(surf);

    //Free render target
    SDL_SetRenderTarget(renderer, NULL);
}

void TextureLoader::deleteAllImages() {


    delete botTexture;


}