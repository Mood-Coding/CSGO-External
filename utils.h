#pragma once
#include "pch.h"
#include "datatype.h"

bool WorldToScreen(Vector3 pos, Vector3& screen, float matrix[16], int windowWidth, int windowHeight);

float DistanceBtw2Vector3(Vector3 v1, Vector3 v2);

// Base / (1 + ScaleFactor * (Distance - 1))
int CalcScale(int Base, float ScaleFactor, float Distance);