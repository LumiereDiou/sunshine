#pragma once
#include "raylib.h"
#include "TileCoord.h"
#define MAP_WIDTH 32
#define MAP_HEIGHT 24

enum class Tile
{
	Floor = 0,
	Wall,
	Count // number of tile types
};

class Tilemap
{
public:
	float tileSizeX = 32;
	float tileSizeY = 32;
	Color tileColors[(int)Tile::Count];

	Tile tiles[MAP_WIDTH][MAP_HEIGHT];

	Tilemap()
	{
		tileColors[(int)Tile::Floor] = GREEN;
		tileColors[(int)Tile::Wall] = DARKGRAY;
	}

	void Randomize()
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				tiles[x][y] = (Tile)(rand() % (int)Tile::Count);
			}
		}
	}

	Vector2 GetScreenPositionOfTile(TileCoord coordinate)
	{
		return { coordinate.x + tileSizeX, coordinate.y + tileSizeY };
	}

	void Draw()
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				Tile tileType = tiles[x][y];
				Color tileColor = tileColors[(int)tileType];
				DrawRectangle(x * tileSizeX, y * tileSizeY, tileSizeX, tileSizeY, tileColor);
			}
		}
	}
};