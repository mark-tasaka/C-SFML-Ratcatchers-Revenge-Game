#pragma once
#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Rat
{
private:
	//how fast is each rat type?
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//how tough is each rat type
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//make each rat vary its speed slightly
	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	//where is this rat?
	Vector2f m_Position;

	//a sprite for the rat
	Sprite m_Sprite;

	//how fast can tis one run/crawl?
	float m_Speed;

	//how much health does it have?
	float m_Health;

	//is it still alive?
	bool m_Alive;

	//Public prototypes go here
public:

	//handle when a bullet hits a rat
	bool hit();

	//find out if the rat is alive
	bool isAlive();

	//spawn a new rat
	void spawn(float startX, float startY, int type, int seed);

	//return a rectangle that is the position in the world
	FloatRect getPosition();

	//get a copy of the sprite to draw
	Sprite getSprite();

	//update the rat each frame
	void update(float elapsedTime, Vector2f playerLocation);

};