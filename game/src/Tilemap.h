#pragma once
#include "raylib.h"
#include "TileCoord.h"

const int TILE_SIZE = 40; //size of each tile

const TileCoord NORTH = { 0, -1 };
const TileCoord SOUTH = { 0,  1 };
const TileCoord EAST = { 1,  0 };
const TileCoord WEST = { -1,  0 };

enum class TileType
{
    Floor = 0,
    Wall
};

struct Tile
{
    TileType tileType;
    std::vector<int> adjacentTiles; //list of adjacent tiles
    std::vector<int> traversableTiles;
};

class Tilemap
{
private:
    int width;  //columns
    int height; //rows
    std::vector<Tile> tiles;

public:
    Tilemap(int width, int height)
    {
        this->width = width; 
        this->height = height;
        tiles.resize(width * height);
    }

    Tile& GetTile(int x, int y)
    {
        return tiles[y * width + x];
    }

    TileCoord GetTileAtScreenPos(Vector2 positionOnScreen) //Find a tile coordinate given a position on the screen over a tile
    {
        return { (float)positionOnScreen.x / width , (float)positionOnScreen.y / height };
    }

    int GetWidth()
    {
        return width;
    }

    int GetHeight()
    {
        return height;
    }

    void GenerateLevel()
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Tile& tile = GetTile(x, y);
                tile.tileType = (rand() % 5 == 0) ? TileType::Wall : TileType::Floor;
            }
        }
    }

    void CreateAdjacentTiles()
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Tile& tile = GetTile(x, y);
                
                tile.adjacentTiles.clear();

                if (tile.tileType == TileType::Floor) //north south east and west
                {
                    if (y > 0 && GetTile(x, y - 1).tileType == TileType::Floor)
                        tile.adjacentTiles.push_back((y - 1) * width + x);

                    if (y < height - 1 && GetTile(x, y + 1).tileType == TileType::Floor)
                        tile.adjacentTiles.push_back((y + 1) * width + x);

                    if (x > 0 && GetTile(x - 1, y).tileType == TileType::Floor)
                        tile.adjacentTiles.push_back(y * width + (x - 1));

                    if (x < width - 1 && GetTile(x + 1, y).tileType == TileType::Floor)
                        tile.adjacentTiles.push_back(y * width + (x + 1));
                }
            }
        }
    }

    bool ContainsTile(TileCoord tilePosition)
    {
        if (tilePosition.x < 0 || tilePosition.x >= GetWidth() || tilePosition.y < 0 || tilePosition.y >= GetHeight())
            return false;
        else
            return true;
    }

    bool IsTileTraversable(int x, int y)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return false;

        return GetTile(x, y).tileType == TileType::Floor;
    }

    int GetCostForTile(TileCoord tilePositon)  // having this function makes it easier to change costs per tile the future
    {
        return 1;
    }

    std::vector<TileCoord> GetAllTraversableTiles()
    {
        std::vector<TileCoord> tilePositions;

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                TileCoord tilePositon{x,y};

                if (IsTileTraversable(x, y))
                {
                    tilePositions.push_back(tilePositon);
                }
            }
        }
        return tilePositions;
    }


    std::vector<TileCoord> GetTraversableTilesAdjacentTo(TileCoord tilePos)
    {
        std::vector<TileCoord> adjacentTilePositions;
        TileCoord N = tilePos + NORTH;
        TileCoord S = tilePos + SOUTH;
        TileCoord E = tilePos + EAST;
        TileCoord W = tilePos + WEST;
        if (IsTileTraversable(N.x, N.y)) adjacentTilePositions.push_back(N);
        if (IsTileTraversable(S.x, S.y)) adjacentTilePositions.push_back(S);
        if (IsTileTraversable(E.x, E.y)) adjacentTilePositions.push_back(E);
        if (IsTileTraversable(W.x, W.y)) adjacentTilePositions.push_back(W);

        return adjacentTilePositions;
    }

    void DrawTiles()
    {
        for(int x = 0; x < width; ++x)
        {
            for(int y = 0; y < height; ++y)
            {
                Tile& tile = GetTile(x, y);
                Rectangle rect{ x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

                switch (tile.tileType)
                {
                case TileType::Floor:
                    DrawRectangleRec(rect, BROWN);
                    break;

                case TileType::Wall:
                    DrawRectangleRec(rect, GRAY);
                    break;
                }

                DrawRectangleLinesEx(rect, 1, BLACK);

            }
        }
    }

    void DrawAdjacentLines()
    {
        for(int x = 0; x < width; ++x)
        {
            for(int y = 0; y < height; ++y)
            {
                Tile& tile = GetTile(x, y);
                Rectangle rect{ x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                // Draw adjacency circles and lines
                if (IsTileTraversable(x, y))
                {
                    Vector2 center = { (rect.x + rect.width / 2), (rect.y + rect.height / 2) };
                    //DrawCircle(center.x, center.y, 5, WHITE);

                    for (int adjTileIndex : tile.adjacentTiles)
                    {
                        Tile& adjTile = tiles[adjTileIndex];
                        Rectangle adjRect{ adjTileIndex % width * TILE_SIZE, adjTileIndex / width * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                        Vector2 adjCenter = { (adjRect.x + adjRect.width / 2), (adjRect.y + adjRect.height / 2) };
                        DrawLineEx(center, adjCenter, 1, WHITE);
                    }
                }
            }
        }
    }
};