
//-----------------------------------------------------------------
// 
// 
// Примерно как устроено:
// -Входная точка в файле Main.cpp
// -Все настраиваемые параметры в файле Settings.h в виде дефайнов
// -Все объекты на поле наследуется от Object
// -Neuron.h это нейрон, BotNeuralNet.h - вся нейросеть
// -Field, это класс игрового поля
// 
// Также:
// -NeuralNetRenderer это класс, который рисует мозг бота в отдельном окошке
// -ObjectSaver - сохраняет объекты и мир в файл
// -MyTypes - определение некоторых дополнительных типов данных для удобства
// 
//
//-----------------------------------------------------------------

#include "Simulation.h"
#include "resourceLoader/TextureLoader.h"
#include "devices/MyKeyBoard.h"

#include <iostream>

#if defined(_WIN32)
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow) {
#else
int main(int argc, char* argv[]) {}
#endif

	Simulation simulation;

	InitSDL();

	if (!CreateWindowSDL()) {
		return -2;
	}
	CreateRenderer();

	InitImGUI();

	TextureLoader::createBotSprite();

	//Main loop	
	SDL_Event e;
	const Uint8*  keyboard = SDL_GetKeyboardState(NULL);

	for (;;)
	{
		//Events
		mouseState.wheel = 0;

		while (SDL_PollEvent(&e) != 0) {

			if (e.type == SDL_QUIT) {
				goto exitfor;
			}
			else if (e.type == SDL_MOUSEWHEEL) {
				mouseState.wheel = e.wheel.y;
			}
			else if (e.type == SDL_KEYDOWN) {
				MyKeyBoard::CatchKeyboard(keyboard);
			}
			else if (e.type == SDL_TEXTINPUT) {
				io->AddInputCharacter(*e.text.text);
			}
		}

		//Mouse down event
		if (ReadMouseState()) {
			simulation.gui.MouseClick();
		}

		//Simulation
		if (simulation.simulate)
		{
			simulation.MakeStep();
		}
		else
		{
			//Delay so it would not eat too many resourses while on pause
			SDL_Delay(1);
		}

		simulation.gui.Render();

		if (simulation.terminate) {
			goto exitfor;
		}

	}
exitfor:

	//Clear memory
	TextureLoader::deleteAllImages();

	DeInitImGUI();
	DeInitSDL();

	return 0;
}

