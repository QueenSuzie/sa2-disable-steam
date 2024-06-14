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

void SteamReplace::init() {
	SteamReplace::FindSteam();
	hLeaderboardDownload.Hook(LeaderboardDownload);
	SteamStats.Hook(SteamStatistics);
	LeaderboardLoad.Hook(LoadLeaderboard);
	LeaderboardsMenu.Hook(LeaderboardsMenuItem);
	hCheckNoInternetConnection.Hook(CheckNoInternetConnection);
	hShowNoInternet.Hook(ShowNoInternetDialog);
	hAchievementsMenu.Hook(AchievementsMenu);
}

void SteamReplace::FindSteam() {
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
	FARPROC runCallbacks = GetProcAddress(steam_api, "SteamAPI_RunCallbacks");
	FunctionHook<void> hRunCallbacks((intptr_t)runCallbacks);
	hRunCallbacks.Hook(RunCallbacks);
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

void SteamReplace::RunCallbacks() {}