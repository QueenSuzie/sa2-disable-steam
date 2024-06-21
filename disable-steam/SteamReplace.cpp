/**************************************************************************************
 *   SteamReplace.cpp  --  This file is part of SA2 Disable Steam.                    *
 *                                                                                    *
 *   Copyright (C) 2024 Queen Suzie                                                   *
 *                                                                                    *
 *   SA2 Disable Steam is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU General Public License as published                *
 *   by the Free Software Foundation, either version 3 of the License,                *
 *   or (at your option) any later version.                                           *
 *                                                                                    *
 *   SA2 Disable Steam is distributed in the hope that it will be useful,             *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty                      *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                          *
 *   See the GNU General Public License for more details.                             *
 *                                                                                    *
 *   You should have received a copy of the GNU General Public License                *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.              *
 *                                                                                    *
 *************************************************************************************/

#include "pch.h"
#include "SteamReplace.h"

FunctionHook<void> hLeaderboardDownload((intptr_t)0x4120C0);
FunctionHook<bool> hCheckNoInternetConnection((intptr_t)0x411AE0);
FunctionHook<void> hShowNoInternet((intptr_t)0x411B30);
FunctionHook<void> hAchievementsMenu((intptr_t)0x40D300);
StdcallFunctionHook<void, void*> hSteamCallBacks((intptr_t)0x40A8F0);

void SteamReplace::init() {
	SteamReplace::FindSteamModule();
	SteamReplace::DisableSteamOnlineChecks();
	hSteamCallBacks.Hook(SteamCallBacks);
	hLeaderboardDownload.Hook(LeaderboardDownload);
	SteamStats.Hook(SteamStatistics);
	LeaderboardLoad.Hook(LoadLeaderboard);
	LeaderboardsMenu.Hook(LeaderboardsMenuItem);
	hCheckNoInternetConnection.Hook(CheckNoInternetConnection);
	hShowNoInternet.Hook(ShowNoInternetDialog);
	hAchievementsMenu.Hook(AchievementsMenu);

	WriteJump((void*)0x664FCB, &SteamReplace::DisableSteamMenuEntriesUp);
	WriteData<1>((void*)0x664FD0, 0x90u);
	WriteJump((void*)0x665007, &SteamReplace::DisableSteamMenuEntriesDown);
	WriteData<1>((void*)0x66500C, 0x90u);

	WriteJump((void*)0x669646, &SteamReplace::SkipSteamMainMenuEntries);
	WriteData<1>((void*)0x66964B, 0x90u);
	WriteData<1>((void*)0x66964C, 0x90u);
	WriteData<1>((void*)0x66964D, 0x90u);
	WriteData<1>((void*)0x66964E, 0x90u);

	WriteJump((void*)0x6696B3, &SteamReplace::ModifyMainMenuEntriesPositions);
	WriteData<1>((void*)0x6696B8, 0x90u);
	WriteData<1>((void*)0x6696B9, 0x90u);
	WriteData<1>((void*)0x6696BA, 0x90u);
	WriteData<1>((void*)0x6696BB, 0x90u);
	WriteData<1>((void*)0x6696BC, 0x90u);
}

void SteamReplace::DisableSteamOnlineChecks() {
	// Disable SteamServersConnected
	WriteData<1>((void*)0x40A4E8, 0x90u);
	WriteData<1>((void*)0x40A4E9, 0x90u);
	WriteData<1>((void*)0x40A4EA, 0x90u);
	WriteData<1>((void*)0x40A4EB, 0x90u);
	WriteData<1>((void*)0x40A4EC, 0x90u);

	// Disable SteamServersDisconnected
	WriteData<1>((void*)0x40A50F, 0x90u);
	WriteData<1>((void*)0x40A510, 0x90u);
	WriteData<1>((void*)0x40A511, 0x90u);
	WriteData<1>((void*)0x40A512, 0x90u);
	WriteData<1>((void*)0x40A513, 0x90u);

	// Disable SteamServerConnectFailure
	WriteData<1>((void*)0x40A536, 0x90u);
	WriteData<1>((void*)0x40A537, 0x90u);
	WriteData<1>((void*)0x40A538, 0x90u);
	WriteData<1>((void*)0x40A539, 0x90u);
	WriteData<1>((void*)0x40A53A, 0x90u);
}

