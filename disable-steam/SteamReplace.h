/**************************************************************************************
 *   SteamReplace.h  --  This file is part of SA2 Disable Steam.                      *
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

#pragma once

#include <psapi.h>
#include <string>

#ifndef UNICODE  
typedef std::string TString;
#else
typedef std::wstring TString;
#endif

DataPointer(DWORD, dword_1AF19FC, 0x1AF19FC);

UsercallFuncVoid(SteamStats, (int a1, int a2), (a1, a2), (intptr_t)0x40E880, rEAX, rECX);
UsercallFuncVoid(LeaderboardLoad, (int a1), (a1), (intptr_t)0x4108B0, rESI);
UsercallFunc(signed int, LeaderboardsMenu, (int a1), (a1), (intptr_t)0x40F520, rEAX, rEBX);

class SteamReplace {
	public:
		static void init();
		static void __stdcall SteamCallBacks(void*);
		static void LeaderboardDownload();
		static void SteamStatistics(int, int);
		static void LoadLeaderboard(int);
		static signed int LeaderboardsMenuItem(int);
		static void AchievementsMenu();
		static bool CheckNoInternetConnection();
		static void ShowNoInternetDialog();
		static void RunCallbacks();
		static void DisableSteamMenuEntriesUp();
		static void DisableSteamMenuEntriesDown();
		static void SkipSteamMainMenuEntries();
		static void ModifyMainMenuEntriesPositions();
		static inline void* JumpNormalMenuItemUp = (void*)0x664FD9;
		static inline void* JumpDisableMenuItemUp = (void*)0x664FD1;
		static inline void* JumpNormalMenuItemDown = (void*)0x665015;
		static inline void* JumpDisableMenuItemDown = (void*)0x66500D;
		static inline void* JumpBackToMenuItem = (void*)0x66968E;
		static inline void* JumpToSkipMenuItem = (void*)0x669604;
		static inline void* JumpToPositionCalcReturn = (void*)0x6696BD;
		static inline void* MenuItemPositionConstant = (void*)0x1A3D678;

	private:
		static inline void(__cdecl* SteamAPI_UnregisterCallback)(void* callbacks) = nullptr;
		static inline unsigned short SteamCallbackOffsets[] = { 0, 20, 40 };
		static inline unsigned short SteamCallbackOffsetsSize = sizeof(SteamReplace::SteamCallbackOffsets) / sizeof(unsigned short);
		static void FindSteamModule();
		static void SteamInit(HMODULE);
		static void DisableSteamOnlineChecks();
};