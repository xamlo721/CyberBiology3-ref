#include "EnergyObjectRenderer.h"

#include "../RenderUtils.h"
#include "../../entity/Bot.h"
#include "../../resourceLoader/TextureLoader.h"

void EnergyObjectRenderer::draw(Object* gameObject) {




    switch (gameObject->type) {

        case EnumObjectType::Bot: {
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

        default: {
            draw(gameObject);
            break;
        }
    }
};
