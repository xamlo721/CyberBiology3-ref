#pragma once
//#include <map>

#include "../resources/Texture.h"
#include "../resources/SpriteBotHead.h"

class TextureLoader {

	private:

		static const Uint32 R_CHANNEL = 0xFF000000;
		static const Uint32 G_CHANNEL = 0x00FF0000;
		static const Uint32 B_CHANNEL = 0x0000FF00;
		static const Uint32 A_CHANNEL = 0x000000FF;

		// остыль
		static MyTexture* appleTexture;
		static MyTexture* botTexture;
		static SpriteBotHead* botSprite;
		static MyTexture* organicTexture;

	public:
		//пока не пойму почему строки тут не работают, будет список статический
		//static std::map<std::string, MyTexture *> textures;
		//static std::map<std::string, SpriteBotHead *> botHeads;


		static MyTexture* getAppleTexture();
		static MyTexture* getBotTexture();
		static SpriteBotHead* getBotSprite();
		static MyTexture* getOrganicTexture();


		static void createAppleTexture();
		static void createBotSprite();
		static void createOrganicTexture();


		static void deleteAllImages();

};

	
	