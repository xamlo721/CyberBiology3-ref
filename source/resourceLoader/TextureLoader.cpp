#include "TextureLoader.h"

#include "../SDL.h"

MyTexture* TextureLoader::appleTexture = 0;
MyTexture* TextureLoader::botTexture = 0;
SpriteBotHead* TextureLoader::botSprite = 0;
MyTexture* TextureLoader::organicTexture = 0;


MyTexture* TextureLoader::getAppleTexture() {
    return appleTexture;
}

MyTexture* TextureLoader::getBotTexture() {
    return botTexture;
}

SpriteBotHead* TextureLoader::getBotSprite() {
    return botSprite;
}

MyTexture* TextureLoader::getOrganicTexture() {
    return organicTexture;
}





// что-то на яблочном
void TextureLoader::createAppleTexture() {
    //Create surface
    SDL_Surface* surf = SDL_CreateRGBSurface(0, FieldCellSize, FieldCellSize, 32, R_CHANNEL, G_CHANNEL, B_CHANNEL, A_CHANNEL);


    //Lock
    SDL_LockSurface(surf);

    //Create pixels array
    std::vector<uint32_t> pixels(surf->h * surf->w, 0x00000000);

    //Draw circle

    for (int _x = 0; _x < FieldCellSize; ++_x)
    {
        for (int _y = 0; _y < FieldCellSize; ++_y)
        {
            int
                x_dist = FieldCellSizeHalf - _x,
                y_dist = FieldCellSizeHalf - _y;

            //Pythagorean theorem
            if (x_dist * x_dist + y_dist * y_dist <= FieldCellSizeHalf * FieldCellSizeHalf)
            {
                pixels[(_y * surf->w + _x)] = AppleDrawColorRGBA;
            }
        }
    }

    //Copy pixels on surface
    SDL_memcpy(surf->pixels, pixels.data(), surf->w * surf->h * 4);

    //Unlock
    SDL_UnlockSurface(surf);

    //Set blend mode
    SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_NONE);

    //Create new texture
    appleTexture = new MyTexture("unknownName", SDL_CreateTextureFromSurface(renderer, surf));

    //Destroy surface
    SDL_FreeSurface(surf);

    //Set texture blend mode
    SDL_SetTextureBlendMode(appleTexture->image, SDL_BLENDMODE_BLEND);

}

void TextureLoader::createBotSprite() {

    SDL_Surface* surf;
    SDL_Texture* tmpTexTarget;
    SDL_Texture* tmpTex2;

    SDL_Texture* botHeadSprites[8];

    //Create rotation sprites (outline and head positions)
    repeat(8)
    {
        //Target texture
        tmpTexTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, FieldCellSize, FieldCellSize);

        //Set render target
        SDL_SetRenderTarget(renderer, tmpTexTarget);

        //Clear texture
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        //Render outline
//#ifdef DrawBotOutline
//        SDL_SetRenderDrawColor(renderer, BotOutlineColor);
//        SDL_RenderDrawRect(renderer, &image_rect);
//#endif
//
//        //Render head
//#ifdef DrawBotHead
//        SDL_RenderDrawLine(renderer,
//            FieldCellSizeHalf,
//            FieldCellSizeHalf,
//            FieldCellSizeHalf + Rotations[i].x * FieldCellSizeHalf,
//            FieldCellSizeHalf + Rotations[i].y * FieldCellSizeHalf);
//#endif

        //Create surface
        surf = SDL_CreateRGBSurface(0, FieldCellSize, FieldCellSize, 32, R_CHANNEL, G_CHANNEL, B_CHANNEL, A_CHANNEL);

        //Copy previously rendered texture on surface
        SDL_LockSurface(surf);

        int32_t pixels[FieldCellSize * FieldCellSize];

        SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, surf->pitch);

        SDL_memcpy(surf->pixels, pixels, surf->w * surf->h * 4);

        SDL_UnlockSurface(surf);

        //Create static texture
        tmpTex2 = SDL_CreateTextureFromSurface(renderer, surf);

        //Save texture
        botHeadSprites[i] = tmpTex2;

        //Free target texture and surface
        SDL_DestroyTexture(tmpTexTarget);
        SDL_FreeSurface(surf);
    }

    //Create bot 'body' image (white rectangle)

    //Another surface
    surf = SDL_CreateRGBSurface(0, FieldCellSize, FieldCellSize, 32, R_CHANNEL, G_CHANNEL, B_CHANNEL, A_CHANNEL);
    botSprite = new SpriteBotHead("BotHead", botHeadSprites);

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

void TextureLoader::createOrganicTexture()
{
    SDL_Surface* surf;

    //Create bot 'body' image (white rectangle)

    //Another surface
    surf = SDL_CreateRGBSurface(0, FieldCellSize, FieldCellSize, 32, R_CHANNEL, G_CHANNEL, B_CHANNEL, A_CHANNEL);

    SDL_LockSurface(surf);

    //Clear surface
    SDL_memset(surf->pixels, 0xFF, surf->w * surf->h * 4);

    //Create pixels array
    std::vector<uint32_t> pixels(surf->h * surf->w, 0x00000000);

    //Draw outlined rectangle 

    for (int _x = 1; _x < FieldCellSize - 1; ++_x)
    {
        for (int _y = 1; _y < FieldCellSize - 1; ++_y)
        {
            pixels[(_y * surf->w + _x)] = OrganicWasteOutlineColor;
        }
    }

    for (int _x = 2; _x < FieldCellSize - 2; ++_x)
    {
        for (int _y = 2; _y < FieldCellSize - 2; ++_y)
        {
            pixels[(_y * surf->w + _x)] = OrganicWasteDrawColor;
        }
    }

    //Copy pixels on surface
    SDL_memcpy(surf->pixels, pixels.data(), surf->w * surf->h * 4);

    SDL_UnlockSurface(surf);

    //Create static texture
    organicTexture = new MyTexture("Organic", SDL_CreateTextureFromSurface(renderer, surf));

    //Free surface
    SDL_FreeSurface(surf);

    //Free render target
    SDL_SetRenderTarget(renderer, NULL);

}

void TextureLoader::deleteAllImages() {

    //for (const auto& [key, value] : textures) {
    //    //TODO: Отсветить, что мы убили текстуру


    //    SDL_DestroyTexture(value.image);
    //}

    //for (const auto& [key, value] : botHeads) {
    //    //TODO: Отсветить, что мы убили текстуру

    //    repeat(8)
    //        SDL_DestroyTexture(value.sprite_head[i]);
    //}

    delete appleTexture;
    delete botTexture;
    //Ну цикл, ну и что?
    for (int i = 0; i < 8; i++) {
        delete botSprite->sprite_head[i];
    }
    delete botSprite;
    delete organicTexture;

}