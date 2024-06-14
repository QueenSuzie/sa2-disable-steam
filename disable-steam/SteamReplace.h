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

class SteamReplace
{
	public:
		static void init();
		static void LeaderboardDownload();
		static void SteamStatistics(int, int);
		static void LoadLeaderboard(int);
		static signed int LeaderboardsMenuItem(int);
		static void AchievementsMenu();
		static bool CheckNoInternetConnection();
		static void ShowNoInternetDialog();
		static void RunCallbacks();

	private:
		static void FindSteam();
		static void SteamInit(HMODULE);
};