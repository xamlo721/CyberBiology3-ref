#pragma once

#include <string>

#include "gui/GUI.h"

#include "world/EnumSeason.h"

#include "logic/WorldController.h"

#include "renderer/EnumRenderTypes.h"

#include "ObjectSaver.h"

class Simulation {

	private:
		static Simulation* instance;

	public:
		static Simulation* INSTANCE() {
			if (instance == 0) {
				instance = new Simulation();
			}
			return instance;
		}

	public: //FIXME: private

		//Simulation
		int seed, id;
		uint realTPS = 0;
		int limit_interval = 0;
		int limit_ticks_per_second = FPSLimitAtStart;

		WorldController* worldController;
		//Save/load object interface
		ObjectSaver saver;
		ApplicationGUIManager gui;


		uint ticknum = 0;

		TimePoint prevTick;
		TimePoint lastSecondTick;
		uint tpsTickCounter = 0;
		TimePoint currentTick;

		//Seasons
		EnumSeason::Season season = EnumSeason::summer;
		uint changeSeasonCounter = 0;

		//Chart (TODO)
		float chartData_bots[ChartNumValues];
		int chart_numValues = 0;
		int chart_currentPosition = 0;

		int timeBeforeNextDataToChart = AddToChartEvery;



		void ClearChart();
		void AddToChart(float);

		//Bot selection
		Object* selectedObject;

		int cursorBlink = 0;
		bool cursorShow = true;
		int selectionShadowScreen = 0;

		void Deselect();

		int brushSize = DefaultBrushRadius;

		//Log
		ImGuiTextBuffer logText;

		void ClearLog();
		void LogPrint(const char* str, bool newLine = false);
		void LogPrint(int num, bool newLine = true);

		//Save/load

		struct listed_file
		{
			string nameFull;
			string nameShort;
			string fileSize;
			string fullCaption;

			bool isSelected = false;

			bool isWorld;
		};

		std::vector<listed_file> allFilenames;
		listed_file* selectedFile = NULL;

		void LoadFilenames();
		void CreateNewFile();


		void SelectionShadowScreen();

		void ChangeSeason();

		void Pause();

		void ClearWorld();


	public:

		//Set false to pause
		bool simulate = true;

		//Set to true to close the app
		bool terminate = false;


		void MakeStep();



		Simulation();
		~Simulation();


};
