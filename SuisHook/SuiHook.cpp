#include "SuiHook.h"
#include "IniReader/IniReader.h"

SuiHook* SuiHook::instance = NULL;

SuiHook::SuiHook()
{
	CIniReader iniReader("");
	Windowed = iniReader.ReadBoolean("MAIN", "Windowed", true);
	DesiredFOV = iniReader.ReadFloat("MAIN", "FOV", 65.0f);
	DesiredFPS = iniReader.ReadInteger("MAIN", "FPS", 60);


	SuiHook::jb_sp_s = GetModuleHandle(L"jb_sp_s.dll");
}

SuiHook* SuiHook::getInstance()
{
	if (!instance)
	{
		instance = new SuiHook();
		return instance;
	}
	else
		return instance;
}

void SuiHook::Hook()
{
	if (SuiHook::jb_sp_s != NULL)
	{
		UnprotectModule(SuiHook::jb_sp_s);

		if(this->Windowed)
			memset((void*)((intptr_t)SuiHook::jb_sp_s + 0x104ACD), 0x90, 0x2);

		if (DesiredFPS != 0)
		{
			fpsHack = new FPS_Hack();
		}
	}
	else
		MessageBox(NULL, L"No game module was found!", L"Error", MB_ICONERROR | MB_OK);
}
