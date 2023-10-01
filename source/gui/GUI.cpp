
#include "GUI.h"

#include "../Simulation.h"
#include "../logic/SimulationTimer.h"
#include "../renderer/ObjectRenderer.h"


#include "../renderer/objectRenderes/IObjectRenderer.h"
#include "../renderer/objectRenderes/EnergyObjectRenderer.h"
#include "../renderer/objectRenderes/NaturalObjectRenderer.h"
#include "../renderer/objectRenderes/PredatorsObjectRenderer.h"

void InitImGUI()
{
	ImGui::CreateContext();

	io = &ImGui::GetIO();

	ImPlot::CreateContext();

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	//Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);
}

void DeInitImGUI()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}



void GUIStartFrame()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}



void ApplicationGUIManager::DrawDemoWindow() {

	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowPos({ 20.0f,20.0f });

	ImGui::ShowDemoWindow();
}

void ApplicationGUIManager::DrawMainWindow() {

	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize({ GUIWindowWidth * 1.0f, 135.0f });
	ImGui::SetNextWindowPos({ (2 * FieldX + FieldWidth) * 1.0f, InterfaceBorder * 1.0f });

	ImGui::Begin("Main", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		//FPS text 
		ImGui::Text("steps: %i", Simulation::INSTANCE()->ticknum);
		ImGui::Text("(interval %i, ticks/sec: %i, fps: %i)", Simulation::INSTANCE()->limit_interval, Simulation::INSTANCE()->realTPS, realFPS);
		ImGui::Text("Objects total: %i", Simulation::INSTANCE()->worldController->gameWorld->GetNumObjects());
		ImGui::Text("Bots total: %i", Simulation::INSTANCE()->worldController->gameWorld->GetNumBots());

		//Show season name
		/*
		switch (season)
		{
		case summer:
			ImGui::Text("Season: Summer");
			break;
		case autumn:
			ImGui::Text("Season: Autumn");
			break;
		case winter:
			ImGui::Text("Season: Winter");
			break;
		case spring:
			ImGui::Text("Season: Spring");
			break;
		}

		ImGui::SameLine();
		ImGui::Text(" ( %i )", ChangeSeasonInterval - changeSeasonCounter);*/

		//Neural net params and FOV x
		ImGui::Text("Layers: %i, Neurons: %i, Render_X: %i", NumNeuronLayers, NeuronsInLayer, Simulation::INSTANCE()->worldController->worldRenderer.renderX);

		//Simulation seed and unique id
		ImGui::Text("Seed: %i, simulation id: %i", Simulation::INSTANCE()->seed, Simulation::INSTANCE()->id);

	}
	ImGui::End();
}


void ApplicationGUIManager::DrawSystemWindow() {

	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize({ GUIWindowWidth * 1.0f, 70.0f });
	ImGui::SetNextWindowPos({ (2 * FieldX + FieldWidth) * 1.0f, 140.0f });

	ImGui::Begin("System", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
		ImGui::SameLine();
		ImGui::Text(" %s", SDL_GetPlatform());

		ImGui::SameLine();

		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "CPU cores: %d", SDL_GetCPUCount());

		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

		ImGui::SameLine();

		//Show number of threads
		#ifdef UseOneThread
		ImGui::Text(", 1 thread used");
		#endif

		#ifdef UseFourThreads
		ImGui::Text(", 4 threads used");
		#endif

		#ifdef UseEightThreads
		ImGui::Text(", 8 threads used");
		#endif
	}
	ImGui::End();
}

