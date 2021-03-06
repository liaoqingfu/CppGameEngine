#include "Tile.h"

namespace GameEngine {
	Tile::Tile() : GameObject(-1)
	{

	}
	Tile::Tile(std::string texture_name)
		: GameObject(Global::texture_manager.getID(texture_name))
	{
		
	}

	Tile::Tile(size_t texture_id)
	{
	}

	Tile::~Tile()
	{
	}

	__int32 Tile::GetX()
	{
		return x;
	}

	__int32 Tile::GetY() {
		return y;
	}

	void Tile::SetX(__int32 x)
	{
		this->x = x;
	}

	void Tile::SetY(__int32 y)
	{
		this->y = y;
	}

	bool Tile::blocking()
	{
		return false;
	}

	int Tile::weight()
	{
		return 1;
	}

}
