#include "PDVec.h"

PDVec::PDVec(Vec2 pos_in, Vec2 direction_in)
{
	pos = pos_in;
	direction = direction_in;
}

PDVec PDVec::rotateLeft(const float angle_in)
{
	float newDirectionX = direction.x * cos(angle_in) - direction.y * sin(angle_in);
	float newDirectionY = direction.x * sin(angle_in) + direction.y * cos(angle_in);
	return PDVec(pos, Vec2(newDirectionX, newDirectionY));
}

PDVec PDVec::rotateRight(const float angle_in)
{
	float newDirectionX = direction.x * cos(-angle_in) - direction.y * sin(-angle_in);
	float newDirectionY = direction.x * sin(-angle_in) + direction.y * cos(-angle_in);
	return PDVec(pos, Vec2(newDirectionX, newDirectionY));
}
