#include "NaturalObjectRenderer.h"

#include "../../SDL.h"
#include "../RenderUtils.h"
#include "../../entity/Bot.h"
#include "../../resourceLoader/TextureLoader.h"

void NaturalObjectRenderer::draw(Object* gameObject) {

    //Used for drawing
    RenderUtils::CalcScreenX(gameObject);
    Color c = gameObject->GetColor();


    SDL_Rect object_rect;
    if (gameObject->isShrinkedObject) {
        object_rect = RenderUtils::CalcObjectRectShrinked(gameObject, 1);
    }
    else {
        object_rect = RenderUtils::CalcObjectRect(gameObject);
    }

    //В конечном итоге мы должны его здесь формализовать и убрать зависимость от EnumObjectType

    switch (gameObject->type) {

        case EnumObjectType::Bot: {
            SDL_Rect object_rect = RenderUtils::CalcObjectRect(gameObject);
            SDL_Texture* sprite_body = TextureLoader::getBotTexture()->image;

            //Draw body
            SDL_SetTextureColorMod(sprite_body, c.r, c.g, c.b);
            SDL_RenderCopy(renderer, sprite_body, &gameObject->image_rect, &object_rect);

            //Draw outlines
            SDL_RenderCopy(renderer, TextureLoader::getBotSprite()->sprite_head[((Bot*)gameObject)->direction], &Object::image_rect, &object_rect);
            break;
        }

        default: {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &object_rect);
            break;
        }
    }

}