void SteamReplace::FindSteamModule() {
	DWORD cbNeeded;
	HMODULE hMods[1024];
	HANDLE currentProcess = GetCurrentProcess();
	TCHAR steamApi[] = { 's', 't', 'e', 'a', 'm', '_', 'a', 'p', 'i', NULL };

	if (currentProcess && EnumProcessModules(currentProcess, hMods, sizeof(hMods), &cbNeeded)) {
		unsigned short hModsSize = cbNeeded / sizeof(HMODULE);
		for (unsigned short i = 0; i < hModsSize; i++) {
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(currentProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
				TString modName(szModName);
				if (modName.find(steamApi) != std::string::npos) {
					return SteamReplace::SteamInit(hMods[i]);
				}
			}
		}
	}
}

void SteamReplace::SteamInit(HMODULE steam_api) {
	SteamReplace::SteamAPI_UnregisterCallback = (void(__cdecl*)(void* callbacks))GetProcAddress(steam_api, "SteamAPI_UnregisterCallback");
}

void __stdcall SteamReplace::SteamCallBacks(void* steam_callbacks) {
	for (unsigned short i = 0; i < SteamReplace::SteamCallbackOffsetsSize; i++) {
		if (*(int*)(((intptr_t)steam_callbacks) + SteamReplace::SteamCallbackOffsets[i])) {
			SteamReplace::SteamAPI_UnregisterCallback((void*)(((intptr_t)steam_callbacks) + SteamReplace::SteamCallbackOffsets[i]));
			*(int*)(((intptr_t)steam_callbacks) + SteamReplace::SteamCallbackOffsets[i]) = NULL;
		}
	}

	hSteamCallBacks.Original(steam_callbacks);
}

void SteamReplace::LeaderboardDownload() {
	dword_1AF19FC = 0;
}

void SteamReplace::SteamStatistics(int a1, int a2) {}

void SteamReplace::LoadLeaderboard(int a1) {}

signed int SteamReplace::LeaderboardsMenuItem(int a1) {
	return 1;
}

void SteamReplace::AchievementsMenu() {}

bool SteamReplace::CheckNoInternetConnection() {
	return true;
}

void SteamReplace::ShowNoInternetDialog() {}

void SteamReplace::RunCallbacks() {

}

__declspec(naked) void SteamReplace::DisableSteamMenuEntriesUp() {
	__asm {
		cmp		byte ptr[eax + 0x24], 2
		je		DISABLE_MENU
		cmp		byte ptr[eax + 0x24], 3
		je		DISABLE_MENU_DOUBLE
		cmp		byte ptr[eax + 0x24], 5
		je		DISABLE_MENU
		jmp		SteamReplace::JumpNormalMenuItemUp

DISABLE_MENU_DOUBLE:
		dec		byte ptr[eax + 0x24]

DISABLE_MENU:
		jmp		SteamReplace::JumpDisableMenuItemUp
	}
}

__declspec(naked) void SteamReplace::DisableSteamMenuEntriesDown() {
	__asm {
		cmp		byte ptr[eax + 0x24], 2
		je		DISABLE_MENU_DOUBLE
		cmp		byte ptr[eax + 0x24], 3
		je		DISABLE_MENU
		cmp		byte ptr[eax + 0x24], 5
		je		DISABLE_MENU
		jmp		SteamReplace::JumpNormalMenuItemDown

DISABLE_MENU_DOUBLE :
		inc		byte ptr[eax + 0x24]

DISABLE_MENU :
		jmp		SteamReplace::JumpDisableMenuItemDown
	}
}

__declspec(naked) void SteamReplace::SkipSteamMainMenuEntries() {
	__asm {
		cmp		esi, 2
		je		DISABLE_MENU
		cmp		esi, 3
		je		DISABLE_MENU
		cmp		esi, 5
		je		DISABLE_MENU
		jmp		SteamReplace::JumpBackToMenuItem

DISABLE_MENU:
		inc     esi
		jmp		SteamReplace::JumpToSkipMenuItem
	}
}

__declspec(naked) void SteamReplace::ModifyMainMenuEntriesPositions() {
	__asm {
		mov		ecx, SteamReplace::MenuItemPositionConstant
		mov		eax, esi
		cmp		esi, 2
		jl		INC_POS
		cmp		esi, 5
		jg		DEC_POS
		dec		eax

MAIN:
		lea		eax, [eax + eax * 2]
		lea		eax, [eax * 4 + ecx]
		jmp		SteamReplace::JumpToPositionCalcReturn

INC_POS:
		inc		eax
		jmp		MAIN

DEC_POS:
		sub		eax, 2
		jmp		MAIN
	}
}