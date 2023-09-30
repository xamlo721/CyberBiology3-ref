#include "Simulation.h"

#include "logic/SimulationTimer.h"
#include "world/World.h"

//SINGLETON
Simulation* Simulation::instance = 0;

void Simulation::ChangeSeason() {

	season = (EnumSeason::Season)((int)season + 1);

	//TODO: Вырезать костыль и сделать нормальное переключение
	if (season > EnumSeason::spring) {
		season = EnumSeason::summer;
	}
}


void Simulation::Pause()
{
	simulate = !simulate;

	if (simulate)
	{
		worldController->UnpauseThreads();
	}
	else
	{
		worldController->PauseThreads();
	}
}

void Simulation::MakeStep()
{
	//Simulation step
	currentTick = SimulationTimer::getTime();

	if (limit_ticks_per_second > 0)
	{
		limit_interval = 1000 / limit_ticks_per_second;
	}
	else
	{
		limit_interval = 0;
	}

	if ((TimeMSBetween(currentTick, prevTick) >= limit_interval) || (limit_interval == 0) || (gui.renderType == EnumRenderTypes::noRender))
	{
		prevTick = currentTick;

		worldController->tick(ticknum);

		++ticknum;
		++tpsTickCounter;

#ifdef UseSeasons	
		if (++changeSeasonCounter >= ChangeSeasonAfter)
		{
			ChangeSeason();

			changeSeasonCounter = 0;
		}
#endif

		//Add data to chart
		if (--timeBeforeNextDataToChart == 0)
		{
			AddToChart(worldController->gameWorld->GetNumBots() * 1.0f,
				worldController->gameWorld->GetNumApples() * 1.0f, worldController->gameWorld->GetNumOrganics() * 1.0f);

			timeBeforeNextDataToChart = AddToChartEvery;
		}
	}

	//Calculate simulation speed
	if (TimeMSBetween(currentTick, lastSecondTick) >= 1000)
	{
		lastSecondTick = currentTick;

		realTPS = tpsTickCounter;
		tpsTickCounter = 0;
	}
}

void Simulation::SelectionShadowScreen()
{
	if (selectedObject)
	{
		if (selectionShadowScreen < 200)
			selectionShadowScreen += 5;
	}
	else
	{
		if (selectionShadowScreen > 0)
			selectionShadowScreen -= 5;
	}

	//Cursor blinking
	if (cursorBlink-- == 0)
	{
		cursorBlink = CursorBlinkRate;
		cursorShow = !cursorShow;
	}
}

void Simulation::ClearWorld()
{
	Deselect();

	worldController->gameWorld->RemoveAllObjects();

	ticknum = 0;
}

void Simulation::ClearChart()
{
	memset(chartData_bots, 0, sizeof(chartData_bots));
	memset(chartData_organics, 0, sizeof(chartData_organics));
	memset(chartData_apples, 0, sizeof(chartData_apples));

	chart_numValues = 0;
	chart_currentPosition = 0;
}

void Simulation::AddToChart(float newVal_bots, float newVal_apples, float newVal_organics)
{
	chartData_bots[chart_currentPosition] = newVal_bots;
	chartData_apples[chart_currentPosition] = newVal_apples;
	chartData_organics[chart_currentPosition] = newVal_organics;

	if (chart_numValues < 250)
	{
		++chart_numValues;
		++chart_currentPosition;
	}
	else
	{
		if (chart_currentPosition == ChartNumValues)
			ClearChart();
		else
			++chart_currentPosition;
	}
}

void Simulation::Deselect()
{
	selectedObject = NULL;
	gui.showBrain = false;
	gui.nn_renderer.selectedNeuron = NULL;
}


void Simulation::ClearLog()
{
	logText.clear();
}

void Simulation::LogPrint(const char* str, bool newLine)
{
	logText.append(str);

	if (newLine)
		LogPrint("\r\n");
}

void Simulation::LogPrint(int num, bool newLine)
{
	logText.appendf("%i", num);

	if (newLine)
		LogPrint("\r\n");
}

void Simulation::LoadFilenames()
{
	//Check if folder exists
	if (!std::filesystem::exists(DirectoryName))
	{
		//If not create a folder
		std::filesystem::create_directory(DirectoryName);
	}

	//Load list of filenames
	allFilenames.clear();

	for (const auto& entry : std::filesystem::directory_iterator(DirectoryName))
	{
		//Skip folders
		if (entry.is_directory())
			continue;

		listed_file f;

		//Full paths to files
		f.nameFull = entry.path().string();

		//Only file name
		f.nameShort = entry.path().filename().string();

		//File size
		uint size = entry.file_size();
		string unit;

		//Units
		if (size > 1000000)
		{
			size /= 1000000;
			unit += "MB";
		}
		else if (size > 1000)
		{
			size /= 1000;
			unit += "KB";
		}
		else
		{
			unit += "b";
		}

		f.fileSize += std::to_string(size);
		f.fileSize += unit;

		//Is world (open file briefly and look for file type)
		MyInputStream file((char*)f.nameFull.c_str(), static_cast<std::ios_base::openmode>(
			std::ios::in | std::ios::binary | std::ios::beg));

		if (!file.is_open())
			continue;

		if (size > 0)
			f.isWorld = (file.ReadInt() == MagicNumber_WorldFile);
		else
			f.isWorld = false;

		file.close();

		//Full file description
		f.fullCaption = f.nameShort;
		f.fullCaption.resize(25, ' ');
		f.fullCaption += f.fileSize;
		f.fullCaption.resize(40, ' ');
		f.fullCaption += (f.isWorld) ? ("[world]") : ("");

		allFilenames.push_back(f);
	}
}




void Simulation::CreateNewFile()
{
	std::string fileName = "New1";
	int fileCounter = 1;

	for (;;)
	{
		if (std::filesystem::exists(DirectoryName + fileName))
		{
			fileName = "New" + std::to_string(++fileCounter);
		}
		else
		{
			break;
		}
	}

	std::ofstream file(DirectoryName + fileName);
	file.close();

	LoadFilenames();
}


Simulation::Simulation()
{
#if !defined(_WIN32)

	auto GetTickCount = []() {
		struct timespec ts;
		if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
			exit(-1);
		}
		uint32_t theTick = ts.tv_nsec / 1000000;
		theTick += ts.tv_sec * 1000;
		return theTick;
		};

#endif

	LogPrint((char*)"Started. Seed:\r\n");

	//Set seed
#ifdef RandomSeed		
	seed = GetTickCount();
#else
	seed = Seed;
#endif

	worldController = new WorldController();
	worldController->gameWorld->seed = seed;
	srand(seed);

	id = rand();

	LogPrint(seed);



#ifdef StartOnPause
	Pause();
#endif

	LoadFilenames();

	gui.keyboard = SDL_GetKeyboardState(NULL);
}

Simulation::~Simulation() {
	delete worldController;
}