void ApplicationGUIManager::DrawControlsWindow()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize({ GUIWindowWidth * 1.0f, 160.0f });
	ImGui::SetNextWindowPos({ (2 * FieldX + FieldWidth) * 1.0f, 220.0f });

	ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		if (ImGui::Button((Simulation::INSTANCE()->simulate) ? "stop" : "start", { 200, 25 }))
		{
			Simulation::INSTANCE()->Pause();
		}

		//Sliders
		ImGui::PushItemWidth(200);
		ImGui::SliderInt("limit TPS", &Simulation::INSTANCE()->limit_ticks_per_second, 0, GUI_Max_tps, "%d");
		ImGui::SliderInt("limit FPS", &limitFPS, 0, GUI_Max_fps, "%d");

		ImGui::SliderInt("PS reward", &(Simulation::INSTANCE()->worldController->gameWorld->photosynthesisReward), 0, GUI_Max_food);
		ImGui::SliderInt("brush", &Simulation::INSTANCE()->brushSize, GUI_Max_brush, 0, "%d");
	}
	ImGui::End();
}

void ApplicationGUIManager::DrawSelectionWindow()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize({ GUIWindowWidth * 1.0f, 150.0f });
	ImGui::SetNextWindowPos({ (2 * FieldX + FieldWidth) * 1.0f, 390.0f });

	ImGui::Begin("Selection", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		if (Simulation::INSTANCE()->selectedObject)
		{
			if (Simulation::INSTANCE()->worldController->gameWorld->ValidateObjectExistance(Simulation::INSTANCE()->selectedObject))
			{
				//Object info
				ImGui::Text("type: Bot	X: %i, Y: %i", Simulation::INSTANCE()->selectedObject->x, Simulation::INSTANCE()->selectedObject->y);
				ImGui::Text("lifetime: %i / %i", Simulation::INSTANCE()->selectedObject->GetLifetime(), MaxBotLifetime);
				ImGui::Text("energy: %i (PS: %i, predation: %i)", Simulation::INSTANCE()->selectedObject->energy, ((Bot*)Simulation::INSTANCE()->selectedObject)->GetEnergyFromPS(), ((Bot*)Simulation::INSTANCE()->selectedObject)->GetEnergyFromKills());

				//Mutation markers
				int m[NumberOfMutationMarkers];
				memcpy(m, ((Bot*)Simulation::INSTANCE()->selectedObject)->GetMarkers(), sizeof(m));
				ImGui::Text("markers: {");

				repeat(NumberOfMutationMarkers)
				{
					ImGui::SameLine();
					ImGui::Text("%i", m[i]);
				}

				ImGui::SameLine();
				ImGui::Text("}");

				//Color
				Uint8 c[3];

				c[0] = Simulation::INSTANCE()->selectedObject->GetColor().r;
				c[1] = Simulation::INSTANCE()->selectedObject->GetColor().g;
				c[2] = Simulation::INSTANCE()->selectedObject->GetColor().b;
				
				//memcpy(c, &((Bot*)Simulation::INSTANCE()->selectedObject)->GetColor(), sizeof(c));
				
				ImGui::Text("color: {%i, %i, %i}", c[0], c[1], c[2]);

				ImGui::SameLine();
				ImGui::TextColored(ImVec4(((c[0] * 1.0f) / 255.0f), ((c[1] * 1.0f) / 255.0f), ((c[2] * 1.0f) / 255.0f), 1.0f), "*****");

				ImGui::SameLine();
				if (ImGui::Button("Repaint", { 50, 20 }))
				{
					Simulation::INSTANCE()->worldController->gameWorld->RepaintBot((Bot*)Simulation::INSTANCE()->selectedObject, Bot::GetRandomColor(), 1);
				}

				if (ImGui::Button("Show brain", { 100, 25 }))
				{
					showBrain = !showBrain;
				}
			}
			else
			{
				Simulation::INSTANCE()->Deselect();
			}
		}
	}
	ImGui::End();
}

void ApplicationGUIManager::DrawRenderWindow()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize({ GUIWindowWidth * 1.0f, 140.0f});
	ImGui::SetNextWindowPos({ (2 * FieldX + FieldWidth) * 1.0f, 550.0f });

	ImGui::Begin("Display", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::BeginGroup();

		ImGui::Text("Render:");

		ImGui::RadioButton("Natural colors", (int*)&renderType, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Predators", (int*)&renderType, 1);

		ImGui::RadioButton("Energy", (int*)&renderType, 2);
		ImGui::SameLine();
		ImGui::RadioButton("No render", (int*)&renderType, 3);

		ImGui::EndGroup();
	}
	ImGui::End();
}

