#include "Bullet.h"


Bullet::Bullet(Animation animation,int x, int y)
{
	this->animation = animation;
	this->x = x;
	this->y = y;
}


Bullet::Bullet()
{
}

Bullet::~Bullet()
{
	//delete(this);
}

