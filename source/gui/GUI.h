#pragma once

#include "SDL.h"
#include "../Settings.h"
#include "../logic/NeuralNetRenderer.h"
#include "../renderer/EnumRenderTypes.h"
#include "../logic/EnumMouseFunction.h"
#include "../MyTypes.h"

void InitImGUI();

void DeInitImGUI();

inline void GUIStartFrame();

class ApplicationGUIManager {


public:

	const SDL_Rect screenRect = { 
		0, 
		0, 
		WindowWidth, 
		WindowHeight 
	};

	//

	//FPS
	int limitFPS = LimitFPSAtStart;
	int fpsInterval;
	uint realFPS = 0;
	uint fpsCounter = 0;
	TimePoint lastTickFps;
	TimePoint lastSecondTickFps;

	EnumRenderTypes::RenderTypes renderType = EnumRenderTypes::RenderTypeAtStart;
	EnumMouseFunction::MouseFunction mouseFunc = EnumMouseFunction::mouse_select;


	//Show initial brain or active brain
	int brainToShow = 0;

	void DrawWindows();

	void HighlightSelection();

	//Windows
	void DrawDemoWindow();
	void DrawMainWindow();
	void DrawSystemWindow();
	void DrawControlsWindow();
	void DrawSelectionWindow();
	void DrawRenderWindow();
	void DrawConsoleWindow();
	void DrawMouseFunctionWindow();
	void DrawAdditionalsWindow();

	//Hidden windows
	void DrawSaveLoadWindow();
	void DrawDangerousWindow();
	void DrawSummaryWindow();
	void DrawAdaptationWindow();
	void DrawChartWindow();
	void DrawBotBrainWindow();

	//Show more windows
	bool showSaveLoad = false;
	bool showDangerous = false;
	bool showBrain = false;
	bool showAdaptation = false;
	bool showChart = false;

	//Neural net renderer
	NeuralNetRenderer nn_renderer;

	void MouseClick();

	void Render();


};