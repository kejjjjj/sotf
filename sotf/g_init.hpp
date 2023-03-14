#pragma once

#ifndef g_init_
#define g_init_

#include "pch.h"

namespace g
{
	void G_Init();
	void G_CreateHooks();
	void G_ReleaseHooks();
	inline bool bHooked = false;

	inline uintptr_t GameAssembly, UnityPlayer;
	inline HWND gameWindow;

}

#endif