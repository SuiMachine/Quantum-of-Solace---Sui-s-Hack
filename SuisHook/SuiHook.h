#pragma once
#include "HookFunctions.h"
#include "FPS_Hack.h"

class SuiHook
{
public:
	static SuiHook * getInstance();
	void Hook();
	bool Windowed;
	float DesiredFOV;
	int DesiredFPS;
	bool FPSHack;
	HMODULE jb_sp_s;
private:
	SuiHook();
	FPS_Hack * fpsHack;
	static SuiHook* instance;
};

