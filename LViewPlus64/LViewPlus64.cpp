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

/* bool Authenticate(); */

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
		"	:::    :::     ::: ::::::::::: :::::::::: :::       ::: \n"
		"	:+:    :+:     :+:     :+:     :+:        :+:       :+: \n"
		"	+:+    +:+     +:+     +:+     +:+        +:+       +:+ \n"
		"	+#+    +#+     +:+     +#+     +#++:++#   +#+  +:+  +#+ \n"
		"	+#+     +#+   +#+      +#+     +#+        +#+ +#+#+ +#+ \n"
		"	#+#      #+#+#+#       #+#     #+#         #+#+# #+#+#  \n"
		"	########## ###     ########### ##########   ###   ###   \n\n"
	);

	Overlay overlay = Overlay();
	LeagueMemoryReader reader = LeagueMemoryReader();

	try {
		printf("[+] Initializing PyModule\n");
		PyImport_AppendInittab("LViewPlus64", &PyInit_LViewPlus64);
		Py_Initialize();

		printf("[+] Initialising imgui and directx UI\n");
		overlay.Init();

		printf("[+] Loading unit data\n");
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
		"[i] Waiting for league process...\n"
	);
	while (true) {

		bool isLeagueWindowActive = reader.IsLeagueWindowActive();
		if (overlay.IsVisible()) {
			// One some systems the ingame cursor is replaced with the default Windows cursor
			// With the WS_EX_TRANSPARENT window flag enabled the cursor is as expected but the user cannot control the overlay
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

			// Try to find the league process and get its information necessary for reading
			if (rehook) {
				reader.HookToProcess();
				rehook = false;
				firstIter = true;
				memSnapshot = MemSnapshot();
				printf("[i] Found league process. The UI will appear when the game stars.\n");
			}
			else {

				if (!reader.IsHookedToProcess()) {
					rehook = true;
					printf("[i] League process is dead.\n");
					printf("[i] Waiting for league process...\n");
				}

				reader.MakeSnapshot(memSnapshot);

				// If the game started
				if (memSnapshot.gameTime > 2.f) {
					// Tell the UI that a new game has started
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
			// This should trigger only when we don't find the league process.
			rehook = true;
		}
		catch (std::runtime_error exception) {
			printf("[!] Unexpected error occured: \n [!] %s \n", exception.what());
			break;
		}
	}
}