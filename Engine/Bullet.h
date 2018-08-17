#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Colors.h"
#include "Asteroid.h"

class Bullet

{
private:
	Vec2 pos;
	Vec2 direction;
	float speed;
	
	Color c;
	float radiusHit = 4.0f;
	Color colorHit;

public:	
enum class Origin
{
	player,
	alien
};
Origin origin;
static constexpr int damageOnHit = 5;
bool underway = false;
bool showHitAnimation = false;
float hitAnimationCounter = 0.0f;
static constexpr float hitAnimationTime = 0.1f;
void draw(Graphics& gfx);
void updateHitAnimationCounter(float dt);
void updatePos(float dt);
void init(Vec2 pos_in, Vec2 direction_in, Origin origin_in);
Vec2 getPos();
float radius;

};