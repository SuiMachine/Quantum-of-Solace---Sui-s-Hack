#include "FPS_Hack.h"
#include "SuiHook.h"

unsigned int _DesiredFPSF = 30;
const float originalDeltaTime = 0.0333333333333334f;
const float one = 1;
float temp = 0;
float lastDeltaTimeCopy;
intptr_t returnAddressNormalMovementF;
intptr_t bytePtr_12246105F;
intptr_t fpsAddressF;

intptr_t returnCodeDetourStoreLastDeltaTime;
//Imprecise... but far better than nothing
__declspec(naked) void CodeDetourStoreLastDeltaTime()
{
	_asm
	{
		movss [edi+0x4],xmm0
		fld [edi+0x4]
		fdiv [originalDeltaTime]
		fstp [lastDeltaTimeCopy]
		jmp returnCodeDetourStoreLastDeltaTime
	}
}

intptr_t returnBalancingGameMultiplierGoingForwardDetour;
__declspec(naked) void BalancingGameMultiplierGoingForwardDetour()
{
	_asm
	{
		//Assembly lerp with clamp t under 1 - would be macro, but VS doesn't want to compile parameters :(
		//According to Godot - result = A + (B-A) * t;
		fld [one] //Load A (1)
		fld [eax + 0x10] //Load B
		fsub [one] // Do B-A
		fmul [lastDeltaTimeCopy] //Do (B-A) * t
		fadd st(0), st(1)
		fstp [temp] //store result
		fstp st(0) //remove last element from float stack
		movss xmm0, [temp]
		jmp returnBalancingGameMultiplierGoingForwardDetour
	}
}

intptr_t returnBalancingGameMultiplierTrevesalDetour;
__declspec(naked) void BalancingGameMultiplierTrevesalDetour()
{
	_asm
	{
		//Assembly lerp with clamp t under 1 - would be macro, but VS doesn't want to compile parameters :(
		fld[one] //Load A (1)
		fld[ecx + 0x10] //Load B
		fsub[one] // Do B-A
		fmul[lastDeltaTimeCopy] //Do (B-A) * t
		fadd st(0), st(1)
		fstp[temp] //store result
		fstp st(0) //remove last element from float stack
		movss xmm0, [temp]
		jmp returnBalancingGameMultiplierTrevesalDetour
	}
}

__declspec(naked) void CodeDetourNormalMovementF()
{
	__asm
	{
		mov ecx, [_DesiredFPSF]
		mov eax, [fpsAddressF]
		mov[eax], ecx
		mov eax, [bytePtr_12246105F]
		cmp byte ptr[eax], 0
		jz end
		mov byte ptr[eax], 1
		end:
		jmp returnAddressNormalMovementF
	}
}

// valTemp = scale * val;
// valTemp2 = scale * deltaTIme * Val;
// val = valTemp2 - ValTemp;

FPS_Hack::FPS_Hack()
{
	_DesiredFPSF = SuiHook::getInstance()->DesiredFPS;
	bytePtr_12246105F = (intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x2246105;
	fpsAddressF = (intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x1A36E50;

	HookInsideFunction((intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x4D087A, &CodeDetourStoreLastDeltaTime, &returnCodeDetourStoreLastDeltaTime, 0x5);
	HookInsideFunction((intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x31B383, &BalancingGameMultiplierGoingForwardDetour, &returnBalancingGameMultiplierGoingForwardDetour, 0x5);
	HookInsideFunction((intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x31B3B6, &BalancingGameMultiplierTrevesalDetour, &returnBalancingGameMultiplierTrevesalDetour, 0x5);

	HookInsideFunction((intptr_t)(SuiHook::getInstance()->jb_sp_s) + 0x31A589, &CodeDetourNormalMovementF, &returnAddressNormalMovementF, 0x10);
}
