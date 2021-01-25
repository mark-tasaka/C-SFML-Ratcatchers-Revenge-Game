#include "stdafx.h"
#include "Rat.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void Rat::spawn(float startX, float startY, int type, int seed)
{

	switch (type)
	{
	case 0:
		//bloater
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		m_Speed = 40;
		m_Health = 5;
		break;

	case 1:
		//chaser
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		m_Speed = 70;
		m_Health = 1;
		break;

	case 2:
		//crawler
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = 20;
		m_Health = 3;
		break;

	default:
		//default, for future use 'error graphic'
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = 0;
		m_Health = 0;
		break;
	}

	//modify the speed to make the Rat unique
	//every Rat is unique.  Create a speed modifier
	srand((int)time(0) * seed);
	//somewhere between 80 and 100
	float modifier = (rand() % MAX_VARIANCE + OFFSET);

	//express as a fraction of 1
	modifier /= 100;
	//now equals between 0.7 and 1
	m_Speed *= modifier;

	//initialize its location
	m_Position.x = startX;
	m_Position.y = startY;

	//set its origin to its centre
	m_Sprite.setOrigin(25, 25);

	//set its position
	m_Sprite.setPosition(m_Position);

}

bool Rat::hit()
{
	m_Health--;

	if (m_Health < 0)
	{
		//dead
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));

		return true;
	}

	//injured by not dead
	return false;
}

bool Rat::isAlive()
{
	return m_Alive;
}

FloatRect Rat::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Rat::getSprite()
{
	return m_Sprite;
}

void Rat::update(float elapsedTime, Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	//update the Rat position variables
	if (playerX > m_Position.x)
	{
		m_Position.x = m_Position.x + m_Speed * elapsedTime;
	}

	if (playerY > m_Position.y)
	{
		m_Position.y = m_Position.y + m_Speed * elapsedTime;
	}

	if (playerX < m_Position.x)
	{
		m_Position.x = m_Position.x - m_Speed * elapsedTime;
	}

	if (playerY < m_Position.y)
	{
		m_Position.y = m_Position.y - m_Speed * elapsedTime;
	}


	//move the sprite
	m_Sprite.setPosition(m_Position);

	//face the sprite in the correct direction
	float angle = (atan2(playerY - m_Position.y, playerX - m_Position.x) * 180) / 3.141;

	m_Sprite.setRotation(angle);

}