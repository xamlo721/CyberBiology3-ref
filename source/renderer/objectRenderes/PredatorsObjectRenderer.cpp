#include "PredatorsObjectRenderer.h"

#include "../../resourceLoader/TextureLoader.h"
#include "../RenderUtils.h"
#include "../../entity/Bot.h"

void PredatorsObjectRenderer::draw(Object* gameObject) {
    switch (gameObject->type) {

        case EnumObjectType::Bot: {
            RenderUtils::CalcScreenX(gameObject);
            SDL_Rect object_rect = RenderUtils::CalcObjectRect(gameObject);

            SDL_Texture* sprite_body = TextureLoader::getBotTexture()->image;

            //Draw body
            int energySumm = ((Bot*)gameObject)->energyFromPredation + ((Bot*)gameObject)->energyFromPS + ((Bot*)gameObject)->energyFromOrganics;

            if (energySumm < 20)
                SDL_SetTextureColorMod(sprite_body, 180, 180, 180);
            else
                SDL_SetTextureColorMod(sprite_body, 255.0f * ((((Bot*)gameObject)->energyFromPredation * 1.0f) / (energySumm * 1.0f)),
                    255.0f * ((((Bot*)gameObject)->energyFromPS * 1.0f) / (energySumm * 1.0f)), 255.0f * ((((Bot*)gameObject)->energyFromOrganics * 1.0f) / (energySumm * 1.0f)));

            SDL_RenderCopy(renderer, sprite_body, &Object::image_rect, &object_rect);

            break;
        }
        default: {
            draw(gameObject);
            break;
        }
    }
};