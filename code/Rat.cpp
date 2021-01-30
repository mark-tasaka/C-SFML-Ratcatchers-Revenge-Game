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
		//Abomination
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/abomination.png"));
		m_Speed = ABOMINATION_SPEED;
		m_Health = ABOMINATION_HEALTH;
		break;

	case 1:
		//Runt
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/runt.png"));
		m_Speed = RUNT_SPEED;
		m_Health = RUNT_HEALTH;
		break;

	case 2:
		//Squeeler
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/squeeler.png"));
		m_Speed = SQUEELER_SPEED;
		m_Health = SQUEELER_HEALTH;
		break;

	default:
		//default, for future use 'error graphic'
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/runt.png"));
		m_Speed = RUNT_SPEED;
		m_Health = RUNT_HEALTH;
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
		//random gore
		int goreIndex = (rand() % 5 );



		//dead
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture(gore[goreIndex]));

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