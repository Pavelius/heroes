#include "main.h"

struct pointc
{
	char			x, y;
};
struct shapeinfo
{
	unsigned char	count;
	pointc			size;
	pointc			points[24];
	unsigned char	animation[24];
	unsigned char	indecies[24];
	pointc			offset;
	unsigned char	zero;
	unsigned char	initialized;
};
static shapeinfo sh1x1 = {1, {1, 1}, {{0, 0}}};
static shapeinfo sh1x1a6 = {1, {1, 1}, {{0, 0}}, {6}};
static shapeinfo sh1x1a11 = {1, {1, 1}, {{0, 0}}, {11}};
//
static shapeinfo sh2x1 = {2, {2, 1}, {{-1, 0}, {0, 0}}};
static shapeinfo sh2x1a6 = {2, {2, 1}, {{-1, 0}, {0, 0}}, {6, 6}};
static shapeinfo sh2x1a6v2 = {2, {2, 1}, {{-1, 0}, {0, 0}}, {0, 6}};
static shapeinfo sh2x2 = {4, {2, 2}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}};
static shapeinfo sh2x2J = {3, {2, 2}, {{0, -1}, {-1, 0}, {0, 0}}};
static shapeinfo sh2x2Ja9 = {3, {2, 2}, {{0, -1}, {-1, 0}, {0, 0}}, {9, 9, 9}};
static shapeinfo sh2x3 = {6, {2, 3}, {{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {-1, 1}, {0, 1}}};
//
static shapeinfo sh3x1 = {3, {3, 1}, {{-2, 0}, {-1, 0}, {0, 0}}};
static shapeinfo sh3x1a6 = {3, {3, 1}, {{-1, 0}, {0, 0}, {1, 0}}, {6, 6, 6}};
static shapeinfo sh3x2 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh3x2a3 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {3, 3, 3, 3, 3, 3}};
static shapeinfo sh3x2u1a6 = {6, {3, 2}, {{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 0}}, {6, 6, 0, 6, 6, 0}};
static shapeinfo sh3x2u1a6v2 = {5, {3, 2}, {{-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 0, 0, 0}};
static shapeinfo sh3x2u2 = {4, {3, 2}, {{1, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh3x2a6 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {0, 0, 0, 6, 6, 6}};
static shapeinfo sh3x2a15 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {15, 15, 15, 15, 0, 0}};
static shapeinfo sh3x2u1 = {5, {3, 2}, {{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh3x2r1 = {5, {3, 2}, {{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh3x2u1r1 = {4, {3, 2}, {{0, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh3x2u2a10 = {4, {3, 2}, {{1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {10, 0, 10, 0}};
static shapeinfo sh3x2u2a9 = {4, {3, 2}, {{1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {9, 9, 9, 0}};
static shapeinfo sh3x3 = {9, {3, 3}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}};
static shapeinfo sh3x3u1u1 = {7, {3, 3}, {{0, -1}, {1, -1}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}};
static shapeinfo sh3x3u1r1 = {7, {3, 3}, {{0, 0}, {-1, 1}, {0, 1}, {1, 1}, {-1, 2}, {0, 2}, {1, 2}}};
static shapeinfo sh3x3d1b1 = {7, {3, 3}, {{-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}, {0, 2}}};
static shapeinfo sh3x3u1d1 = {7, {3, 3}, {{0, -2}, {1, -2}, {-1, -1}, {0, -1}, {1, -1}, {0, 0}, {1, 0}}};
static shapeinfo sh3x3u1r1a6 = {7, {3, 3}, {{0, -2}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {0, 6, 6, 0, 6, 6, 0}};
static shapeinfo sh3x3r1a6 = {8, {3, 3}, {{0, -2}, {1, -2}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {6, 0, 0, 6, 0, 0, 0, 0}};
static shapeinfo sh3x3u2u2 = {5, {3, 3}, {{0, -2}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}};
static shapeinfo sh3x3u2u2a6 = {5, {3, 3}, {{0, -2}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 0, 6}};
static shapeinfo sh3x3u2m1 = {6, {3, 3}, {{0, -2}, {-2, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {0, 0, 9, 0, 0}};
static shapeinfo sh3x2u2a5 = {4, {3, 2}, {{0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 6}};
static shapeinfo sh3x2u2a5v2 = {4, {3, 2}, {{0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 0}};
//
static shapeinfo sh4x1 = {4, {4, 1}, {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2 = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2u1 = {7, {4, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2r1 = {7, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2u2 = {6, {4, 2}, {{0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2d1 = {7, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2r1d1 = {6, {4, 2}, {{-1, -1}, {0, -1}, {1, -1}, {0, 0}, {1, 0}, {2, 0}}};
static shapeinfo sh4x2u1b1 = {6, {4, 2}, {{0, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2�6 = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0, 0, 0, 0, 0, 0, 0, 6}};
static shapeinfo sh4x2�6_rogcmp = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0, 0, 0, 0, 0, 0, 6, 0}};
static shapeinfo sh4x3 = {12, {4, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}}};
static shapeinfo sh4x3u1r1 = {10, {4, 3}, {{-1, -2}, {0, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x3r1d1 = {10, {4, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}};
static shapeinfo sh4x3u1b1 = {10, {4, 3}, {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-2, 1}, {-1, 1}, {0, 1}}};
static shapeinfo sh4x3u2a3 = {10, {4, 3}, {{0, -2}, {1, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {3, 3, 3, 3, 3, 3, 3, 3, 3, 3}};
static shapeinfo sh4x3u2r1d1a9 = {8, {4, 3}, {{-1, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {8, 8, 0, 0, 0, 0, 0, 0}};
//
static shapeinfo sh5x2 = {10, {5, 2}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh5x2u1 = {9, {5, 2}, {{-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
static shapeinfo sh5x2r1 = {9, {5, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
static shapeinfo sh5x3 = {14, {5, 3}, {{-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}}};
static shapeinfo sh5x3a6 = {9, {5, 3}, {{1, -2}, {-2, -1}, {-1, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {6, 6, 6, 6, 0, 0, 0, 0, 0}};
static shapeinfo sh5x3a4 = {9, {5, 3}, {{1, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {10, 10, 10, 0, 10, 0, 0, 0, 0}};
static shapeinfo sh5x3u2a6 = {13, {5, 3}, {{-1, -2}, {0, -2}, {1, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {6, 0, 0, 6, 0, 6, 6, 0, 0, 6, 6, 0, 0}};
//
static shapeinfo sh6x2 = {12, {6, 2}, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}};
static shapeinfo sh6x3r1d2 = {15, {6, 3}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}}};
static shapeinfo sh6x3u1b2 = {15, {6, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}}};
static shapeinfo sh6x3u1a10 = {17, {6, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}, {0, 0, 0, 0, 0, 10, 10, 10, 10, 10, 0, 0, 10, 10, 10, 10, 0}};
static shapeinfo sh6x4 = {24, {6, 4}, {{-3, -3}, {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
static shapeinfo sh6x4r1d2 = {21, {6, 4}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {-1, 2}, {0, 2}, {1, 2}, {2, 2}}};
static shapeinfo sh6x4u1b2 = {21, {6, 4}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {-3, 2}, {-2, 2}, {-1, 2}, {0, 2}}};
//
static shapeinfo sh7x3r1 = {20, {7, 3}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}};
static shapeinfo sh7x4 = {22, {7, 4}, {{-2, -3}, {-1, -3}, {0, -3}, {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {-5, -1}, {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
//
static shapeinfo sh8x3 = {24, {8, 3}, {{-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-4, 1}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}};
static shapeinfo sh8x5a10 = {23, {8, 5}, {{0, -4}, {1, -4}, {2, -4}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {3, -3}, {-3, -2}, {0, -2}, {1, -2}, {2, -2}, {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0}};
// Map object description
struct mapobjectinfo
{
	tokens			object;
	shapeinfo&		shape;
	unsigned char	first;
	unsigned char	last;
};
static mapobjectinfo grass[] = {
	{AbandoneMine, sh4x2}, // Haunted mine
	{Empthy, sh2x1},
	{Empthy, sh4x2d1},
	{Empthy, sh3x3},
	{Empthy, sh2x2J},
	{Empthy, sh3x1},
	{Empthy, sh3x1},
	{Empthy, sh3x2u1r1}, // Big boulder
	{Empthy, sh2x1}, // Big 
	{Rock, sh1x1},
	{Rock, sh2x1},
	{Rock, sh2x1},
	{Rock, sh2x1},
	{Rock, sh2x1},
	{Lake, sh5x3},
	{Flowers, sh1x1}, // Flower
	{Lake, sh4x2}, // Lake
	{Lake, sh3x1}, // Lake
	{Hill, sh3x1}, // Hill
	{Trees, sh4x2u1}, // Trees
	{Trees, sh3x2u1}, // Trees
	{Trees, sh2x2J}, // Trees
	{Empthy, sh4x1}, // Brush flowers
	{Empthy, sh4x1}, // Brush
	{Empthy, sh3x1}, // Brush small
	{Empthy, sh3x1}, // Brush small flowers
	{Empthy, sh3x1}, // Brush small to up
	{Empthy, sh2x1}, // Single brush
	{Empthy, sh4x1}, // Flowers red
	{Empthy, sh4x2u1}, // Flowers red
	{Empthy, sh4x1}, // Flowers red another
	{Empthy, sh4x1}, // Flowers blue
	{Empthy, sh3x2u1}, // Flowers blue
	{Empthy, sh1x1}, // Flowers tiny
	{Empthy, sh3x1}, // Flowers
	{Empthy, sh2x1}, // Flowers
	{Empthy, sh2x1}, // Flowers to up
	{Empthy, sh2x1}, // Flowers white
	{Empthy, sh2x1}, // Flowers ultraviolet
	{Hill, sh2x1}, // Hill
};
static mapobjectinfo grass2[] = {
	{HillFort, sh3x2u1},
	{HalflingHole, sh4x1},
	{DiggingHole, sh1x1},
	{Cliff, sh2x1},
	{Cliff, sh2x1},
	{Cliff, sh3x1},
	{SpriteHouse, sh4x2u2},
	{WindMill, sh4x3u2a3},
	{ArcherHouse, sh3x2u2a5},
	{GoblinHut, sh2x1},
	{PeasantHut, sh3x2u2a5},
	{Oracle, sh3x2},
	{Obelisk, sh2x2J},
};
static mapobjectinfo dirt[] = {
	{AbandoneMine, sh5x2},
	{Hole, sh2x1},
	{Hill, sh2x1},
	{Hill, sh3x1},
	{Empthy, sh2x1}, // Cracked earth
	{Empthy, sh2x1}, // Cracked earth
	{Empthy, sh2x1}, // Cracked earth
	{Lake, sh8x3},
	{Lake, sh5x2},
	{Lake, sh2x1},
	{Empthy, sh3x1}, // Brush
	{Empthy, sh3x1}, // Brush
	{Empthy, sh3x1}, // Brush
	{Empthy, sh2x1}, // Brush
	{Empthy, sh2x1}, // Brush
	{Flowers, sh3x1},
	{Flowers, sh3x1},
	{Flowers, sh3x1},
	{Flowers, sh3x1},
	{Flowers, sh2x1},
	{Flowers, sh1x1},
	{Flowers, sh2x1},
	{Flowers, sh1x1},
	{Flowers, sh1x1},
	{Rock, sh3x1},
	{Rock, sh3x2},
	{Rock, sh3x1},
	{Rock, sh2x1},
	{Rock, sh1x1},
	{Flowers, sh1x1},
	{Flowers, sh2x1},
	{Flowers, sh2x1},
	{Empthy, sh2x1},
	{Empthy, sh1x1},
	{Trees, sh3x2},
	{Trees, sh3x2u1},
	{Trees, sh2x2J},
	{Mushrooms, sh3x1},
	{HillFort, sh3x2u1},
	{HalflingHole, sh4x1},
	{DiggingHole, sh1x1},
	{Cliff, sh2x1},
	{Cliff, sh2x1},
	{Cliff, sh2x1},
	{SpriteHouseCity, sh4x2u2},
	{WindMill, sh4x3u2a3},
	{Oracle, sh3x2},
	{Obelisk, sh2x2J},
};
static mapobjectinfo snow[] = {
	{Hole, sh2x1}, // Hole in Earth
	{CentautCave, sh2x1},
	{CampFire, sh1x1a6},
	{DiggingHole, sh1x1}, // Digging
	{LeanTo, sh2x1},
	{Cliff, sh2x1}, // Cliff
	{Empthy, sh2x1}, // Cliff
	{Empthy, sh3x1}, // Cliff
	{Empthy, sh2x1}, // Rock
	{Empthy, sh3x2u1}, // Rock
	{Empthy, sh1x1}, // Rock
	{Empthy, sh2x1}, // Rock
	{Empthy, sh2x1}, // Rock
	{Empthy, sh3x1}, // Rock
	{Empthy, sh2x1}, // Rock
	{Empthy, sh2x1}, // Rock
	{Empthy, sh2x1}, // Wood (������)
	{Empthy, sh1x1}, // Wood (������)
	{Empthy, sh1x1}, // Grass
	{Empthy, sh1x1}, // Grass
	{Empthy, sh1x1}, // Grass
	{Empthy, sh3x2u1}, // Tree
	{Empthy, sh4x2u1}, // Tree
	{Empthy, sh2x2J}, // Tree
	{Empthy, sh3x2u1}, // Tree
	{Empthy, sh2x2J}, // Tree
	{Empthy, sh2x2J}, // Tree
	{Empthy, sh2x2J}, // Tree
	{Empthy, sh2x2J}, // Tree
	{Empthy, sh2x2J}, // Tree
	{Empthy, sh5x2r1}, // Lake
	{Empthy, sh3x1}, // Lake
	{Empthy, sh3x1}, // Lake
	{WindMill, sh4x3u2a3},
	{ThatchedHut, sh2x2J},
	{Obelisk, sh2x2J},
	{Sign, sh2x1},
	{AlchemyLab, sh4x2�6},
	{Graveyard, sh3x1},
	{WaterWheel, sh3x3u1r1a6},
	{Well, sh2x2J},
	{Empthy, sh4x2},
	{Graveyard, sh4x2},
};
static mapobjectinfo desert[] = {
	{Empthy, sh2x1}, // Hole in Earth
	{Empthy, sh2x1}, // Tree
	{Empthy, sh2x2J}, // Tree
	{Empthy, sh2x2J}, // Tree
	{Empthy, sh2x2J}, // Tree
	{Empthy, sh3x1}, // Hill
	{Empthy, sh3x1}, // Hill
	{Empthy, sh4x1}, // Tree
	{Empthy, sh2x1}, // Tree
	{Empthy, sh2x1}, // Tree
	{Empthy, sh2x1},
	{Empthy, sh2x1},
	{Empthy, sh1x1}, // Cactus
	{Empthy, sh1x1}, // Cactus
	{Empthy, sh2x1}, // Cactus
	{Empthy, sh2x1}, // Cactuses
	{Empthy, sh2x2J}, // Cactuses
	{Empthy, sh1x1}, // Cactuses
	{Empthy, sh2x1}, // Cactuses
	{Empthy, sh2x2J}, // Cactuses
	{Empthy, sh2x2J}, // Cactuses
	{Empthy, sh1x1}, // Cactuses
	{Empthy, sh2x1}, // Cactuses
	{Empthy, sh2x1}, // Cactuses
	{CampFire, sh2x1a6}, // Cactuses
	{Empthy, sh1x1}, // Cactuses
	{DesertTent, sh3x2u1}, // Tent
	{Empthy, sh2x2J},
	{Pyramid, sh3x2},
	{DeadSkeleton, sh2x1},
	{Sphinx, sh3x2u1r1},
	{CityDead, sh5x2},
	{Excavation, sh3x1},
	{Obelisk, sh2x2J},
	{Empthy, sh3x2u1},
	{Empthy, sh3x1}, // Hole
	{Empthy, sh3x2u1},
	{Sign, sh2x1},
	{Graveyard, sh3x1},
	{Mines, sh4x2},
};
static mapobjectinfo water[] = {
	{Bottle, sh1x1a11},
	{WaterChest, sh2x1a6},
	{Empthy, sh1x1a6}, // Error?
	{Empthy, sh3x2u1}, // Error?
	{Empthy, sh2x1a6}, // Woods
	{WaterBoatStation, sh3x2a6}, // Map crafter woods
	{Empthy, sh3x1a6}, // ��������
	{Empthy, sh2x1a6}, // ��������
	{Empthy, sh1x1a6}, // ���������
	{Empthy, sh3x2a15}, // �����
	{Empthy, sh2x1},
	{Empthy, sh2x1},
	{Empthy, sh2x1a6},
	{Empthy, sh3x2a3},
	{ShipWreck, sh3x2u1a6},
};
static mapobjectinfo water2[] = {
	{Empthy, sh2x2J}, // Rock
	{ShipWreck, sh3x3r1a6},
	{Boat, sh1x1},
};
static mapobjectinfo swamp[] = {
	{WitchHut, sh3x3u2u2a6}, // Rock
	{Empthy, sh2x1}, // Rock
	{Empthy, sh3x1}, // Herbs
	{Xanadu, sh5x3u2a6},
	{Mushrooms, sh3x1},
	{Empthy, sh1x1},// Digging hole
	{Empthy, sh7x3r1},// Lake
	{Empthy, sh4x2},// Lake
	{Empthy, sh4x3u1r1},// Lake
	{Empthy, sh2x1},// Flower
	{Empthy, sh3x1},// Flower
	{Empthy, sh2x1},// Flower
	{Empthy, sh4x1},// Flower
	{Empthy, sh2x1},// Flower
	{Empthy, sh2x1},// ���
	{Sign, sh1x1},
	{Empthy, sh5x2u1}, // ������
	{Empthy, sh3x1}, // ������
	{Empthy, sh3x1}, // ������
	{Empthy, sh3x1}, // ������
	{Empthy, sh2x1}, // �������
	{Empthy, sh3x3u1d1}, // ������
	{Empthy, sh3x2u1}, // ������
	{Empthy, sh3x2u1}, // ������
	{Empthy, sh2x1}, // �����
	{Empthy, sh4x1}, // �����
	{Empthy, sh3x1}, // �����
	{Empthy, sh1x1}, // �����
	{Empthy, sh2x1}, // ��������
	{Empthy, sh2x1}, // ��������
	{Empthy, sh1x1}, // ��������
	{Empthy, sh2x1}, // �����
	{Empthy, sh2x1}, // �����
	{Empthy, sh2x1}, // ����� (������)
	{Empthy, sh2x1}, // �����
	{Empthy, sh1x1}, // �����
	{Empthy, sh2x1}, // ������
	{Empthy, sh2x1}, // ������
	{Empthy, sh3x2u2}, // ������
	{Empthy, sh1x1}, // ������
	{Empthy, sh3x1}, // ��������
	{Obelisk, sh2x2J},
};
static mapobjectinfo wasteland[] = {
	{ArtesianSpring, sh3x2u1},
	{Empthy, sh2x1}, // Hole
	{Empthy, sh3x2u1}, // ������
	{Empthy, sh2x2J}, // ������
	{Empthy, sh2x1}, // ����
	{Empthy, sh1x1}, // ����� �������� �����
	{Empthy, sh1x1}, // ������
	{Empthy, sh3x2u1r1}, // ������
	{Empthy, sh3x1}, // ������
	{Empthy, sh3x2u1}, // ������
	{Empthy, sh2x1}, // ������
	{Empthy, sh3x1}, // ������
	{Empthy, sh3x1}, // ������
	{Empthy, sh3x1}, // ������
	{Empthy, sh2x1}, // ������
	{Empthy, sh2x2J}, // ������
	{Empthy, sh2x2J}, // ������
	{Empthy, sh2x2J}, // ������
	{Empthy, sh2x2J}, // ������
	{Empthy, sh2x1}, // �������� �����
	{Empthy, sh4x3}, // �������
	{Empthy, sh1x1}, // Digging
	{Empthy, sh1x1}, // �������� �����
	{Empthy, sh1x1}, // �������� �����
	{Wagon, sh2x1},
	{Empthy, sh6x3u1a10},
	{TrollBridge, sh4x2},
	{Empthy, sh3x2u2a10}, // �������� ����
	{Empthy, sh4x2u1},
	{Empthy, sh3x2}, // �������
	{Empthy, sh2x3}, // �������
	{Empthy, sh3x1}, // �������
	{Obelisk, sh2x2J},
	{SawMill, sh4x2},
};
static mapobjectinfo lava[] = {
	{Empthy, sh2x1}, // Hole
	{Empthy, sh4x2}, // ���� ������
	{Empthy, sh2x1}, // Error???
	{Empthy, sh2x3}, // Stones
	{Empthy, sh2x2J}, // Stones and lava
	{Empthy, sh3x2u1}, // Stones and lava
	{Empthy, sh1x1}, // Digg
	{Empthy, sh6x2}, // Fire Lake
	{Empthy, sh3x2}, // Fire Lake
	{Empthy, sh4x2}, // Fire Lake
	{Empthy, sh2x2}, // Lava
	{Empthy, sh2x2J}, // Lava
	{Empthy, sh2x2}, // Lava
	{Empthy, sh3x2r1}, // Lava
	{Empthy, sh3x2r1}, // Lava
	{Empthy, sh2x2}, // Volcano
	{Empthy, sh2x2Ja9}, // Steam
	{Obelisk, sh2x2J},
	{DemonCave, sh3x2u1},
	{Sign, sh2x1},
	{SawMill, sh4x2},
};
static mapobjectinfo lava2[] = {
	{Volcano, sh5x3a6},
	{Volcano, sh5x3a4},
};
static mapobjectinfo lava3[] = {
	{Volcano, sh8x5a10},
};
static mapobjectinfo trees[] = {
	{Trees, sh4x3r1d1},
	{Trees, sh4x3u1b1},
	{Trees, sh4x2r1d1},
	{Trees, sh4x2u1b1},
	{Trees, sh2x1},
	{Trees, sh2x1},
};
static mapobjectinfo mountains[] = {
	{Mountains, sh6x4r1d2},
	{Mountains, sh6x4u1b2},
	{Mountains, sh4x3r1d1},
	{Mountains, sh4x3u1b1},
	{Mountains, sh4x2r1d1},
	{Mountains, sh4x2u1b1},
	{Mines, sh5x2},
};
static mapobjectinfo mountains2[] = {
	{Mountains, sh6x4r1d2},
	{Mountains, sh6x4u1b2},
	{Mountains, sh6x3r1d2},
	{Mountains, sh6x3u1b2},
	{Mountains, sh4x3r1d1},
	{Mountains, sh4x3u1b1},
	{Mountains, sh4x2r1d1},
	{Mountains, sh4x2u1b1},
	{Mines, sh5x2},
};
static mapobjectinfo multiobj[] = {
	{Trees, sh2x2J},
	{Trees, sh2x1},
	{PeasantHut, sh3x2u2a9},
	{Fort, sh4x3u2r1d1a9},
	{Gazebo, sh2x2J},
	{Flowers, sh3x1},
	{DoctorHut, sh3x2u2},
	{MercenaryCamp, sh3x1},
	{Ruins, sh2x1},
	{Shrine2, sh2x1},
	{Shrine3, sh2x1},
	{Shrine1, sh2x1},
	{Idol, sh2x1},
	{DruidicCircle, sh3x1},
	{Temple, sh2x2},
	{MarketPlace, sh3x2u2a5v2},
	{SpriteHouse, sh2x2J},
	{ThatchedHut, sh2x1},
	{TreeKnowledge, sh3x3u1u1},
	{CampFire, sh2x1a6},
};
static mapobjectinfo multiobj2[] = {
	{RiverDeltaDown, sh3x3u1r1},
	{RiverDeltaDown, sh3x3d1b1},
	{Fountain, sh2x1},
	{Stumps, sh1x1},
	{Stumps, sh2x1},
	{Stumps, sh1x1},
	{AlchemyLab, sh4x2�6},
	{DragonCity, sh7x4},
	{Graveyard, sh3x1}, // Not usable
	{LitghHouse, sh3x3u2m1},
	{SawMill, sh4x2}, // On brown dirt
	{WaterWheel, sh3x3u1r1a6},
	{Sign, sh2x1},
	{Teleporter, sh2x1},
	{Teleporter, sh2x2J},
	{Teleporter, sh2x2J},
	{RogueCamp, sh4x2�6_rogcmp},
	{Empthy, sh6x4}, // Error ??
	{MagicWell, sh2x1},
	{Event, sh1x1},
	{MagicWell, sh2x1},
	{FreemanFoundry, sh3x2u1a6v2},
	{MagicGarden, sh2x1a6v2},
	{Leprechaunt, sh1x1}, // Lephrechaunt is no here
	{WatchTower, sh3x2u2},
	{Graveyard, sh4x2},
	{SawMill, sh4x2}, // Green grass
};
static struct mapobjectset
{
	tokens			tile;
	res::tokens		icn;
	int				count;
	mapobjectinfo*	objects;
} mapobjectsets[] = {
	{Grass, res::OBJNGRAS, sizeof(grass) / sizeof(grass[0]), grass},
	{Grass, res::OBJNGRA2, sizeof(grass2) / sizeof(grass2[0]), grass2},
	{Dirt, res::OBJNDIRT, sizeof(dirt) / sizeof(dirt[0]), dirt},
	{Snow, res::OBJNSNOW, sizeof(snow) / sizeof(snow[0]), snow},
	{Desert, res::OBJNDSRT, sizeof(desert) / sizeof(desert[0]), desert},
	{Water, res::OBJNWATR, sizeof(water) / sizeof(water[0]), water},
	{Water, res::OBJNWAT2, sizeof(water2) / sizeof(water2[0]), water2},
	{Swamp, res::OBJNSWMP, sizeof(swamp) / sizeof(swamp[0]), swamp},
	{Wastelands, res::OBJNCRCK, sizeof(wasteland) / sizeof(wasteland[0]), wasteland},
	{Lava, res::OBJNLAVA, sizeof(lava) / sizeof(lava[0]), lava},
	{Lava, res::OBJNLAV2, sizeof(lava2) / sizeof(lava2[0]), lava2},
	{Lava, res::OBJNLAV3, sizeof(lava3) / sizeof(lava3[0]), lava3},
	{Trees, res::TREDECI, sizeof(trees) / sizeof(trees[0]), trees},
	{Trees, res::TREFALL, sizeof(trees) / sizeof(trees[0]), trees},
	{Trees, res::TREJNGL, sizeof(trees) / sizeof(trees[0]), trees},
	{Trees, res::TREEVIL, sizeof(trees) / sizeof(trees[0]), trees},
	{Trees, res::TREFIR, sizeof(trees) / sizeof(trees[0]), trees},
	{Trees, res::TRESNOW, sizeof(trees) / sizeof(trees[0]), trees},
	{Mountains, res::MTNDIRT, sizeof(mountains2) / sizeof(mountains2[0]), mountains2},
	{Mountains, res::MTNCRCK, sizeof(mountains2) / sizeof(mountains2[0]), mountains2},
	{Mountains, res::MTNDSRT, sizeof(mountains) / sizeof(mountains[0]), mountains},
	{Mountains, res::MTNGRAS, sizeof(mountains) / sizeof(mountains[0]), mountains},
	{Mountains, res::MTNLAVA, sizeof(mountains) / sizeof(mountains[0]), mountains},
	{Mountains, res::MTNMULT, sizeof(mountains) / sizeof(mountains[0]), mountains},
	{Mountains, res::MTNSNOW, sizeof(mountains) / sizeof(mountains[0]), mountains},
	{Mountains, res::MTNSWMP, sizeof(mountains) / sizeof(mountains[0]), mountains},
	{Empthy, res::OBJNMULT, sizeof(multiobj) / sizeof(multiobj[0]), multiobj},
	{Empthy, res::OBJNMUL2, sizeof(multiobj2) / sizeof(multiobj2[0]), multiobj2},
};

void map_objects_initialize()
{
	// Initialize shapes
	for(auto& ts : mapobjectsets)
	{
		int start_ex = 0;
		for(int i = 0; i < ts.count; i++)
		{
			if(ts.objects[i].shape.initialized)
				continue;
			auto& sh = ts.objects[i].shape;
			auto index = 0;
			auto mx = 0;
			auto my = 0;
			for(int i = 0; i < sh.count; i++)
			{
				auto x = sh.points[i].x;
				if(x < mx)
					mx = x;
				auto y = sh.points[i].y;
				if(y < my)
					my = y;
				sh.indecies[i] = index;
				index += 1 + sh.animation[i];
				if(sh.points[i].x == 0 && sh.points[i].y == 0)
					sh.zero = index;
			}
			sh.offset.x = mx;
			sh.offset.y = my;
			sh.initialized = 1;
		}
	}
	// Initialize map objects
	for(auto& ts : mapobjectsets)
	{
		int start_ex = 0;
		for(int i = 0; i < ts.count; i++)
		{
			ts.objects[i].first = start_ex;
			auto& sh = ts.objects[i].shape;
			ts.objects[i].last = ts.objects[i].first + sh.indecies[sh.count - 1] + sh.animation[sh.count - 1];
			start_ex += sh.indecies[sh.count - 1] + 1 + sh.animation[sh.count - 1];
		}
	}
}

static mapobjectinfo* find_object(res::tokens icn, unsigned char frame)
{
	for(auto& ts : mapobjectsets)
	{
		if(ts.icn != icn)
			continue;
		for(int i = 0; i < ts.count; i++)
		{
			if(ts.objects[i].first <= frame && frame <= ts.objects[i].last)
				return ts.objects + i;
		}
	}
	return 0;
}

void tilesetsview()
{
	auto index = 0;
	auto ts_index = 0;
	char temp[64];
	while(true)
	{
		auto x1 = 200;
		auto y1 = 200;
		if(ts_index < 0)
			ts_index = 0;
		else if(ts_index >= sizeof(mapobjectsets) / sizeof(mapobjectsets[0]))
			ts_index = sizeof(mapobjectsets) / sizeof(mapobjectsets[0]) - 1;
		auto& ts = mapobjectsets[ts_index];
		if(index > ts.count - 1)
			index = ts.count - 1;
		else if(index < 0)
			index = 0;
		auto icn = ts.icn;
		auto& e = ts.objects[index];
		shapeinfo& sh = e.shape;
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		int e_count = sh.indecies[sh.count - 1] + 1 + sh.animation[sh.count - 1];
		szprint(temp, "object %1i/%5i (start=%2i, count=%3i, next=%4i)", index, e.first, e_count, e.first + e_count, ts.count);
		draw::text(0, 0, temp);
		point center = {0, 0};
		for(int i = 0; i < sh.count; i++)
		{
			auto px = x1 + sh.points[i].x * 32;
			auto py = y1 + sh.points[i].y * 32;
			auto frame = e.first + sh.indecies[i];
			if(sh.animation[i])
				draw::image(px, py, icn, frame + 1 + (draw::counter % sh.animation[i]));
			draw::image(px, py, icn, frame);
			if(sh.points[i].x == 0 && sh.points[i].y == 0)
			{
				center.x = px;
				center.y = py;
			}
			draw::rectb(px, py, px + 32, py + 32, 0xC);
		}
		int rx1 = x1 + sh.offset.x * 32;
		int ry1 = y1 + sh.offset.y * 32;
		draw::rectb(rx1, ry1, rx1 + sh.size.x * 32, ry1 + sh.size.y * 32, 0xBF);
		draw::cursor(res::ADVMCO, 0);
		draw::rectb(center.x + 1, center.y + 1, center.x + 32 - 1, center.y + 32 - 1, 0x72);
		int id = draw::input();
		switch(id)
		{
		case KeyEscape:
		case Cancel:
			return;
		case KeyRight:
			index++;
			break;
		case KeyLeft:
			index--;
			break;
		case KeyUp:
			ts_index++;
			break;
		case KeyDown:
			ts_index--;
			break;
		}
	}
}

struct mapobject : public drawable
{

	res::tokens		icn;
	short unsigned	index;
	short unsigned	count;
	mapobjectinfo*	info;

	int getid() const override;

	point getpos() const
	{
		auto x = map::i2x(index) * 32;
		auto y = map::i2y(index) * 32;
		return{(short)x, (short)y};
	}

	rect getrect() const override
	{
		auto pt = getpos();
		return{pt.x, pt.y, pt.x + 32, pt.y + 32};
	}

	point getzpos() const override
	{
		return getpos();
	}

	void painting(point camera, unsigned flags) const override
	{
		auto pt = getpos() - camera;
		auto& sh = info->shape;
		for(int i = 0; i < sh.count; i++)
		{
			auto px = pt.x + sh.points[i].x * 32 - 32;
			auto py = pt.y + sh.points[i].y * 32;
			auto frame = info->first + sh.indecies[i];
			if(sh.animation[i])
				draw::image(px, py, icn, frame + 1 + ((draw::counter + count) % sh.animation[i]));
			draw::image(px, py, icn, frame);
		}
	}

	int priority() const override
	{
		return getid();
	}

};
static mapobject objects[LastMapObject - FirstMapObject + 1];
static bsmeta::field fields[] = {
	BSREQ(mapobject, index, Index, Number),
	BSREQ(mapobject, count, Count, Number),
	{0}
};
BSMETA(mapobject, "Objects", "�������", FirstMapObject);

int mapobject::getid() const
{
	return FirstMapObject + (this - objects);
}

mapobject* find_object_on_map(short unsigned index, tokens type)
{
	for(int i=0; i<mapobjects.count; i++)
	{
		if(objects[i].index == index)
		{
			if(objects[i].info && objects[i].info->object==type)
				return objects + i;
		}
	}
	return 0;
}

void add_mapobject(int index, res::tokens icn, unsigned char frame, unsigned char object, unsigned char quantity)
{
	// ���������� ��� ����� ������� � ���
	if(icn == res::EXTRAOVR)
	{
		auto p = find_object_on_map(index, Mines);
		if(!p)
			// Error
			return;
		p->count = frame;
		return;
	}
	auto pi = find_object(icn, frame);
	if(!pi)
		// Error;
		return;
	// Skip all frame, tha are not zero point.
	if((pi->first + pi->shape.zero) != frame)
		return;
	if(mapobjects.from + mapobjects.count >= mapobjects.to)
		return;
	auto& e = objects[mapobjects.count++];
	e.icn = icn;
	e.index = index;
	e.count = quantity;
	e.info = pi;
}

static struct mapobject_drawable_plugin : public drawable::plugin
{
	void selecting(drawable** result, rect screen, unsigned flags) override
	{
		auto mode = flags&DWMask;
		if(mode != DWObjects)
			return;
		auto p = result;
		for(auto& e : objects)
		{
			if(!e.icn)
				break;
			if(!e.getrect().intersect(screen))
				continue;
			*p++ = &e;
		}
		*p = 0;
	}
} drawable_plugin;