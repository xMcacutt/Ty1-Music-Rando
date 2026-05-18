#pragma once
#include "core.h"
#include "TygerFrameworkAPI.hpp"

extern "C" {
	__declspec(dllexport) void __cdecl GenerateShuffleWithSeed(unsigned int seed);
}

class Hooks
{
public:
	static void InitHooks();
private:
	static inline std::unordered_map<int, int> level_music_mapping = {
		{0, 1},
		{1, 2},
		{2, 3},
		{3, 4},
		{4, 5},
		{5, 6},
		{6, 7}
	};
	static void __stdcall LoadLevelMusicHook();
	static uintptr_t OnLoadLevelMusic(uintptr_t eax);
};

