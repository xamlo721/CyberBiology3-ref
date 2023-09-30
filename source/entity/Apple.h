#pragma once
//#pragma message("	Apple_h")



#include "Object.h"


class Apple :public Object {

	private:

		static SDL_Texture* image;

	public:

		Apple(int X, int Y, int Energy = 0);

};