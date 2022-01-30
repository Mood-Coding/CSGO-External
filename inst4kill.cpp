#define WIN32_LEAN_AND_MEAN
#define VC_EXTRA_LEAN

#include "pch.h"
#include "d3d.h"
#include "draw.h"
#include "menu.h"
#include "utils.h"
#include "Memory.hpp"
#include "data.h"

string onOff[]		= { "Off", "On" };
string aimBone[]	= { "Torso", "Upper Torso", "Head", "Neck", "Nuts" };
string aimAngle[]	= { "360", "180", "90", "45", "30", "15" };
string multiplier[]	= { "Off", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10" };
string aimTime[]	= { "Instant", "100ms", "200ms", "300ms", "400ms", "500ms", "600ms", "700ms", "800ms", "900ms", "1000ms" };
string keys[]		= { "LClick", "MClick", "RClick", "Alt", "Ctrl", "Shift" };

void InitializeMenu()
{
	menu.Header("Very Shitty CSGO Source");

	menu.AddTab("Visual");
	menu.AddItem("Box ESP", onOff, BoxESPValue, 2);
	menu.AddItem("Health ESP", onOff, HealthESPValue, 2);
}

int main()
{
	HWND TargetHWND = FindWindow(NULL, L"Counter-Strike: Global Offensive - Direct3D 9");
	RECT rct;
	GetClientRect(TargetHWND, &rct);

	d3d.Initialize(rct.right, rct.bottom, TargetHWND);
	InitializeMenu();
	menu.OverlayHWND = d3d.GetOverlayHWND();

	draw.Init(d3d.GetDevice(), d3d.GetFont());

	Memory mem;
	mem.Init(mem.GetProcessID(L"csgo.exe"));
	ClientBaseAddress = mem.GetProcessModuleBaseAddress(L"client.dll", mem.GetProcessID(L"csgo.exe"));


	mem.Read(ClientBaseAddress + 0x218090, &ViewMatrixAddress, sizeof(ViewMatrixAddress));
	ViewMatrixAddress += 0x80;
	
	EntityListAddress = ClientBaseAddress + 0x4DD1E0C + 0x10 + 0x10;

	std::thread OverlayWindowManager(MoveOverlay, d3d.GetOverlayHWND(), &d3d.WindowWidth, &d3d.WindowHeight);

	// Prepare FPS limiter
	std::chrono::steady_clock::time_point next_frame = std::chrono::steady_clock::now();

	while (true)
	{
		mem.Read(ClientBaseAddress + 0xDB65EC, &LocalPlayerAddress, sizeof(LocalPlayerAddress));

		mem.Read(ViewMatrixAddress, &ViewMatrix, sizeof(ViewMatrix));

		mem.Read(LocalPlayerAddress + 0x138, &LocalPlayerOrigin, sizeof(LocalPlayerOrigin));

		uintptr_t EntityAddress;
		mem.Read(EntityListAddress, pEntityListData, sizeof(pEntityListData));
		int i = 0;
		int counter = 0;
		while (counter < 64)
		{			
			EntityAddress = *(uintptr_t*)(pEntityListData + i*0x10);

			if (EntityAddress != 0)
			{
				Entity ent;
				mem.Read(EntityAddress, &ent.Player, sizeof(ent.Player));
				ent.Distance = DistanceBtw2Vector3(LocalPlayerOrigin, ent.Player.vecOrigin) / 100.0f;
				Players.emplace_back(ent);
			}

			++counter;
			++i;
		}

		if (!d3d.Begin())
			break;

		menu.Render();

		for (Entity ent : Players)
		{
			if (!WorldToScreen(ent.Player.vecOrigin, ent.ScreenPos, ViewMatrix, d3d.WindowWidth, d3d.WindowHeight)
				|| ent.Player.bIsDormant
				|| ent.Player.Health <= 0)
				continue;

			ent.Height = CalcScale(504, 0.9677, ent.Distance);
			ent.Width = CalcScale(208, 1, ent.Distance);
			
			if (BoxESPValue)
				draw.Border(ent.ScreenPos.x - ent.Width / 2, ent.ScreenPos.y - ent.Height, ent.Width, ent.Height, WHITE(255));
			
			if (HealthESPValue)
				draw.Text(ent.ScreenPos.x, ent.ScreenPos.y, std::to_string(ent.Player.Health).c_str(), RED(255), false, kCenter);
		}

		d3d.End();

		Players.clear();

		next_frame += std::chrono::milliseconds(16);
		std::this_thread::sleep_until(next_frame); // Wait for end of frame
	}

	system("pause");
	return 0;
}