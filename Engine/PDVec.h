#pragma once
#include"Vec2.h"
#include <math.h>

class PDVec
{
public:
	Vec2 pos;
	Vec2 direction;
	PDVec(Vec2 pos_in, Vec2 direction_in);
	PDVec rotateLeft(const float angle_in);
	PDVec rotateRight(const float angle_in);
};