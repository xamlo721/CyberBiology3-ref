#include "EnergyObjectRenderer.h"

#include "../RenderUtils.h"
#include "../../entity/Bot.h"
#include "../../resourceLoader/TextureLoader.h"

void EnergyObjectRenderer::draw(Object* gameObject) {




    switch (gameObject->type) {

        case EnumObjectType::apple: {

            break;
        }
        case EnumObjectType::bot: {
            RenderUtils::CalcScreenX(gameObject);
            SDL_Rect object_rect = RenderUtils::CalcObjectRect(gameObject);
            SDL_Texture* sprite_body = TextureLoader::getBotTexture()->image;

            //Draw body
            SDL_SetTextureColorMod(sprite_body, 255, (1.0f - ((((Bot*)gameObject)->energy * 1.0f) / (MaxPossibleEnergyForABot * 1.0f))) * 255.0f, 0);
            SDL_RenderCopy(renderer, sprite_body, &Object::image_rect, &object_rect);

            //Draw outlines
            SDL_RenderCopy(renderer, TextureLoader::getBotSprite()->sprite_head[((Bot*)gameObject)->direction], &Object::image_rect, &object_rect);
            break;
        }
        case EnumObjectType::organic_waste: {
            SDL_Rect object_rect = RenderUtils::CalcObjectRect(gameObject);

            RenderUtils::CalcScreenX(gameObject);
            RenderUtils::CalcObjectRectShrinked(gameObject, 2);

            //Draw
            SDL_SetRenderDrawColor(renderer, 255, (1.0f - ((((Bot*)gameObject)->energy * 1.0f) / (MaxPossibleEnergyForABot * .5f))) * 255.0f, 0, 255);
            SDL_RenderFillRect(renderer, &object_rect);
            break;
        }
        case EnumObjectType::rock: {

            break;
        }

        default: {
            draw(gameObject);
            break;
        }
    }
};
