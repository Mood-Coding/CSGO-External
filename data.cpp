#include "pch.h"
#include "data.h"

uintptr_t ClientBaseAddress;
uintptr_t LocalPlayerAddress;
DWORD ViewMatrixAddress;
uintptr_t EntityListAddress;
Vector3 LocalPlayerOrigin;
float ViewMatrix[16];

int BoxESPValue = 0;
int HealthESPValue = 0;

std::vector<Entity> Players;

uint8_t pEntityListData[0x1000];