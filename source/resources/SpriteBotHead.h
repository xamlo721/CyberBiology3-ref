#pragma once

#include "../SDL.h"


class SpriteBotHead {

	public:
		std::string textureName;
		SDL_Texture* sprite_head[8];

		SpriteBotHead(std::string textureName, SDL_Texture* sprite_head[8])
			: textureName(textureName){

			//Ну цикл, ну и что?
			for (int i = 0; i < 8; i++) {
				this->sprite_head[i] = sprite_head[i];
			}

		}
};