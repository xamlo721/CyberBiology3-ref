#pragma once
//#pragma message("	Organics_h")

#include "Object.h"

class Organics :public Object {

	protected:
		bool doneFalling = false;

		static SDL_Texture* image;

	public:

		Organics(int X, int Y, int Energy);

		int tick() override;

};

