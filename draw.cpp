#include "pch.h"
#include "draw.h"

Draw draw;

void Draw::Init(PDIRECT3DDEVICE9& pDIRECT3DDEVICE9, LPD3DXFONT& pFont)
{
	this->pDIRECT3DDEVICE9 = pDIRECT3DDEVICE9;
	this->pFont = pFont;

	this->isInitialized = true;

	if (D3DXCreateLine(this->pDIRECT3DDEVICE9, &this->pD3DXLINE) != D3D_OK)
	{
		std::cout << "error\n";
	}
}

void Draw::GetViewport()
{
	this->pDIRECT3DDEVICE9->GetViewport(&this->Viewport);
}

int Draw::TextWidth(string Text)
{
	RECT rect = {0, 0, 0, 0};
	draw.pFont->DrawTextA(NULL, Text.c_str(), -1, &rect, DT_CALCRECT, NULL);
	return rect.right - rect.left;
}

void Draw::Text(int x, int y, const char* text, D3DCOLOR color, bool isBordered, TextAlignment eAlignment)
{
	RECT rect;

	if (isBordered)
	{
		struct ShadowStruct { int x; int y; };
		ShadowStruct shadowOffset[] = {{-1, 0}, {+1, 0}, {0, -1}, {0, +1}};

		for (int i = 0; i < GetSizeOf(shadowOffset); i++)
		{
			SetRect(&rect, x + shadowOffset[i].x, y + shadowOffset[i].y, x + shadowOffset[i].x, y + shadowOffset[i].y);
			this->pFont->DrawTextA(NULL, text, -1, &rect, eAlignment | DT_NOCLIP, BlackColor(100));
		}
	}
	else
	{
		SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
		this->pFont->DrawTextA(NULL, text, -1, &rect, eAlignment | DT_NOCLIP, BlackColor(50));
	}

	SetRect(&rect, x, y, x, y);
	this->pFont->DrawTextA(NULL, text, -1, &rect, eAlignment | DT_NOCLIP, color);
}

void Draw::Line(float x, float y, float x2, float y2, D3DCOLOR color)
{
	Vertex v[2] =
	{
		{x,		y,	0.0f, 1.0f, color},
		{x2,	y2,	0.0f, 1.0f, color},
	};
	this->pDIRECT3DDEVICE9->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(Vertex));
}

void Draw::Border(float x, float y, float w, float h, D3DCOLOR color)
{
	/*color = D3DCOLOR_ARGB(255, 24, 24, 24);*/

	pD3DXLINE->SetWidth(1);
	D3DXVECTOR2 points[5];

	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x + w, y);
	points[2] = D3DXVECTOR2(x + w, y + h);
	points[3] = D3DXVECTOR2(x, y + h);
	points[4] = D3DXVECTOR2(x, y);

	pD3DXLINE->Draw(points, 5, color);
}

void Draw::Rectangle(float x, float y, float w, float h, D3DCOLOR startColor, D3DCOLOR endColor, D3DCOLOR borderColor)
{
	if (endColor == NULL)
		endColor = startColor;

	Vertex vertices[4] =
	{
		x,  y,			0.0f, 1.0f, startColor, 0.0f, 0.0f,
		x + w, y,		0.0f, 1.0f, startColor, 1.0f, 0.0f,

		x + w, y + h,	0.0f, 1.0f, endColor, 1.0f, 1.0f,
		x,  y + h,		0.0f, 1.0f, endColor, 0.0f, 1.0f,
	};

	this->pDIRECT3DDEVICE9->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(Vertex));

	if (borderColor != NULL)
		Border(x - 1, y - 1, w + 1, h + 1, borderColor);
}