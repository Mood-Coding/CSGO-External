#pragma once
#include "pch.h"
#include "draw.h"

#ifndef _D3D_

void MoveOverlay(HWND OverlayHWND, long* WindowWidth, long* WindowHeight);

template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class D3D
{
private:
	PDIRECT3D9 pDirect3D9;
	PDIRECT3DDEVICE9 pDIRECT3DDEVICE9;
	LPD3DXFONT pFont;

	HWND OverlayHWND;
	WNDCLASSEX wc;

	HWND GameHWND;

	bool InitializeD3D();
	bool CreateOverlay();

public:
	long WindowWidth;
	long WindowHeight;

	~D3D()
	{
		SafeRelease(&pFont);
		SafeRelease(&pDIRECT3DDEVICE9);
		SafeRelease(&pDirect3D9);

		::DestroyWindow(OverlayHWND);
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
	}

	bool Initialize(long windowWidth, long windowHeight, HWND gameHWND);
	/*bool Render();*/

	void MoveOverlay();

	bool Begin();
	void End();

	LPDIRECT3DDEVICE9& GetDevice() { return this->pDIRECT3DDEVICE9; }
	LPD3DXFONT& GetFont() { return this->pFont; }
	HWND GetOverlayHWND() { return this->OverlayHWND; }
};

extern D3D d3d;

#endif // !_D3D_