void ApplicationGUIManager::DrawConsoleWindow()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize({ GUIWindowWidth * 1.0f, 120.0f });
	ImGui::SetNextWindowPos({ (2 * FieldX + FieldWidth) * 1.0f, 700.0f});

	ImGui::Begin("Log", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(LogBackgroundColor));

		ImGui::BeginChild("scrolling", ImVec2(240, 80), true);
		{
			ImGui::TextUnformatted(Simulation::INSTANCE()->logText.Buf.Data);

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
		}
		ImGui::EndChild();

		ImGui::PopStyleColor();
	}
	ImGui::End();
}

void ApplicationGUIManager::DrawMouseFunctionWindow()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize({ GUIWindowWidth * 1.0f, 130.0f});
	ImGui::SetNextWindowPos({ (2 * FieldX + FieldWidth) * 1.0f, 830.0f });

	ImGui::Begin("Mouse function", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::BeginGroup();

		ImGui::RadioButton("Select", (int*)&mouseFunc, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Remove", (int*)&mouseFunc, 1);

		ImGui::RadioButton("Place rock", (int*)&mouseFunc, 2);
		ImGui::SameLine();
		ImGui::RadioButton("From file", (int*)&mouseFunc, 3);

		ImGui::RadioButton("Mutate", (int*)&mouseFunc, 4);

		ImGui::EndGroup();
	}
	ImGui::End();
}

void ApplicationGUIManager::DrawAdditionalsWindow()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowSize({ GUIWindowWidth * 1.0f, 100.0f });
	ImGui::SetNextWindowPos({ (2 * FieldX + FieldWidth) * 1.0f, 970.0f });

	ImGui::Begin("Additional windows", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{

		if (ImGui::Button("Save/load", { 75, 30 }))
		{
			Simulation::INSTANCE()->LoadFilenames();

			showSaveLoad = !showSaveLoad;
		}
		ImGui::SameLine();

		if (ImGui::Button("Dangerous", { 75, 30 }))
		{
			showDangerous = !showDangerous;
		}
		ImGui::SameLine();

		if (ImGui::Button("Adaptation", { 75, 30 }))
		{
			showAdaptation = !showAdaptation;
		}

		if (ImGui::Button("Chart", { 75, 30 }))
		{
			showChart = !showChart;
		}

	}
	ImGui::End();
}

