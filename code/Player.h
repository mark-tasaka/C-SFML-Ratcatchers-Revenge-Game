#pragma once
#include <SFML/Graphics.hpp>



using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 800;

	//where is the player
	Vector2f m_Position;

	//Player Sprite
	Sprite m_Sprite;

	//add a texture
	Texture m_Texture;

	//screen resolution
	Vector2f m_Resolution;

	//size of the current arena
	IntRect m_Arena;

	//how big is each tile in the arena
	int m_TileSize;

	//direction player is currently moving in
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_leftPressed;
	bool m_RightPressed;

	//player's current health
	int m_Health;

	//player's max health
	int m_MaxHealth;

	//when was the player hit last
	Time m_LastHit;

	//speed in pixels per second
	float m_Speed;


	/*Public Functions*/

public:

	//C'tor
	Player();

	void spawn(IntRect arena, Vector2f resolution, int tielSize);

	//call this at the end of every game
	void resetPlayerStats();

	//handle the player getting hit by rat
	bool hit(Time timeHit);

	//how long ago was the Player last hit
	Time getLastHitTime();

	//where is the player
	FloatRect getPosition();

	//where is the centre of the player
	Vector2f getCenter();

	//which angle is the player facing
	float getRotation();

	//send a copy of the sprite to main()
	Sprite getSprite();

	//move the player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	//stop player from moving in specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	//called once every frame
	void update(float elaspedTime, Vector2i mousePosition);

	//gives player speed boost
	void upgradeSpeed();

	//gives player some health
	void upgradeHealth();

	//increase max amount of health for Player
	void increaseHealthLevel(int amount);

	//how much health the player has
	int getHealth();

};