#pragma once

#include "../SDL.h"


class MyTexture {

	public:
		std::string textureName;
		SDL_Texture* image;

		MyTexture(std::string textureName, SDL_Texture* image) 
			: textureName(textureName), image(image) {
			//NO-OP
		}

};