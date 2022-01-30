#pragma once

#include "pch.h"

#ifndef _DATATYPE_H_

struct Vector3
{
	float x, y, z;
};

struct Vector4
{
	float x, y, z, w;
};

struct Vector2
{
	float x, y;
};

class C_CSPlayer
{
public:
	char pad_0000[148]; //0x0000
	float XVelocity; //0x0094
	float YVelocity; //0x0098
	float ZVelocity; //0x009C
	float XAxis; //0x00A0
	float YAxis; //0x00A4
	float ZAxis; //0x00A8
	float XAxis2; //0x00AC
	float YAxis2; //0x00B0
	float ZAxis2; //0x00B4
	char pad_00B8[16]; //0x00B8
	float Yaw; //0x00C8
	char pad_00CC[8]; //0x00CC
	float Yaw2; //0x00D4
	char pad_00D8[21]; //0x00D8
	bool bIsDormant; //0x00ED
	char pad_00EE[18]; //0x00EE
	int32_t Health; //0x0100
	char pad_0104[40]; //0x0104
	float Pitch; //0x012C
	float N000002F4; //0x0130
	char pad_0134[4]; //0x0134
	Vector3 vecOrigin; //0x0138
	char pad_0144[16]; //0x0144
}; //Size: 0x0154

class Entity
{
public:
	C_CSPlayer Player;
	Vector3 ScreenPos;
	float Distance;
	int Width;
	int Height;
};

#endif // !_DATATYPE_H_