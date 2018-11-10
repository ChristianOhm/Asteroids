#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Colors.h"
#include "Asteroid.h"
#include "PDVec.h"

class Bullet

{
private:
	Vec2 pos;
	Vec2 direction;
	float speed;
	Color c;
	float radiusHit = 4.0f;
	Bullet* nextBulletPtr = nullptr;
	float radius;

public:	
enum class Origin
{
	player,
	alien,
	hitAnimation
};
Origin origin;
static constexpr int damageOnHit = 5;
static constexpr float radiusHitAnimation = 5;
static constexpr float durationHitAnimation = 0.05f;
void draw(Graphics& gfx);
bool updatePosCheckScreen(float dt);
Vec2 getPos();
float getRadius();
Bullet(PDVec pdVec_in, Origin origin_in, Bullet* nextBulletPtr_in);
Bullet(Vec2 pos_in, Bullet* nextHitAnimationPtr_in);
~Bullet();
Bullet* separate();
void reLink(Bullet* toReplace, Bullet* replaceWith);
Bullet* getNext();
bool deactivateHitAnimation;

};