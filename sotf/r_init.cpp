#include "pch.h"
r::endScene r::GetEndScene()
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	HANDLE handle = GetCurrentProcess();
	DWORD pid = GetProcessId(handle);
	g::gameWindow = FindWindowA(NULL, "SonsOfTheForest");
	sd.OutputWindow = g::gameWindow;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGISwapChain* swap_chain;
	ID3D11Device* device;

	const D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, feature_levels, 2, D3D11_SDK_VERSION, &sd, &swap_chain, &device, nullptr, nullptr) == S_OK) {
		void** vTable = *reinterpret_cast<void***>(swap_chain);
		swap_chain->Release();
		device->Release();

		return (endScene)vTable[8];
	}
	return nullptr;
}
bool r::R_Init()
{
	pEndScene = GetEndScene();

	if (!pEndScene) {
		__err::error("R_Init(): !pEndScene");
		return false;
	}
	std::cout << "endScene: " << pEndScene << '\n';
	return true;
}
void r::R_GetID3D11_Device(IDXGISwapChain* p_swap_chain)
{
	static bool get_once = true;

	if (!get_once)
		return;

	get_once = false;

	if (p_swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice) != S_OK) {
		__err::error("p_swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice) != S_OK");
	}
	pDevice->GetImmediateContext(&pContext);
	ID3D11Texture2D* pBackBuffer;
	p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	D3D11_RENDER_TARGET_VIEW_DESC desc = {};
	memset(&desc, 0, sizeof(desc));
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	pDevice->CreateRenderTargetView(pBackBuffer, &desc, &mainRenderTargetView);
	pBackBuffer->Release();

	oWndProc = (WNDPROC)SetWindowLongPtr(g::gameWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
	R_InitImGui();
	std::cout << "directx is now active and ready!\n";


}
LRESULT __stdcall r::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return 1l;

	switch (uMsg) {
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		ImGui_ImplDX11_InvalidateDeviceObjects();
	}

	return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);
}
void r::R_InitImGui()
{
	if (!pDevice || !pContext) {
		MessageBox(NULL, "failed", "!pDevice || !pContext", 0);
		exit(-1);
	}
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	std::string font_dir = "C:\\Windows\\Fonts\\Arial.ttf";
	io.Fonts->AddFontFromFileTTF(font_dir.c_str(), 12 * 2);
	io.Fonts->Build();

	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(g::gameWindow);
	ImGui_ImplDX11_Init(pDevice, pContext);

}
long __stdcall r::D3D_Draw(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags)
{
	if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		std::cout << "hello?\n";

	R_GetID3D11_Device(p_swap_chain);

	R_StartRender();

	ImGui::GetBackgroundDrawList()->AddText(ivec2(400, 400), Pixel(255, 0, 0, 255), "Hello World!!!!");

	R_EndRender();

	return endscene_f.orgFnc(p_swap_chain, sync_interval, flags);
}

void r::R_StartRender()
{
	if (!ImGui::GetCurrentContext()) {
		std::cout << "POSSIBLY NO CONTEXT!!!!\n";
		return;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


}
void r::R_EndRender()
{
	if (!ImGui::GetCurrentContext()) {
		std::cout << "POSSIBLY NO CONTEXT!!!!\n";
		return;
	}
	ImGui::EndFrame();
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);


	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}