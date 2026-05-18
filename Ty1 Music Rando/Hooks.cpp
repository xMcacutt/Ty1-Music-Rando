#include "pch.h"
#include "Hooks.h"
#include "minhook.h"
#include <random>

typedef void* (__cdecl* PlayTrackType)(const char* trackName);
PlayTrackType originalPlayTrack = nullptr;

std::unordered_map<std::string, std::string> music_map;
const std::vector<std::string> music_list = {
	"music_a1", "music_a1_ta", "music_a2", "music_a2_mg", "music_a2_ta",
	"music_a3", "music_a4", "music_a4_boss", "music_b1", "music_b1_mb",
	"music_b1_mg", "music_b1_ta", "music_b2", "music_b2_ta", "music_b3",
	"music_b3_mg", "music_b3_ta", "music_c1", "music_c1_mb", "music_c1_ta",
	"music_c2", "music_c2_mg", "music_c2_ta", "music_c3", "music_c3_mg",
	"music_c3_ta", "music_c4", "music_c4_boss", "music_d1", "music_d2",
	"music_d4", "music_d4_boss", "music_e1", "music_e3", "music_e4",
	"music_e4_boss", "music_frontend", "music_z1", "music_z2"
};


void* __cdecl OnPlayTrack(const char* trackName) {
	if (trackName != nullptr) {
		std::string requestedTrack(trackName);
		auto it = music_map.find(requestedTrack);
		if (it != music_map.end()) {
			return originalPlayTrack(it->second.c_str());
		}
	}
	return originalPlayTrack(trackName);
}

void GenerateInitialRandomShuffle() {
	std::vector<std::string> randomized_list = music_list;
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(randomized_list.begin(), randomized_list.end(), g);
	for (size_t i = 0; i < music_list.size(); ++i) {
		music_map[music_list[i]] = randomized_list[i];
	}
}

extern "C" {
	__declspec(dllexport) void __cdecl GenerateShuffleWithSeed(unsigned int seed) {
		music_map.clear();
		std::vector<std::string> randomized_list = music_list;
		std::mt19937 g(seed);
		std::shuffle(randomized_list.begin(), randomized_list.end(), g);
		for (size_t i = 0; i < music_list.size(); ++i) {
			music_map[music_list[i]] = randomized_list[i];
		}
	}
}

void Hooks::InitHooks() {
	MH_Initialize();
	GenerateInitialRandomShuffle();
	LPVOID targetAddress = reinterpret_cast<LPVOID>(Core::moduleBase + 0x1A8F30);	
	MH_CreateHook(targetAddress, &OnPlayTrack, reinterpret_cast<LPVOID*>(&originalPlayTrack));
	MH_EnableHook(targetAddress);
}