#include "pch.h"


VOID __stdcall __main::DLL_Entry(HMODULE mod)
{
	FILE* _con = 0;

	AllocConsole();
	freopen_s(&_con, "CONOUT$", "w", stdout);

	g::G_Init();

	g::G_ReleaseHooks();

	MH_STATUS state = MH_STATUS::MH_OK;
	if (state = MH_Uninitialize(), state != MH_STATUS::MH_OK) {
		__err::error(std::string("MH_Uninitialize(): ") + MH_StatusToString(state));
	}

	if (_con)
		fclose(_con);
	FreeConsole();

	FreeLibraryAndExitThread(mod, 0);
}