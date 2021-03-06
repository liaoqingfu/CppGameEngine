#include "Entity.h"

namespace GameEngine {

	Entity::Entity() 
		: GameObject(-1), _x(0.0f), _y(0.0f)
	{
		callUpdate = true;
	}

	Entity::Entity(float x, float y, int texture) 
		: GameObject(texture), _x(x), _y(y)
	{
		callUpdate = true;
	}

	Entity::Entity(float x, float y, std::string texture_name) 
		: Entity(x, y, Global::texture_manager.getID(texture_name))
	{
		callUpdate = true;
	}

	Entity::~Entity()
	{
		delete collider;
	}

	Shape * Entity::getCollider()
	{
		return collider;
	}

	void Entity::setCollider(Shape * newCollider)
	{
		delete collider;
		collider = newCollider;
	}

	float Entity::x()
	{
		return _x;
	}

	void Entity::x(float x)
	{
		_x = x;
	}

	float Entity::y()
	{
		return _y;
	}

	void Entity::y(float y)
	{
		_y = y;
	}

	void Entity::setVel(Vector2 newVel)
	{
		vel = newVel;
	}

	Vector2 & Entity::getVel()
	{
		return vel;
	}

}