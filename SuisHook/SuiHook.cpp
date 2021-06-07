#include "SuiHook.h"

SuiHook::SuiHook()
{
	auto gameModule = GetModuleHandle(L"jb_sp_s.dll");
	if (gameModule != NULL)
	{
		UnprotectModule(gameModule);
		memset((void*)((intptr_t)gameModule + 0x104ACD), 0x90, 0x2);
		*(float*)0x105BD3B0 = 65.0f;
	}
}
