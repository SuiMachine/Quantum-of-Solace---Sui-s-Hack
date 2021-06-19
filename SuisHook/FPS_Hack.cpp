#include "FPS_Hack.h"
#include "SuiHook.h"

unsigned int _DesiredFPS = 30;
intptr_t returnAddressNormalMovement;
intptr_t returnAddressAnimatedMovement;
intptr_t bytePtr_12246105;
intptr_t bytePtr_1063C489;
intptr_t fpsAddress;


__declspec(naked) void CodeDetourAnimatedMovement()
{
	__asm
	{
		mov eax,[bytePtr_1063C489]
		mov [eax],1
		mov eax, [fpsAddress]
		mov [eax], 30
		mov eax, 0x1031A630
		call eax
		jmp returnAddressAnimatedMovement
	}
}

__declspec(naked) void CodeDetourNormalMovement()
{
	__asm
	{
		mov ecx,[_DesiredFPS]
		mov eax,[fpsAddress]
		mov [eax], ecx
		mov eax,[bytePtr_12246105]
		cmp byte ptr[eax], 0
		jz end
		mov byte ptr[eax], 1
		end:
		jmp returnAddressNormalMovement
	}
}

FPS_Hack::FPS_Hack()
{
	_DesiredFPS = SuiHook::getInstance()->DesiredFPS;
	bytePtr_12246105 = (intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x2246105;
	bytePtr_1063C489 = (intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x63C489;
	fpsAddress = 0x11A36E50;

	HookInsideFunction((intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x31A519, &CodeDetourAnimatedMovement, &returnAddressAnimatedMovement, 0xC);
	HookInsideFunction((intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x31A589, &CodeDetourNormalMovement, &returnAddressNormalMovement, 0x10);

}
