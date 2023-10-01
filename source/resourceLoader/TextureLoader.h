#pragma once
//#include <map>

#include "../resources/Texture.h"

class TextureLoader {

	private:

		static const Uint32 R_CHANNEL = 0xFF000000;
		static const Uint32 G_CHANNEL = 0x00FF0000;
		static const Uint32 B_CHANNEL = 0x0000FF00;
		static const Uint32 A_CHANNEL = 0x000000FF;

		// остыль
		static MyTexture* botTexture;

	public:
		//пока не пойму почему строки тут не работают, будет список статический
		//static std::map<std::string, MyTexture *> textures;
		//static std::map<std::string, SpriteBotHead *> botHeads;


		static MyTexture* getBotTexture();


		static void createBotSprite();


		static void deleteAllImages();

};

	
	