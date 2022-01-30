#pragma once

#include "pch.h"
#include "datatype.h"

#ifndef _DATA_H_

extern uintptr_t ClientBaseAddress;
extern uintptr_t LocalPlayerAddress;
extern DWORD ViewMatrixAddress;
extern uintptr_t EntityListAddress;
extern Vector3 LocalPlayerOrigin;
extern float ViewMatrix[16];

extern std::vector<Entity> Players;

extern uint8_t pEntityListData[0x1000];

extern int BoxESPValue;
extern int HealthESPValue;

#endif // !_DATA_H_