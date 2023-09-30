#include "MyKeyBoard.h"

#include "../MyTypes.h"
#include "../Settings.h"
#include "../Simulation.h"


void MyKeyBoard::CatchKeyboard(const Uint8* keyboard) {

	if (keyboard[Keyboard_Pause] || keyboard[Keyboard_Pause2]) {
		Simulation::INSTANCE()->Pause();
	}
	else if (keyboard[Keyboard_SpawnRandoms]) {
		Simulation::INSTANCE()->worldController->SpawnControlGroup();
	}
	else if (keyboard[Keyboard_PlaceWall])
	{
		repeat(FieldCellsHeight)
			Simulation::INSTANCE()->worldController->gameWorld->AddObject(new Rock(0, i));
	}
	else if (keyboard[Keyboard_DropOrganics])
	{
		for (int X = 0; X < FieldCellsWidth; ++X)
		{
			for (int Y = 0; Y < 25 + RandomVal(20); ++Y)
			{
				Simulation::INSTANCE()->worldController->gameWorld->AddObject(new Organics(X, Y, MaxPossibleEnergyForABot / 2));
			}
		}
	}
	else if (keyboard[Keyboard_NextFrame])
	{
		if (!Simulation::INSTANCE()->simulate) {
			Simulation::INSTANCE()->MakeStep();
		}
	}
	else if (keyboard[Keyboard_RenderNatural])
	{
		Simulation::INSTANCE()->gui.renderType = EnumRenderTypes::natural;
	}
	else if (keyboard[Keyboard_RenderPredators])
	{
		Simulation::INSTANCE()->gui.renderType = EnumRenderTypes::predators;
	}
	else if (keyboard[Keyboard_RenderEnergy])
	{
		Simulation::INSTANCE()->gui.renderType = EnumRenderTypes::energy;
	}
	else if (keyboard[Keyboard_NoRender])
	{
		Simulation::INSTANCE()->gui.renderType = EnumRenderTypes::noRender;
	}
	else if (keyboard[SDL_SCANCODE_RIGHT])
	{
		Simulation::INSTANCE()->worldController->worldRenderer.shiftRenderPoint((io->KeyShift) ? MoveCameraFastSpeed : MoveCameraSpeed);
	}
	else if (keyboard[SDL_SCANCODE_LEFT])
	{
		Simulation::INSTANCE()->worldController->worldRenderer.shiftRenderPoint((io->KeyShift) ? -MoveCameraFastSpeed : -MoveCameraSpeed);
	}
	else if (keyboard[Keyboard_Reset_RenderX])
	{
		Simulation::INSTANCE()->worldController->worldRenderer.renderX = 0;
	}
	else if (keyboard[Keyboard_Jump_Up_RenderX])
	{
		Simulation::INSTANCE()->worldController->worldRenderer.shiftRenderPoint(MoveCameraJump);
	}
	else if (keyboard[Keyboard_Jump_Down_RenderX])
	{
		Simulation::INSTANCE()->worldController->worldRenderer.shiftRenderPoint(-MoveCameraJump);
	}
	else if (keyboard[Keyboard_Jump_To_First_bot])
	{
		Simulation::INSTANCE()->worldController->jumpToFirstBot();
	}
	//Additional windows hotkeys
	else if (keyboard[Keyboard_ShowSaveLoad_Window])
	{
		Simulation::INSTANCE()->LoadFilenames();

		Simulation::INSTANCE()->gui.showSaveLoad = !Simulation::INSTANCE()->gui.showSaveLoad;
	}
	else if (keyboard[Keyboard_ShowDangerous_Window])
	{
		Simulation::INSTANCE()->gui.showDangerous = !Simulation::INSTANCE()->gui.showDangerous;
	}
	else if (keyboard[Keyboard_ShowAdaptation_Window])
	{
		Simulation::INSTANCE()->gui.showAdaptation = !Simulation::INSTANCE()->gui.showAdaptation;
	}
	else if (keyboard[Keyboard_ShowChart_Window])
	{
		Simulation::INSTANCE()->gui.showChart = !Simulation::INSTANCE()->gui.showChart;
	}
	else if (keyboard[Keyboard_ShowBrain_Window])
	{
		Simulation::INSTANCE()->gui.showBrain = !Simulation::INSTANCE()->gui.showBrain;
	}
}
