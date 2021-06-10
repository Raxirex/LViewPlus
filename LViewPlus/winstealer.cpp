#define BOOST_DEBUG_PYTHON 
#define USE_IMPORT_EXPORT
#define USE_WINDOWS_DLL_SEMANTICS
#define STB_IMAGE_IMPLEMENTATION

#include "PyStructs.h"

#include <iostream>
#include "windows.h"
#include "Utils.h"
#include "Structs.h"
#include "LeagueMemoryReader.h"
#include "Offsets.h"
#include "AntiCrack.h"
#include "MapObject.h"
#include "GameData.h"
#include "imgui.h"

#include <chrono>
#include "Overlay.h"
#include <map>
#include <list>
#include <conio.h>
#include <thread>
using namespace std::this_thread;

using namespace std::chrono;

void MainLoop(Overlay& overlay, LeagueMemoryReader& reader);

std::wstring getComputerName() {
	wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
	DWORD cchBufferSize = sizeof(buffer) / sizeof(buffer[0]);
	if (!GetComputerNameW(buffer, &cchBufferSize))
		throw std::runtime_error("GetComputerName() failed.");
	return std::wstring(&buffer[0]);
}

int main()
{
	printf(
		"	Thanks for using my LView fork if you have a question you can ask on the unknowncheats \n"
		"	Discord soon...\n"
		"	############################################################ \n"
		"	############################################################ \n"
		"	############################################################ \n"
	);
	// TODO license system example
	/*
	const std::wstring compName = getComputerName();
	if (compName != L"Your computer name")
		printf("Your computer not verifed!");
		return 0;
	*/

	Overlay overlay = Overlay();
	LeagueMemoryReader reader = LeagueMemoryReader();

	try {
		PyImport_AppendInittab("winstealer", &PyInit_winstealer);
		Py_Initialize();
		overlay.Init();
		MapObject::Get(MapType::SUMMONERS_RIFT)->Load("data/height_map_sru.bin");
		MapObject::Get(MapType::HOWLING_ABYSS)->Load("data/height_map_ha.bin");
		std::string dataPath("data");
		GameData::Load(dataPath);

		MainLoop(overlay, reader);

		Py_Finalize();

	}
	catch (std::runtime_error exception) {
		std::cout << exception.what() << std::endl;
	}

	printf("Press any key to exit...");
	getch();
}

void MainLoop(Overlay& overlay, LeagueMemoryReader& reader) {

	MemSnapshot memSnapshot;
	bool rehook = true;
	bool firstIter = true;

	printf(
		"	Script loaded! \n"
		"	Waiting the win game...\n"
	);
	while (true) {

		bool isLeagueWindowActive = reader.IsLeagueWindowActive();
		if (overlay.IsVisible()) {
			if (Input::WasKeyPressed(HKey::F8)) {
				overlay.ToggleTransparent();
			}
			if (!isLeagueWindowActive) {
				overlay.Hide();
			}
		}
		else if (isLeagueWindowActive) {
			overlay.Show();
		}
		try {
			overlay.StartFrame();
			if (rehook) {
				reader.HookToProcess();
				rehook = false;
				firstIter = true;
				memSnapshot = MemSnapshot();
				printf("	Match to be won found.\n");
			}
			else {

				if (!reader.IsHookedToProcess()) {
					rehook = true;
					printf("	Game stopped?\n");
					printf("	Waiting the win game...\n");
				}

				reader.MakeSnapshot(memSnapshot);
				if (memSnapshot.gameTime > 2.f) {
					if (firstIter) {

						overlay.GameStart(memSnapshot);
						firstIter = false;
					}
					overlay.Update(memSnapshot);
				}
			}
			overlay.RenderFrame();
		}
		catch (WinApiException exception) {
			rehook = true;
		}
		catch (std::runtime_error exception) {
			printf("	Error? %s \n", exception.what());
			break;
		}
	}
}