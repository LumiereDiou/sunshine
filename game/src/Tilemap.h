#pragma once
#include <vector>
#include "raylib.h"
#include "TileCoord.h"

#define TILEMAP_WIDTH 16
#define TILEMAP_HEIGHT 9

//const TileCoord NORTH = { 0, -1 };
//const TileCoord SOUTH = { 0,  1 };
//const TileCoord EAST = { 1,  0 };
//const TileCoord WEST = { -1,  0 };

const Vector2 NORTH = { 0, -1 };
const Vector2 SOUTH = { 0,  1 };
const Vector2 EAST = { 1,  0 };
const Vector2 WEST = { -1,  0 };

enum class Tile
{
	Floor = 0, // Floor tiles can be walked on
	Wall, // Wall tiles cannot be walked on
	Count // number of Tile types (i.e. 2)
};

class Tilemap
{
public:
	int tileSizeX = 80; // width of tiles in pixels when drawing
	int tileSizeY = 80; // height of tiles in pixels when drawing

private:
	//2D array of tiles (just IDs storing which type it is)
	//outer array is to pick a column, inner array is to pick a row from the column,
	//so to get to tile at xy position (0,6) access tiles[0][6]
	Tile tiles[TILEMAP_WIDTH][TILEMAP_HEIGHT];

public:
	size_t GetWidth() // get number of columns in the grid
	{
		return TILEMAP_WIDTH;
	}

	size_t GetHeight()  // get number of rows in the grid
	{
		return TILEMAP_HEIGHT;
	}

	Tile GetTile(TileCoord tilePos); // get the tile at the specified coordinate in the grid
	Tile GetTile(int x, int y)
	{
		Tile tileType = tiles[x][y];
		return tileType;
	}

	void SetTile(TileCoord tilePos, Tile value)  // set the tile at the specified coordinate in the grid
	{
		tiles[tilePos.x][tilePos.y] = value;
	}

	void SetTile(int x, int y, Tile value)
	{
		tiles[x][y] = value;
	}

	bool ContainsTile(TileCoord tilePosition); // returns true if the coordinate is inside the grid, false otherwise

	bool IsInsideLevel(Vector2 tilePosition)
	{
		if (tilePosition.x < 0 || tilePosition.x >= GetWidth() || tilePosition.y < 0 || tilePosition.y >= GetHeight())
			return false;
		else
			return true;
	}

	bool IsInsideLevel(int x, int y)
	{
		if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight())
			return false;
		else
			return true;
	}

	bool IsTraversableAt(TileCoord tilePosition); // Returns true if the specified tile is in the level and walkable, false otherwise
	bool IsTraversableAt(Vector2 tilePosition)
	{
		if (IsInsideLevel(tilePosition))
		{
			if (GetTile(tilePosition.x, tilePosition.y) == Tile::Floor) return true;
		}
		return false;
	}

	bool IsTraversableAt(int x, int y)
	{
		if (IsInsideLevel(x, y))
		{
			if (GetTile(x,y) == Tile::Floor) return true;
		}
		return false;
	}

	int GetCostForTile(TileCoord tilePositon)  // having this function makes it easier to change costs per tile the future
	{
		return 1;
	}

	Vector2 GetScreenPosOfTileCentered(TileCoord tilePos); //Convert from a tile coordinate to a screen position in middle of tile

	Vector2 GetScreenPosOfTile(TileCoord tilePosition) //Convert from a tile coordinate to a screen position at top left corner of tile
	{
		return { (float)tilePosition.x * tileSizeX , (float)tilePosition.y * tileSizeY };
	}
	
	Vector2 GetScreenPosOfTile(int x, int y) //Convert from a tile coordinate to a screen position at top left corner of tile
	{
		return { (float)x * tileSizeX , (float)y * tileSizeY };
	}

	TileCoord GetTileAtScreenPos(Vector2 positionOnScreen); //Find a tile coordinate given a position on the screen over a tile

	std::vector<TileCoord> GetAllTiles(); // return all tile positions

	std::vector<TileCoord> GetAllTraversableTiles(); // Get all Tiles which are Traversable (in the level and walkable)

	std::vector<TileCoord> GetTraversableTilesAdjacentTo(TileCoord tilePos); // For a given TileCoord, return all TileCoords which are adjacent and are traversable
	
	std::vector<Vector2> GetTraversableTilesAdjacentTo(Vector2 tilePos) // For a given TileCoord, return all TileCoords which are adjacent and are traversable
	{
		std::vector<Vector2> adjacentTilePositions;
		Vector2 N = tilePos + NORTH;
		Vector2 S = tilePos + SOUTH;
		Vector2 E = tilePos + EAST;
		Vector2 W = tilePos + WEST;
		if (IsTraversableAt(N)) adjacentTilePositions.push_back(N);
		if (IsTraversableAt(S)) adjacentTilePositions.push_back(S);
		if (IsTraversableAt(E)) adjacentTilePositions.push_back(E);
		if (IsTraversableAt(W)) adjacentTilePositions.push_back(W);

		return adjacentTilePositions;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// Drawing functions ////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	void DrawTiles() // Draw all tiles
	{
		for (int x = 0; x < GetWidth(); x++)
		{
			for (int y = 0; y < GetHeight(); y++)
			{
				Tile tileType = tiles[x][y];
				Vector2 tilePosition = GetScreenPosOfTile(x, y);
				Color colorToDraw = PINK;
				if (tileType == Tile::Floor) colorToDraw = WHITE;
				else if (tileType == Tile::Wall) colorToDraw = DARKGRAY;

				DrawRectangle(tilePosition.x, tilePosition.y, tileSizeX, tileSizeY, colorToDraw);

			}
		}
	}

	void DrawBorders(Color color = BLACK) // Draw lines between tiles
	{
		for (int x = 0; x < GetWidth(); x++)
			DrawLine(x * tileSizeX, 0, x * tileSizeX, GetHeight() * tileSizeY, color);
		for (int y = 0; y < GetHeight(); y++)
			DrawLine(0, y * tileSizeY, GetWidth() * tileSizeX, y * tileSizeY, color);
	}

	void DrawAdjacencies() // Draw lines indicating which tiles are connected to which others
	{

		for (int x = 0; x < GetWidth(); ++x)
		{
			for (int y = 0; y < GetHeight(); ++y)
			{
				Tile tileType = tiles[x][y];
				Vector2& tilePosition = GetScreenPosOfTile(x, y);
				std::vector<Vector2> adjacentTiles = GetTraversableTilesAdjacentTo(tilePosition);
				Color colorToDraw = PINK;
				if (tileType == Tile::Floor)
				{
					Vector2 center = { (tilePosition.x + tileSizeX / 2), (tilePosition.y + tileSizeY / 2) };
					DrawCircle(center.x, center.y, 5, RED);
					
					for (auto adjIndex : adjacentTiles)
					{
						Vector2 adjTilePosition = GetScreenPosOfTile(adjIndex.x, adjIndex.y);
						Vector2 adjCenter = { (adjTilePosition.x + tileSizeX / 2), (adjTilePosition.y + tileSizeY / 2) };
						DrawLineEx(center, adjCenter, 1, RED);
					}
				}
			}
		}
	}

	void DrawCoordinates(); // Write the TileCoordinate on each tile
};