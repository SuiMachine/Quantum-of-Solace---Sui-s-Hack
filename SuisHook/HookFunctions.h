#pragma once
#pragma once
#include <Windows.h>
#include <string>

static void UnprotectModule(HMODULE p_Module)
{
	//This function was provided by Orfeasz
	PIMAGE_DOS_HEADER s_Header = (PIMAGE_DOS_HEADER)p_Module;
	PIMAGE_NT_HEADERS s_NTHeader = (PIMAGE_NT_HEADERS)((DWORD)p_Module + s_Header->e_lfanew);

	SIZE_T s_ImageSize = s_NTHeader->OptionalHeader.SizeOfImage;

	DWORD s_OldProtect;
	VirtualProtect((LPVOID)p_Module, s_ImageSize, PAGE_EXECUTE_READWRITE, &s_OldProtect);
}

static int StrEndsWith(WCHAR* chrArray, int lenght, char character)
{
	int pos = -1;
	for (int i = 0; i < lenght; i++)
	{
		if (chrArray[i] == character)
			pos = i;
	}

	return pos;
}

static void StrToLower(WCHAR* chrArray)
{
	int lenght = std::wcslen(chrArray);
	for (int i = 0; i < lenght; i++)
	{
		chrArray[i] = ::tolower(chrArray[i]);
	}
}