#include "stdafx.h"
#include "Bullet.h"

using namespace sf;

//c'tor
Bullet::Bullet()
{
	m_BulletShape.setSize(sf::Vector2f(6, 6));
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
	//keep track of the bullet
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	//calculate the gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);

	//any gradient less than zero needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;
	}

	//calcaulate the ratio between x and y
	float ratioXY = m_BulletSpeed / (1 + gradient);

	//set the 'speed' horizontally and vertically
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	//point the bullet in the right direction
	if (targetX < startX)
	{
		m_BulletDistanceX *= -1;
	}

	if (targetY < startY)
	{
		m_BulletDistanceY *= -1;
	}

	//set a max range of 1,000 pixels
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	//Position the bullet ready to be drawn
	m_BulletShape.setPosition(m_Position);
}

void Bullet::stop()
{
	m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}

void Bullet::update(float elapsedTime)
{
	//update the bullet position variables
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	//move the bullet
	m_BulletShape.setPosition(m_Position);

	//has the bullet gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX || m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
}