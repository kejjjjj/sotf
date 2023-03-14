#include "pch.h"

void g::G_Init()
{
	MH_STATUS state = MH_STATUS::MH_OK;

	if (state = MH_Initialize(), state != MH_STATUS::MH_OK) {
		__err::error(MH_StatusToString(state));
	}

	GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
	UnityPlayer = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");

	if (!GameAssembly || !UnityPlayer) {
		__err::error("!GameAssembly || !UnityPlayer");
		return;
	}

	if (!r::R_Init())
		return;

	G_CreateHooks();

	std::cout << "success!\n";


	while (!__err::fatal_error && (GetAsyncKeyState(VK_HOME)&1)==false) {
		Sleep(500);
	}
}
const auto _hook = (void(*)(float, float))(g::GameAssembly + 47343840); //Sons.Environment.TimeOfDayHolder$$JumpTime
static hookobj<void(float, float)> obj;
void Sons_Environment_TimeOfDayHolder__JumpTime(float seconds, float overtime)
{
	return obj.orgFnc(seconds, overtime);
}
void g::G_CreateHooks()
{
	if (g::bHooked)
		return;

	//const auto _hook = (void(*)(float, float))(GameAssembly + 47343840); //Sons.Environment.TimeOfDayHolder$$JumpTime
	//obj.init(_hook, (PVOID)(GameAssembly + 47343840), Sons_Environment_TimeOfDayHolder__JumpTime, true);

	r::endscene_f.init(r::pEndScene, r::pEndScene, r::D3D_Draw, true);

	//hookobj<long(*)(IDXGISwapChain*, UINT, UINT)> WTF;

	//std::cout << "calling: " << &r::endscene_f.orgFnc << '\n';
	g::bHooked = true;

}
void g::G_ReleaseHooks()
{
	if (!r::endscene_f.release()) {
		std::cout << "r::endscene_f.release() = failed\n";
	}else
		std::cout << "r::endscene_f.release() = success\n";

}