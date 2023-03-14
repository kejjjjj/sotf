#pragma once

#ifndef __entry1
#define __entry1

#include "pch.h"

#define _ERR_EXISTS

namespace __err
{
	inline bool fatal_error = false;
	inline static void error(const std::string& t) { MessageBox(NULL, t.c_str(), "Fatal Error!", MB_ICONERROR); fatal_error = true; }
}
namespace __main
{
	VOID __stdcall DLL_Entry(HMODULE mod);

}

#endif