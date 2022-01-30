#ifndef __DRAW_H__
#define __DRAW_H__

#include "pch.h"
#include "Colors.h"

#pragma warning(disable: 26495)

#define GetSizeOf(a)			sizeof(a)/sizeof(*a)

using namespace std;

enum TextAlignment { kLeft, kCenter, kRight };

class Draw
{
private:
	struct Vertex
	{
		float x, y, z, h;
		D3DCOLOR color;
		float tu, tv;
		static DWORD FVF;
	};

	bool isInitialized;

	PDIRECT3DDEVICE9 pDIRECT3DDEVICE9;
	LPD3DXFONT pFont;
	LPD3DXLINE pD3DXLINE;
	D3DVIEWPORT9 Viewport;

public:
	Draw()
	{
		isInitialized = false;
	}
	
	
	void Init(PDIRECT3DDEVICE9& pDIRECT3DDEVICE9, LPD3DXFONT& pFont);
	/*void Init() { this->isInitialized = true; }*/
	bool IsInitialized() { return this->isInitialized; }

	int TextWidth(string text);
	void Text(int x, int y, const char* text, D3DCOLOR color, bool isBordered = false, TextAlignment eAlignment = TextAlignment::kLeft);
	void Line(float x, float y, float x2, float y2, D3DCOLOR color);
	void Border(float x, float y, float w, float h, D3DCOLOR color);
	void Rectangle(float x, float y, float w, float h, D3DCOLOR startColor, D3DCOLOR endColor = NULL, D3DCOLOR borderColor = NULL);

	unsigned int GetScreenWidth() { return this->Viewport.Width; }
	unsigned int GetScreenHeight() { return this->Viewport.Height; }
	
	LPDIRECT3DDEVICE9 GetDevice() { return this->pDIRECT3DDEVICE9; }
	void SetDevice(LPDIRECT3DDEVICE9 device) { this->pDIRECT3DDEVICE9 = device; }

	void GetViewport();
};

#define opacity(v)				(255 * v) / 100

#define RedColor(a)				D3DCOLOR_ARGB(opacity(a), 255, 0, 0)
#define GreenColor(a)			D3DCOLOR_ARGB(opacity(a), 0, 255, 0)
#define BlueColor(a)			D3DCOLOR_ARGB(opacity(a), 0, 0, 255)
#define YellowColor(a)			D3DCOLOR_ARGB(opacity(a), 255, 255, 0)
#define OrangeColor(a)			D3DCOLOR_ARGB(opacity(a), 255, 125, 0)
#define WhiteColor(a)			D3DCOLOR_ARGB(opacity(a), 255, 255, 255)
#define BlackColor(a)			D3DCOLOR_ARGB(opacity(a), 0, 0, 0)

extern Draw draw;

#endif //__DRAW_H__