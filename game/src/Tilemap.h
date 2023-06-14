#pragma once
#include "raylib.h"
#include "TileCoord.h"

const int TILE_SIZE = 40; //size of each tile

enum class TileType
{
    Floor = 0,
    Wall
};

struct Tile
{
    TileType tileType;
    std::vector<int> adjacentTiles; //list of adjacent tiles
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

                if (tile.tileType == TileType::Floor)
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

    bool IsTileTraversable(int x, int y)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return false;

        return GetTile(x, y).tileType == TileType::Floor;
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
                    DrawCircle(center.x, center.y, 5, WHITE);

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