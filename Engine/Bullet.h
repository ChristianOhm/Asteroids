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

public:	
enum class Origin
{
	player,
	alien
};
Origin origin;
static constexpr int damageOnHit = 5;
void draw(Graphics& gfx);
void updateHitAnimationCounter(float dt);
bool updatePosCheckScreen(float dt);
Vec2 getPos();
float radius;
Bullet(PDVec pdVec_in, Origin origin_in, Bullet* nextBulletPtr_in);
~Bullet();
Bullet* separate();
void reLink(Bullet* toReplace, Bullet* replaceWith);
Bullet* getNext();

};