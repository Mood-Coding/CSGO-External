#include "pch.h"
#include "d3d.h"

D3D d3d;

bool D3D::InitializeD3D()
{
	// Create the Direct3D interface
	pDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (pDirect3D9 == nullptr)
	{
		std::cout << "<Direct3DCreate9> Failed\n";
		return false;
	}

	// Create a struct to hold various device information
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));			 // clear out the struct for use
	d3dpp.Windowed = TRUE;						 // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = OverlayHWND;			 // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;    // set the back buffer format to 32-bit
	d3dpp.BackBufferWidth = this->WindowWidth;   // set the width of the buffer
	d3dpp.BackBufferHeight = this->WindowHeight; // set the height of the buffer
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	D3DCAPS9 caps;
	DWORD behaviorFlags;
	if (FAILED(pDirect3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		std::cout << "<GetDeviceCaps> Failed\n";
		return false;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		std::cout << "BehaviorFlags: D3DCREATE_HARDWARE_VERTEXPROCESSING\n";
	}
	else
	{
		behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		std::cout << "BehaviorFlags: D3DCREATE_SOFTWARE_VERTEXPROCESSING\n";
	}

	// create a device using behaviorFlags information and the information from the d3dpp stuct
	if (FAILED(pDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, OverlayHWND, behaviorFlags, &d3dpp, &pDIRECT3DDEVICE9)))
	{
		std::cout << "<CreateDevice> Failed!\n";
		return false;
	}

	if (D3DXCreateFontA(pDIRECT3DDEVICE9, 16, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Calibri", &pFont) != S_OK)
	{
		std::cout << "<D3DXCreateFont> Failed!\n";
		return false;
	}

	return true;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
		{
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		}
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool D3D::CreateOverlay()
{
	wc = WNDCLASSEX();
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wc.lpszMenuName = L"Overlay";
	wc.lpszClassName = L"Overlay";
	RegisterClassEx(&wc);

	OverlayHWND = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, wc.lpszMenuName, WS_POPUP, 0, 0, this->WindowWidth, this->WindowHeight, NULL, NULL, NULL, NULL);

	if (OverlayHWND)
	{
		std::cout << "Overlay HWND: " << OverlayHWND << '\n';
		return true;
	}
	else
	{
		std::cout << "<CreateWindowEx> Failed! " << GetLastError() << '\n';
		return false;
	}
}

bool D3D::Initialize(long windowWidth, long windowHeight, HWND gameHWND)
{
	std::cout << "<D3D> Initializing...\n";

	this->WindowWidth = windowWidth;
	this->WindowHeight = windowHeight;
	this->GameHWND = gameHWND;

	if (!this->CreateOverlay())
		return false;

	if (!this->InitializeD3D())
		return false;

	/*SetLayeredWindowAttributes(this->OverlayHWND, 0, 1, LWA_ALPHA);*/
	SetLayeredWindowAttributes(this->OverlayHWND, RGB(0, 0, 0), 255, /*LWA_COLORKEY |*/ LWA_ALPHA);
	//SetLayeredWindowAttributes(this->OverlayHWND, 0, RGB(0, 0, 0), LWA_COLORKEY); // this shit will cause dwm.exe high cpu usage
	MARGINS margin = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(this->OverlayHWND, &margin);

	ShowWindow(this->OverlayHWND, SW_SHOW);
	UpdateWindow(this->OverlayHWND);

	std::cout << "<D3D> Initialization success!\n";

	return true;
}


void MoveOverlay(HWND OverlayHWND, long* WindowWidth, long* WindowHeight)
{
	while (true)
	{
		HWND GameHWND = FindWindow(NULL, L"Counter-Strike: Global Offensive - Direct3D 9");
		if (GameHWND == NULL)
		{
			::PostQuitMessage(0);
			return;
		}

		RECT rct;
		GetClientRect(GameHWND, &rct);

		// Because client coordinates are relative to the upper-left corner of a window's client area,
		// the relative coordinates of the upper-left corner are always (0,0).
		POINT top_left;
		top_left.x = 0;
		top_left.y = 0;

		POINT bottom_right;
		bottom_right.x = rct.right;
		bottom_right.y = rct.bottom;

		// Converts relative coordinates of 2 corners to screen coordinates
		ClientToScreen(GameHWND, &top_left);
		ClientToScreen(GameHWND, &bottom_right);

		*WindowWidth = rct.right;
		*WindowHeight = rct.bottom;

		SetWindowPos
		(
			OverlayHWND,
			NULL,
			top_left.x,
			top_left.y,
			rct.right,
			rct.bottom,
			3u
		);

		// Move overlay window when we move game window
		MoveWindow
		(
			OverlayHWND,
			top_left.x,
			top_left.y,
			rct.right,
			rct.bottom,
			true
		);

		MARGINS margins{ -1, -1, -1, -1 };
		DwmExtendFrameIntoClientArea(OverlayHWND, &margins);

		Sleep(50);
	}
}

bool D3D::Begin()
{
	MSG msg{};
	// Handle input message
	if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}

	this->pDIRECT3DDEVICE9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0); // Clear the window alpha
	this->pDIRECT3DDEVICE9->BeginScene(); // Begin the 3D scene

	this->pDIRECT3DDEVICE9->SetTexture(NULL, NULL);
	this->pDIRECT3DDEVICE9->SetRenderState(D3DRS_ALPHABLENDENABLE, D3DZB_TRUE);
	this->pDIRECT3DDEVICE9->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	// To be able to use DrawPrimitiveUP
	this->pDIRECT3DDEVICE9->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	draw.GetViewport();

	return true;
}

void D3D::End()
{
	this->pDIRECT3DDEVICE9->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	this->pDIRECT3DDEVICE9->EndScene(); // End the 3D scene
	this->pDIRECT3DDEVICE9->Present(NULL, NULL, NULL, NULL); // Display the created frame on the screen
}
