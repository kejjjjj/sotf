#pragma once

#ifndef rinit_
#define rinit_

#include "pch.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace r
{
	typedef long(__stdcall* endScene)(IDXGISwapChain*, UINT, UINT);
	inline endScene pEndScene;
	inline ID3D11Device* pDevice;
	inline ID3D11DeviceContext* pContext;
	inline ID3D11RenderTargetView* mainRenderTargetView = NULL;

	endScene GetEndScene();
	inline hookobj<long(IDXGISwapChain*, UINT, UINT)> endscene_f;

	bool R_Init();
	void R_GetID3D11_Device(IDXGISwapChain* p_swap_chain);
	long __stdcall D3D_Draw(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags);
	void R_InitImGui();

	inline WNDPROC oWndProc;
	extern LRESULT WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void R_StartRender();
	void R_EndRender();
}

#endif