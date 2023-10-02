#pragma once

#include "../SDL.h"

class IRenderableObject {

	public: //protected:
		//MyTexture rectangle
		static const Rect image_rect;

		Color color;



		//SpriteBotHead * getSprite_Head();

		//MyTexture * getSprite_Body();

		//MyTexture * getImage();

		virtual Color GetColor() = 0;


};