void ApplicationGUIManager::DrawSaveLoadWindow()
{
	if (showSaveLoad)
	{
		//Save/load window
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::SetNextWindowSize({ 400.0f, 200.0f });
		ImGui::SetNextWindowPos({ 100 * 1.0f, 100.0f }, ImGuiCond_Once);

		ImGui::Begin("Save/load", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			//List of files
			ImGui::Text("Select file");

			ImGui::ListBoxHeader("##", ImVec2(380, 110));

			for (int i = 0; i < Simulation::INSTANCE()->allFilenames.size(); ++i)
			{
				if (ImGui::Selectable(Simulation::INSTANCE()->allFilenames[i].fullCaption.c_str(), &Simulation::INSTANCE()->allFilenames[i].isSelected))
				{
					for (int b = 0; b < Simulation::INSTANCE()->allFilenames.size(); ++b)
						Simulation::INSTANCE()->allFilenames[b].isSelected = false;

					Simulation::INSTANCE()->allFilenames[i].isSelected = true;
					Simulation::INSTANCE()->selectedFile = &Simulation::INSTANCE()->allFilenames[i];
				}
			}

			ImGui::ListBoxFooter();

			//Buttons

			if (ImGui::Button("Load", { 50, 30 }))
			{
				if (Simulation::INSTANCE()->selectedFile)
				{
					if (Simulation::INSTANCE()->selectedFile->isWorld)
					{
						ObjectSaver::WorldParams ret = Simulation::INSTANCE()->saver.LoadWorld(Simulation::INSTANCE()->worldController->gameWorld, (char*)Simulation::INSTANCE()->selectedFile->nameFull.c_str());

						if (ret.id != -1)
						{
							if (ret.width != FieldCellsWidth)
								Simulation::INSTANCE()->LogPrint("World loaded(width mismatch)\r\n");
							else
								Simulation::INSTANCE()->LogPrint("World loaded\r\n");

							Simulation::INSTANCE()->seed = ret.seed;
							Simulation::INSTANCE()->ticknum = ret.tick;
							Simulation::INSTANCE()->id = ret.id;

							Simulation::INSTANCE()->worldController->gameWorld->seed = Simulation::INSTANCE()->seed;
						}
						else
							Simulation::INSTANCE()->LogPrint("Error while loading world\r\n");
					}
					else
					{
						if (Simulation::INSTANCE()->selectedObject)
						{
							delete Simulation::INSTANCE()->selectedObject;
						}

						Simulation::INSTANCE()->selectedObject = Simulation::INSTANCE()->saver.LoadObject((char*)Simulation::INSTANCE()->selectedFile->nameFull.c_str());

						if (Simulation::INSTANCE()->selectedObject)
							Simulation::INSTANCE()->LogPrint("Object loaded\r\n");
						else
							Simulation::INSTANCE()->LogPrint("Error while loading object\r\n");
					}
				}
			}			

			ImGui::SameLine();

			if (ImGui::Button("Save bot", { 100, 30 }))
			{
				if (Simulation::INSTANCE()->selectedObject)
				{
					if (Simulation::INSTANCE()->selectedFile)
					{
						if (Simulation::INSTANCE()->saver.SaveObject(Simulation::INSTANCE()->selectedObject, (char*)Simulation::INSTANCE()->selectedFile->nameFull.c_str()))
						{
							Simulation::INSTANCE()->LogPrint("Object saved\r\n");

							Simulation::INSTANCE()->LoadFilenames();
						}
						else
						{
							Simulation::INSTANCE()->LogPrint("Error while saving object\r\n");
						}
					}
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Save world", { 100, 30 }))
			{
				if (Simulation::INSTANCE()->selectedFile)
				{
					if (Simulation::INSTANCE()->saver.SaveWorld(Simulation::INSTANCE()->worldController->gameWorld, (char*)Simulation::INSTANCE()->selectedFile->nameFull.c_str(), Simulation::INSTANCE()->id, Simulation::INSTANCE()->ticknum))
					{
						Simulation::INSTANCE()->LogPrint("World saved\r\n");

						Simulation::INSTANCE()->LoadFilenames();
					}
					else
					{
						Simulation::INSTANCE()->LogPrint("Error while saving world\r\n");
					}
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("New file", { 100, 30 }))
			{
				Simulation::INSTANCE()->CreateNewFile();

				Simulation::INSTANCE()->LoadFilenames();
			}
		}
		ImGui::End();
	}
}

void ApplicationGUIManager::DrawDangerousWindow()
{
	if (showDangerous)
	{
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::SetNextWindowSize({ 290.0f, 100.0f });
		ImGui::SetNextWindowPos({ 100 * 1.0f, 300.0f }, ImGuiCond_Once);

		ImGui::Begin("Use with caution!", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{

			if (ImGui::Button("Wipe world", { 130, 30 }))
			{
				Simulation::INSTANCE()->ClearWorld();
			}

			ImGui::SameLine();

			if (ImGui::Button("Kill bots", { 130, 30 }))
			{
				Simulation::INSTANCE()->Deselect();

				for (int cx = 0; cx < FieldCellsWidth; ++cx)
				{
					for (int cy = 0; cy < FieldCellsHeight; ++cy)
					{
						Object* o = Simulation::INSTANCE()->worldController->gameWorld->GetObjectLocalCoords(cx, cy);

						if (o == NULL)
							continue;

						if (o->type == EnumObjectType::Bot)
							Simulation::INSTANCE()->worldController->gameWorld->RemoveObject(cx, cy);
					}
				}
			}

			if (ImGui::Button("CloseApp", { 130, 30 }))
			{
				Simulation::INSTANCE()->terminate = true;
			}

			ImGui::SameLine();

			if (ImGui::Button("Zero time", { 130, 30 }))
			{
				Simulation::INSTANCE()->ticknum = 0;
			}
		}
		ImGui::End();
	}
}

void ApplicationGUIManager::DrawSummaryWindow()
{
	if (showBrain)
	{
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::SetNextWindowSize({ 330.0f, 180.0f });
		ImGui::SetNextWindowPos({ 100 * 1.0f, 150.0f }, ImGuiCond_Once);

		ImGui::Begin("Bot summary", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			if (Simulation::INSTANCE()->selectedObject)
			{
				if (Simulation::INSTANCE()->worldController->gameWorld->ValidateObjectExistance(Simulation::INSTANCE()->selectedObject))
				{
					auto summ = ((Bot*)Simulation::INSTANCE()->selectedObject)->GetNeuronSummary();

					ImGui::Text("Neurons:");
					ImGui::Text("Simple: %i", summ.simple);
					ImGui::Text("Radial basis: %i", summ.radialBasis);
					ImGui::Text("Random: %i", summ.random);
					ImGui::Text("Memory: %i", summ.memory);

					ImGui::NewLine();

					ImGui::Text("Total neurons: %i, dead end neurons: %i", summ.neurons, summ.deadend);
					ImGui::Text("Total connections: %i", summ.connections);
				}
				else
					goto Nothing;

			}
			else
			{
			Nothing:
				ImGui::Text("Nothing is selected");
			}

		}
		ImGui::End();
	}
}

void ApplicationGUIManager::DrawAdaptationWindow()
{
	if (showAdaptation)
	{
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::SetNextWindowSize({ 500.0f, 500.0f });
		ImGui::SetNextWindowPos({ 100 * 1.0f, 250.0f }, ImGuiCond_Once);

		ImGui::Begin("Adaptation", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			if(ImGui::CollapsingHeader("Winds"))			
			{
				ImGui::SliderInt("Phase", &Simulation::INSTANCE()->worldController->gameWorld->params.adaptation_DeathChance_Winds, 0, 1000);
				ImGui::SliderInt("Steps", &Simulation::INSTANCE()->worldController->gameWorld->params.adaptation_StepsNum_Winds, 0, 20);
			}

			ImGui::NewLine();
			
			if (ImGui::CollapsingHeader("Divers", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderInt("Land mulpitly block", &Simulation::INSTANCE()->worldController->gameWorld->params.adaptation_landBirthBlock, 0, 1000);
				ImGui::SliderInt("Sea mulpitly block", &Simulation::INSTANCE()->worldController->gameWorld->params.adaptation_seaBirthBlock, 0, 1000);

				ImGui::NewLine();

				ImGui::SliderInt("no PS in ocean", &Simulation::INSTANCE()->worldController->gameWorld->params.adaptation_PSInOceanBlock, 0, 1000, "%d");
				ImGui::SliderInt("no PS in mud", &Simulation::INSTANCE()->worldController->gameWorld->params.adaptation_PSInMudBlock, 0, 1000, "%d");
				ImGui::SliderInt("On land at least once", &Simulation::INSTANCE()->worldController->gameWorld->params.adaptation_botShouldBeOnLandOnceToMultiply, 0, 1000, "%d");
				ImGui::SliderInt("On land PS at least once", &Simulation::INSTANCE()->worldController->gameWorld->params.adaptation_botShouldDoPSOnLandOnceToMultiply, 0, 1000, "%d");
				ImGui::SliderInt("Force movements Y", &Simulation::INSTANCE()->worldController->gameWorld->params.adaptation_forceBotMovements, 0, 1000);

				ImGui::NewLine();

				ImGui::SliderInt("Ocean level", &Simulation::INSTANCE()->worldController->gameWorld->params.oceanLevel, 0, FieldCellsHeight);
				ImGui::SliderInt("Mud level", &Simulation::INSTANCE()->worldController->gameWorld->params.mudLevel, 0, FieldCellsHeight);
			}


			ImGui::NewLine();

			if (ImGui::Button("Reset", { 70, 20 }))
			{
				Simulation::INSTANCE()->worldController->gameWorld->params.Reset();
			}
		}
		ImGui::End();
	}
}

void ApplicationGUIManager::DrawChartWindow()
{
	if (showChart)
	{
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::SetNextWindowSize({ 900.0f, 600.0f });
		ImGui::SetNextWindowPos({ 700.0f, 250.0f }, ImGuiCond_Once);

		ImGui::Begin("Population chart", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			if (ImPlot::BeginPlot("Objects", { 800, 550 }))
			{
				
				//Axes
				ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, 250.0, ImPlotCond_Always);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 26000.0);

				ImPlot::SetupAxis(ImAxis_X1, "Time");
				ImPlot::SetupAxis(ImAxis_Y1, "Number");

				//Bots number
				ImPlot::SetNextLineStyle({ 1, 0, 0, 1 }, ChartLineThickness);

				ImPlot::PlotLine("Bots", Simulation::INSTANCE()->chartData_bots, Simulation::INSTANCE()->chart_numValues - 1, 1.0f, 0.0f, ImPlotLineFlags_None);

				ImPlot::EndPlot();
			}

			ImGui::SameLine();

			ImGui::BeginGroup();			

			if (ImGui::Button("Clear", { 70.0f, 30.0f }))
				Simulation::INSTANCE()->ClearChart();

			ImGui::EndGroup();
		}
		ImGui::End();
	}
}

void ApplicationGUIManager::DrawBotBrainWindow()
{	
	if (showBrain)
	{
		if (Simulation::INSTANCE()->selectedObject)
		{
			//Bot brain window
			ImGui::SetNextWindowBgAlpha(1.0f);
			ImGui::SetNextWindowSize({ 330.0f, 240.0f });
			ImGui::SetNextWindowPos({ 650 * 1.0f, 350.0f });

			ImGui::Begin("Bot brain data", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			{
				ImGui::BeginGroup();

				ImGui::RadioButton("Active brain", &brainToShow, 0);
				ImGui::RadioButton("Initial brain", &brainToShow, 1);

				ImGui::EndGroup();

				if (nn_renderer.selectedNeuron)
				{
					//Show neuron type
					ImGui::Text("Neuron type:");
					ImGui::SameLine();
					ImGui::Text(Neuron::GetTextFromType(nn_renderer.selectedNeuron->type));

					//Show bias
					ImGui::Text("Bias: %f", nn_renderer.selectedNeuron->bias);

					//Show connections
					repeat(nn_renderer.selectedNeuron->numConnections)
					{
						ImGui::Text("Connection to l: %i, n: %i, weight: %f", nn_renderer.selectedNeuron->allConnections[i].dest_layer,
							nn_renderer.selectedNeuron->allConnections[i].dest_neuron, nn_renderer.selectedNeuron->allConnections[i].weight);
					}

					//Show memory data
					//ImGui::Text("Memory data: %i", nn_renderer.selectedBrain->allMemory[][]);

					//TODO
                    if((nn_renderer.selectedNeuron->type != NeuronType::input)
                            && (nn_renderer.selectedNeuron->type != NeuronType::output))
					{
						if (ImGui::Button("set random", { 100,30 }))
						{
							nn_renderer.selectedNeuron->SetRandom();
						}

						ImGui::SameLine();

						if (ImGui::Button("set zero", { 100,30 }))
						{
							nn_renderer.selectedNeuron->SetZero();
						}
					}
				}
				else
				{
					ImGui::Text("Nothing selected");
				}
			}
			ImGui::End();

			if(brainToShow == 0)
				nn_renderer.Render(((Bot*)Simulation::INSTANCE()->selectedObject)->GetActiveBrain());
			else
				nn_renderer.Render(((Bot*)Simulation::INSTANCE()->selectedObject)->GetInitialBrain());
		}
	}
}

void ApplicationGUIManager::DrawWindows()
{
	//ImGUI demo window
	#ifdef ShowGUIDemoWindow
		DrawDemoWindow();
	#endif

	DrawMainWindow();
	DrawSystemWindow();
	DrawControlsWindow();
	DrawSelectionWindow();
	DrawRenderWindow();
	DrawConsoleWindow();
	DrawMouseFunctionWindow();
	DrawAdditionalsWindow();

	//Below windows that are hidden at startup

	DrawSaveLoadWindow();
	DrawDangerousWindow();
	DrawSummaryWindow();
	DrawAdaptationWindow();
	DrawChartWindow();
	DrawBotBrainWindow();
}

void ApplicationGUIManager::HighlightSelection()
{
	if (Simulation::INSTANCE()->selectionShadowScreen > 0)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, Simulation::INSTANCE()->selectionShadowScreen);
		SDL_RenderFillRect(renderer, &screenRect);
	}

	if (Simulation::INSTANCE()->selectedObject)
	{
		if (Simulation::INSTANCE()->cursorShow) {

			IObjectRenderer *  worker;

			switch (renderType) {

				case EnumRenderTypes::energy: worker = new EnergyObjectRenderer(); break;

				case EnumRenderTypes::natural: worker = new NaturalObjectRenderer(); break;

				case EnumRenderTypes::predators: worker = new PredatorsObjectRenderer(); break;

				case EnumRenderTypes::noRender:
				default: 
					return;
			}

			//FIXME: ObjectRenderer::drawOne(Simulation::INSTANCE()->selectedObject, worker);
		}
	}
}

void ApplicationGUIManager::MouseClick()
{
	if (!(nn_renderer.MouseClick({ mouseState.mouseX, mouseState.mouseY }) && (showBrain)))
	{
		if (Simulation::INSTANCE()->worldController->worldRenderer.IsInBoundsScreenCoords(mouseState.mouseX, mouseState.mouseY))
		{
			Point fieldCoords = Simulation::INSTANCE()->worldController->ScreenCoordsToLocal(mouseState.mouseX, mouseState.mouseY);

			if (fieldCoords.x < 0)
			{
				fieldCoords.x = 0;
			}
			else if (fieldCoords.x >= FieldCellsWidth)
			{
				fieldCoords.x = FieldCellsWidth - 1;
			}

			Object* obj = Simulation::INSTANCE()->worldController->gameWorld->GetObjectLocalCoords(fieldCoords.x, fieldCoords.y);

			if (mouseFunc == EnumMouseFunction::mouse_select)
			{
				if (obj)
				{
					if (obj->type == EnumObjectType::Bot)
					{
						Simulation::INSTANCE()->selectedObject = obj;
					}
				}
				else
				{
					Simulation::INSTANCE()->Deselect();
				}
			}
			else if (mouseFunc == EnumMouseFunction::mouse_remove)
			{
				Simulation::INSTANCE()->Deselect();

				for (int cx = -Simulation::INSTANCE()->brushSize; cx < Simulation::INSTANCE()->brushSize + 1; ++cx)
				{
					for (int cy = -Simulation::INSTANCE()->brushSize; cy < Simulation::INSTANCE()->brushSize + 1; ++cy)
					{
						if (Simulation::INSTANCE()->worldController->gameWorld->IsInBounds(fieldCoords.x + cx, fieldCoords.y + cy))
							Simulation::INSTANCE()->worldController->gameWorld->RemoveObject(fieldCoords.x + cx, fieldCoords.y + cy);
					}
				}
			}
			else if (mouseFunc == EnumMouseFunction::mouse_from_file)
			{
				//Cell is empty				
				if (!obj)
				{
					if (Simulation::INSTANCE()->selectedFile)
					{
						obj = Simulation::INSTANCE()->saver.LoadObject((char*)Simulation::INSTANCE()->selectedFile->nameFull.c_str());

						if (obj)
						{
							obj->x = fieldCoords.x;
							obj->y = fieldCoords.y;
							obj->energy = MaxPossibleEnergyForABot;

							if (Simulation::INSTANCE()->worldController->gameWorld->AddObject(obj))
							{
								Simulation::INSTANCE()->LogPrint("Object loaded\r\n");
							}
						}
						else
						{
							Simulation::INSTANCE()->LogPrint("Error while loading object\r\n");
						}
					}
				}				
			}
			else if (mouseFunc == EnumMouseFunction::mouse_force_mutation)
			{
				for (int cx = -Simulation::INSTANCE()->brushSize; cx < Simulation::INSTANCE()->brushSize + 1; ++cx)
				{
					for (int cy = -Simulation::INSTANCE()->brushSize; cy < Simulation::INSTANCE()->brushSize + 1; ++cy)
					{
						if (Simulation::INSTANCE()->worldController->gameWorld->IsInBounds(fieldCoords.x + cx, fieldCoords.y + cy))
						{
							Bot* obj = (Bot*)Simulation::INSTANCE()->worldController->gameWorld->GetObjectLocalCoords(fieldCoords.x + cx, fieldCoords.y + cy);

							if (obj)
							{
								if (obj->type == EnumObjectType::Bot)
								{
									obj->Mutagen();
								}
							}
						}
					}
				}
			}
		}
	}
}

void ApplicationGUIManager::Render()
{
	
	//Limit FPS
	TimePoint currentTickFps = SimulationTimer::getTime();

	if (renderType == EnumRenderTypes::noRender)
	{
		fpsInterval = 1000 / GUI_FPSWhenNoRender;
	}
	else
	{
		if (limitFPS > 0)
		{
			fpsInterval = 1000 / limitFPS;
		}
		else
		{
			fpsInterval = 0;
		}
	}

	if (TimeMSBetween(currentTickFps, lastTickFps) < fpsInterval)
	{
		return;
	}
	else
	{
		lastTickFps = currentTickFps;
	}

	//Begin frame

	//Clear background
	glClearColor(BackgroundColorFloat);
	glClear(GL_COLOR_BUFFER_BIT);

	//Render
	if (renderType != EnumRenderTypes::noRender) {
		Simulation::INSTANCE()->worldController->worldRenderer.draw(Simulation::INSTANCE()->worldController->gameWorld->params);


		IObjectRenderer* worker;

		switch (renderType) {

		case EnumRenderTypes::energy: worker = new EnergyObjectRenderer(); break;

		case EnumRenderTypes::natural: worker = new NaturalObjectRenderer(); break;

		case EnumRenderTypes::predators: worker = new PredatorsObjectRenderer(); break;

		case EnumRenderTypes::noRender:
		default:
			return;
		}



		ObjectRenderer::drawObjects(World::INSTANCE()->allCells, worker);

		//Highlight selected object
		HighlightSelection();

		++fpsCounter;
	}

	Simulation::INSTANCE()->SelectionShadowScreen();

	//Draw GUI
	GUIStartFrame();
	
	DrawWindows();

	ImGui::EndFrame();

	//Present scene
	SDLPresentScene();

	//Calculate fps
	if (TimeMSBetween(currentTickFps, lastSecondTickFps) >= 1000)
	{
		lastSecondTickFps = currentTickFps;

		realFPS = fpsCounter;
		fpsCounter = 0;
	}
}
