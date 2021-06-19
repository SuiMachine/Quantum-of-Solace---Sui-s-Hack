// dllmain.cpp : Defines the entry point for the DLL application.
#include "HookFunctions.h"
#include <regex>
#include "SuiHook.h"

SuiHook * hook;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        auto baseModule = GetModuleHandle(NULL);
        WCHAR baseModuleName[MAX_PATH];
        GetModuleFileName(baseModule, baseModuleName, sizeof(baseModuleName));
        int indexOfLastPathNode = StrEndsWith(baseModuleName, sizeof(baseModuleName), '\\') + 1;
        auto lenght = std::wcslen(baseModuleName);
        StrToLower(baseModuleName);

        std::wregex pattern(L"jb_liveengine_s\\.exe");
        if (std::regex_search(baseModuleName, pattern))
        {
#if _DEBUG
            MessageBox(NULL, L"Stop", L"Stop", 0);
#endif
            hook = SuiHook::getInstance();
            hook->Hook();
        }
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

