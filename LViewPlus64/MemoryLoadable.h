#pragma once
#include "windows.h"

/// Interface to be implemented by game objects that are read from memory
class MemoryLoadable {

	virtual void LoadFromMem(DWORD64 base, HANDLE hProcess, bool deepLoad = true) = 0;
};