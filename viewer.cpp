#include "main.h"

// 1 - water / вода (30)
// 2 - grass / трава (62)
// 3 - snow / снег (62)
// 4 - swamp / болото (62)
// 5 - lava / лава (62)
// 6 - desert / пустыня (62)
// 7 - dirt / грязь (38)
// 8 - canion / каньон (62)
// 9 - beach / пляж (17)

// 1) Каждая местность имеет тайлы границ с пляжем и грязью.
// Причем в одном тайле может быть одновременно и то и другое.
// 2) Вода граничит только с пляжем.
// 3) Границу считать пляжем для: вода, пустыня, каньон.
//
// 4 - Земля сверху. Зеркально по верикали - снизу.
// 4 - Земля сверху и справа. Зеркально по вертикале - снизу и справа. Зеркально по горизонтале - сверху и слева. Зеркально по горизонтале и вертикале - снизу и слева.
// 4 - Земля справа. Зеркально по горизонтале - слева.
// 4 - Земля наискосок сверху-справа. Зеркально по аналогично.
// Блок земли (16). Блок песка (16). Блок земли и песка (6). Блок середины (8+8+8=24). Итого 62 на каждую местность.
// 8 - Это тайлы середины
// 8 - Это тайлы середины высоких объектов (холмы)
// 8 - Это тайлы середины низких объектов (цветы, трещинки)

struct terrain
{
	int			first;
	int			last;
	tokens		id;
};
static terrain tarrains[LastTerrain - FirstTerrain + 1] = {
	{0, 29, Water},
	{30, 91, Grass},
	{92, 145, Snow},
	{146, 207, Swamp},
	{208, 261, Lava},
	{262, 320, Desert},
	{321, 360, Dirt},
	{361, 414, Wastelands},
	{415, 431, Beach},
};
//enum shape_type {
//	SH1x1, SH1x1a6, SH1x1a11,
//	SH2x1, SH2x1a6, SH2x2, SH2x2J, SH2x2Ja9, SH2x3,
//	SH3x1, SH3x1a6, SH3x2, SH3x2a3, SH3x2u1a6, SH3x2u2, SH3x2a6, SH3x2a15, SH3x2u1, SH3x2r1, SH3x2u1r1, SH3x2u2a10, SH3x2u2a9,
//	SH3x3, SH3x3u1u1, SH3x3u1r1, SH3x3d1b1, SH3x3u1d1, SH3x3u1r1a6, SH3x3r1a6, SH3x3u2u2, SH3x3u2u2a6, SH3x2u2a5, SH3x2u2a5v2,
//	SH4x1, SH4x2, SH4x2u1, SH4x2r1, SH4x2u2, SH4x2d1, SH4x2r1d1, SH4x2u1b1, SH4x2а6, SH4x3, SH4x3u1r1, SH4x3r1d1, SH4x3u1b1, SH4x3u2a3, SH4x3u2r1d1a9,
//	SH5x2, SH5x2u1, SH5x2r1, SH5x3, SH5x3a6, SH5x3a4, SH5x3u2a6,
//	SH6x2, SH6x3r1d2, SH6x3u1b2, SH6x3u1a10, SH6x4r1d2, SH6x4u1b2,
//	SH7x3r1, SH7x4,
//	SH8x3, SH8x5a10,
//};
struct pointc
{
	char			x;
	char			y;
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
struct mapobjectinfo
{
	tokens			object;
	shapeinfo&		shape;
	int				first;
	int				last;
};
//static shapeinfo	shapes[] = {
//	{1, {1, 1}, {{0, 0}}},
//	{1, {1, 1}, {{0, 0}}, {6}},
//	{1, {1, 1}, {{0, 0}}, {11}},
//	//
//	{2, {2, 1}, {{-1, 0}, {0, 0}}},
//	{2, {2, 1}, {{-1, 0}, {0, 0}}, {6, 6}},
//	{4, {2, 2}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
//	{3, {2, 2}, {{0, -1}, {-1, 0}, {0, 0}}},
//	{3, {2, 2}, {{0, -1}, {-1, 0}, {0, 0}}, {9, 9, 9}},
//	{6, {2, 3}, {{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {-1, 1}, {0, 1}}},
//	//
//	{3, {3, 1}, {{-1, 0}, {0, 0}, {1, 0}}},
//	{3, {3, 1}, {{-1, 0}, {0, 0}, {1, 0}}, {6, 6, 6}},
//	{6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}},
//	{6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {3, 3, 3, 3, 3, 3}},
//	{6, {3, 2}, {{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {6, 6, 0, 6, 6}},
//	{4, {3, 2}, {{1, -1}, {-1, 0}, {0, 0}, {1, 0}}},
//	{6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {0, 0, 0, 6, 6, 6}},
//	{6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {15, 15, 15, 15, 0, 0}},
//	{5, {3, 2}, {{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}},
//	{5, {3, 2}, {{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {1, 0}}},
//	{4, {3, 2}, {{0, -1}, {-1, 0}, {0, 0}, {1, 0}}},
//	{4, {3, 2}, {{1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {10, 0, 10, 0}},
//	{4, {3, 2}, {{1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {9, 9, 9, 0}},
//	//
//	{9, {3, 3}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}},
//	{7, {3, 3}, {{0, -1}, {1, -1}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}},
//	{7, {3, 3}, {{0, 0}, {-1, 1}, {0, 1}, {1, 1}, {-1, 2}, {0, 2}, {1, 2}}},
//	{7, {3, 3}, {{-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}, {0, 2}}},
//	{7, {3, 3}, {{0, -2}, {1, -2}, {-1, -1}, {0, -1}, {1, -1}, {0, 0}, {1, 0}}},
//	{7, {3, 3}, {{0, -2}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {0, 6, 6, 0, 6, 6, 0}},
//	{8, {3, 3}, {{0, -2}, {1, -2}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {6, 0, 0, 6, 0, 0, 0, 0}},
//	{5, {3, 3}, {{0, -2}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}},
//	{5, {3, 3}, {{0, -2}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 0, 6}},
//	{4, {3, 2}, {{0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 6}},
//	{4, {3, 2}, {{0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 0}},
//	//
//	{4, {4, 1}, {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}}},
//	{8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}},
//	{7, {4, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}},
//	{7, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}},
//	{6, {4, 2}, {{0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}},
//	{7, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}},
//	{6, {4, 2}, {{-1, -1}, {0, -1}, {1, -1}, {0, 0}, {1, 0}, {2, 0}}},
//	{6, {4, 2}, {{0, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}}},
//	{8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0, 0, 0, 0, 0, 0, 0, 6}},
//	{12, {4, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}}},
//	{10, {4, 3}, {{-1, -2}, {0, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}},
//	{10, {4, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}},
//	{10, {4, 3}, {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-2, 1}, {-1, 1}, {0, 1}}},
//	{10, {4, 3}, {{0, -2}, {1, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {3, 3, 3, 3, 3, 3, 3, 3, 3, 3}},
//	{8, {4, 3}, {{-1, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {8, 8, 0, 0, 0, 0, 0, 0, 0, 0}},
//	//
//	{10, {5, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}},
//	{9, {5, 2}, {{-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}},
//	{9, {5, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}},
//	{14, {5, 3}, {{-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}}},
//	{9, {5, 3}, {{1, -2}, {-2, -1}, {-1, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {6, 6, 6, 6, 0, 0, 0, 0, 0}},
//	{9, {5, 3}, {{1, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {10, 10, 10, 0, 10, 0, 0, 0, 0}},
//	{13, {5, 3}, {{-1, -2}, {0, -2}, {1, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {6, 0, 0, 6, 0, 6, 6, 0, 0, 6, 6}},
//	//
//	{12, {6, 2}, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}},
//	{15, {6, 3}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}}},
//	{15, {6, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}}},
//	{17, {6, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}, {0, 0, 0, 0, 0, 10, 10, 10, 10, 10, 0, 0, 10, 10, 10, 10, 0, 0}},
//	{21, {6, 4}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {-1, 2}, {0, 2}, {1, 2}, {2, 2}}},
//	{21, {6, 4}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {-3, 2}, {-2, 2}, {-1, 2}, {0, 2}}},
//	//
//	{20, {7, 3}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}},
//	{22, {7, 4}, {{-2, -3}, {-1, -3}, {0, -3}, {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {-5, -1}, {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}},
//	//
//	{8 * 3, {8, 3}, {{-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-4, 1}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}},
//	{23, {8, 5}, {{0, -4}, {1, -4}, {2, -4}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {3, -3}, {-3, -2}, {0, -2}, {1, -2}, {2, -2}, {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0}},
//};
static shapeinfo sh1x1 = {1, {1, 1}, {{0, 0}}};
static shapeinfo sh1x1a6 = {1, {1, 1}, {{0, 0}}, {6}};
static shapeinfo sh1x1a11 = {1, {1, 1}, {{0, 0}}, {11}};
static shapeinfo sh2x1 = {2, {2, 1}, {{-1, 0}, {0, 0}}};
static shapeinfo sh2x1a6 = {2, {2, 1}, {{-1, 0}, {0, 0}}, {6, 6}};
static shapeinfo sh2x2 = {4, {2, 2}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}};
static shapeinfo sh2x2J = {3, {2, 2}, {{0, -1}, {-1, 0}, {0, 0}}};
static shapeinfo sh2x2Ja9 = {3, {2, 2}, {{0, -1}, {-1, 0}, {0, 0}}, {9, 9, 9}};
static shapeinfo sh2x3 = {6, {2, 3}, {{-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {-1, 1}, {0, 1}}};
static shapeinfo sh3x1 = {3, {3, 1}, {{-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh3x1a6 = {3, {3, 1}, {{-1, 0}, {0, 0}, {1, 0}}, {6, 6, 6}};
static shapeinfo sh3x2 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh3x2a3 = {6, {3, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}, {3, 3, 3, 3, 3, 3}};
static shapeinfo sh3x2u1a6 = {6, {3, 2}, {{0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 0}}, {6, 6, 0, 6, 6, 0}};
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
static shapeinfo sh3x2u2a5 = {4, {3, 2}, {{0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 6}};
static shapeinfo sh3x2u2a5v2 = {4, {3, 2}, {{0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {6, 6, 6, 0}};
static shapeinfo sh4x1 = {4, {4, 1}, {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2 = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2u1 = {7, {4, 2}, {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2r1 = {7, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2u2 = {6, {4, 2}, {{0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2d1 = {7, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2r1d1 = {6, {4, 2}, {{-1, -1}, {0, -1}, {1, -1}, {0, 0}, {1, 0}, {2, 0}}};
static shapeinfo sh4x2u1b1 = {6, {4, 2}, {{0, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x2а6 = {8, {4, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {0, 0, 0, 0, 0, 0, 0, 6}};
static shapeinfo sh4x3 = {12, {4, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}}};
static shapeinfo sh4x3u1r1 = {10, {4, 3}, {{-1, -2}, {0, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh4x3r1d1 = {10, {4, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}};
static shapeinfo sh4x3u1b1 = {10, {4, 3}, {{-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {-2, 1}, {-1, 1}, {0, 1}}};
static shapeinfo sh4x3u2a3 = {10, {4, 3}, {{0, -2}, {1, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {3, 3, 3, 3, 3, 3, 3, 3, 3, 3}};
static shapeinfo sh4x3u2r1d1a9 = {8, {4, 3}, {{-1, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}}, {8, 8, 0, 0, 0, 0, 0, 0}};
static shapeinfo sh5x2 = {10, {5, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
static shapeinfo sh5x2u1 = {9, {5, 2}, {{-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
static shapeinfo sh5x2r1 = {9, {5, 2}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
static shapeinfo sh5x3 = {14, {5, 3}, {{-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}}};
static shapeinfo sh5x3a6 = {9, {5, 3}, {{1, -2}, {-2, -1}, {-1, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {6, 6, 6, 6, 0, 0, 0, 0, 0}};
static shapeinfo sh5x3a4 = {9, {5, 3}, {{1, -2}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {10, 10, 10, 0, 10, 0, 0, 0, 0}};
static shapeinfo sh5x3u2a6 = {13, {5, 3}, {{-1, -2}, {0, -2}, {1, -2}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}, {6, 0, 0, 6, 0, 6, 6, 0, 0, 6, 6, 0, 0}};
static shapeinfo sh6x2 = {12, {6, 2}, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}};
static shapeinfo sh6x3r1d2 = {15, {6, 3}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}}};
static shapeinfo sh6x3u1b2 = {15, {6, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}}};
static shapeinfo sh6x3u1a10 = {17, {6, 3}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}, {0, 0, 0, 0, 0, 10, 10, 10, 10, 10, 0, 0, 10, 10, 10, 10, 0}};
static shapeinfo sh6x4r1d2 = {21, {6, 4}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {-1, 2}, {0, 2}, {1, 2}, {2, 2}}};
static shapeinfo sh6x4u1b2 = {21, {6, 4}, {{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {-3, 2}, {-2, 2}, {-1, 2}, {0, 2}}};
static shapeinfo sh7x3r1 = {20, {7, 3}, {{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}};
static shapeinfo sh7x4 = {22, {7, 4}, {{-2, -3}, {-1, -3}, {0, -3}, {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {-5, -1}, {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {-5, 0}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}}};
static shapeinfo sh8x3 = {24, {8, 3}, {{-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1}, {-4, 0}, {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {-4, 1}, {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}};
static shapeinfo sh8x5a10 = {23, {8, 5}, {{0, -4}, {1, -4}, {2, -4}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {3, -3}, {-3, -2}, {0, -2}, {1, -2}, {2, -2}, {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}}, {14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0}};

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
	{Empthy, sh2x1}, // Wood (пеньки)
	{Empthy, sh1x1}, // Wood (пеньки)
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
	{AlchemyLab, sh4x2а6},
	{Graveyard, sh3x1},
	{SawMill, sh3x3u1r1a6},
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
	{Empthy, sh3x1a6}, // Водросли
	{Empthy, sh2x1a6}, // Водросли
	{Empthy, sh1x1a6}, // Утопабщий
	{Empthy, sh3x2a15}, // Птицы
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
	{Empthy, sh2x1},// Мох
	{Sign, sh1x1},
	{Empthy, sh5x2u1}, // Заводь
	{Empthy, sh3x1}, // Заводь
	{Empthy, sh3x1}, // Заводь
	{Empthy, sh3x1}, // Заводь
	{Empthy, sh2x1}, // Кушинка
	{Empthy, sh3x3u1d1}, // Дерево
	{Empthy, sh3x2u1}, // Дерево
	{Empthy, sh3x2u1}, // Дерево
	{Empthy, sh2x1}, // Трава
	{Empthy, sh4x1}, // Трава
	{Empthy, sh3x1}, // Трава
	{Empthy, sh1x1}, // Трава
	{Empthy, sh2x1}, // Кувшинка
	{Empthy, sh2x1}, // Кувшинка
	{Empthy, sh1x1}, // Кувшинка
	{Empthy, sh2x1}, // Камыш
	{Empthy, sh2x1}, // Камыш
	{Empthy, sh2x1}, // Камыш (сверху)
	{Empthy, sh2x1}, // Лилии
	{Empthy, sh1x1}, // Лилии
	{Empthy, sh2x1}, // Камень
	{Empthy, sh2x1}, // Камень
	{Empthy, sh3x2u2}, // Камень
	{Empthy, sh1x1}, // Камень
	{Empthy, sh3x1}, // Кувшинка
	{Obelisk, sh2x2J},
};
static mapobjectinfo wasteland[] = {
	{ArtesianSpring, sh3x2u1},
	{Empthy, sh2x1}, // Hole
	{Empthy, sh3x2u1}, // Пальмы
	{Empthy, sh2x2J}, // Кактус
	{Empthy, sh2x1}, // Куст
	{Empthy, sh1x1}, // Череп рогатого скота
	{Empthy, sh1x1}, // Камень
	{Empthy, sh3x2u1r1}, // Камень
	{Empthy, sh3x1}, // Камень
	{Empthy, sh3x2u1}, // Камень
	{Empthy, sh2x1}, // Камень
	{Empthy, sh3x1}, // Камень
	{Empthy, sh3x1}, // Камень
	{Empthy, sh3x1}, // Камень
	{Empthy, sh2x1}, // Камень
	{Empthy, sh2x2J}, // Камень
	{Empthy, sh2x2J}, // Камень
	{Empthy, sh2x2J}, // Камень
	{Empthy, sh2x2J}, // Камень
	{Empthy, sh2x1}, // Засохшая трава
	{Empthy, sh4x3}, // Трещина
	{Empthy, sh1x1}, // Digging
	{Empthy, sh1x1}, // Засохшая трава
	{Empthy, sh1x1}, // Засохшая трава
	{Wagon, sh2x1},
	{Empthy, sh6x3u1a10},
	{TrollBridge, sh4x2},
	{Empthy, sh3x2u2a10}, // Торговый порт
	{Empthy, sh4x2u1},
	{Empthy, sh3x2}, // Трещина
	{Empthy, sh2x3}, // Трещина
	{Empthy, sh3x1}, // Трещина
	{Obelisk, sh2x2J},
	{SawMill, sh4x2},
};
static mapobjectinfo lava[] = {
	{Empthy, sh2x1}, // Hole
	{Empthy, sh4x2}, // Куча камней
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
	{AlchemyLab, sh4x2а6},
	{DragonCity, sh7x4},
	{Graveyard, sh3x1}, // Not usable
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

//static void write_shapes(const char* filename)
//{
//	static const char* names[] = {
//		"sh1x1", "sh1x1a6", "sh1x1a11",
//		"sh2x1", "sh2x1a6", "sh2x2", "sh2x2J", "sh2x2Ja9", "sh2x3",
//		"sh3x1", "sh3x1a6", "sh3x2", "sh3x2a3", "sh3x2u1a6", "sh3x2u2", "sh3x2a6", "sh3x2a15", "sh3x2u1", "sh3x2r1", "sh3x2u1r1", "sh3x2u2a10", "sh3x2u2a9",
//		"sh3x3", "sh3x3u1u1", "sh3x3u1r1", "sh3x3d1b1", "sh3x3u1d1", "sh3x3u1r1a6", "sh3x3r1a6", "sh3x3u2u2", "sh3x3u2u2a6", "sh3x2u2a5", "sh3x2u2a5v2",
//		"sh4x1", "sh4x2", "sh4x2u1", "sh4x2r1", "sh4x2u2", "sh4x2d1", "sh4x2r1d1", "sh4x2u1b1", "sh4x2а6", "sh4x3", "sh4x3u1r1", "sh4x3r1d1", "sh4x3u1b1", "sh4x3u2a3", "sh4x3u2r1d1a9",
//		"sh5x2", "sh5x2u1", "sh5x2r1", "sh5x3", "sh5x3a6", "sh5x3a4", "sh5x3u2a6",
//		"sh6x2", "sh6x3r1d2", "sh6x3u1b2", "sh6x3u1a10", "sh6x4r1d2", "sh6x4u1b2",
//		"sh7x3r1", "sh7x4",
//		"sh8x3", "sh8x5a10",
//	};
//	io::file file(filename, StreamWrite);
//	if(!file)
//		return;
//	for(int i = 0; i <= SH8x5a10; i++)
//	{
//		auto& sh = shapes[i];
//		file << "static shape " << names[i] << " = {"
//			<< sh.count << ","
//			<< "{" << sh.size.x << "," << sh.size.y << "}, ";
//		file << "{";
//		for(int j = 0; j < sh.count; j++)
//		{
//			file << "{" << sh.points[j].x << "," << sh.points[j].y << "}";
//			if(j != sh.count - 1)
//				file << ", ";
//		}
//		file << "}";
//		bool isanimation = false;
//		for(int j = 0; j < sh.count; j++)
//		{
//			if(sh.animation[j])
//			{
//				isanimation = true;
//				break;
//			}
//		}
//		if(isanimation)
//		{
//			file << ", {";
//			for(int j = 0; j < sh.count; j++)
//			{
//				file << (int)sh.animation[j];
//				if(j != sh.count - 1)
//					file << ", ";
//			}
//			file << "}";
//		}
//		file << "};\n";
//	}
//}

static void grassview()
{
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
		draw::rectb(center.x + 1, center.y + 1, center.x + 32 - 1, center.y + 32 - 1, 0x72);
		draw::cursor(res::ADVMCO, 0);
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

const char*	rsname(int res);

const char* strstr(const char* s1, const char* s2)
{
	auto n = zlen(s2) - 1;
	if(n <= 0)
		return 0;
	while(true)
	{
		auto p = zchr(s1, s2[0]);
		if(!p)
			return 0;
		if(memcmp(p + 1, s2 + 1, n) == 0)
			return p;
		s1 = p + 1;
	}
}

static int res_read(res::tokens* resdata)
{
	int count = 0;
	for(res::tokens i = res::FntBIGFONT; i < res::Empthy; i = (res::tokens)(i + 1))
	{
		const char* fn = rsname(i);
		if(!fn)
			continue;
		const char* ex = szext(fn);
		if(ex[0] != 'I'
			|| ex[1] != 'C'
			|| ex[2] != 'N')
			continue;
		if(resdata)
			resdata[count] = (res::tokens)i;
		count++;
	}
	return count;
}

static int find_index(res::tokens* resdata, int count, res::tokens icn)
{
	for(int i = 0; i < res::Empthy; i++)
	{
		if(resdata[i] == icn)
			return i;
	}
	return 0;
}

static void mapview()
{
	const int mx = 10;
	int offset = 0;
	while(true)
	{
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		for(int y = 0; y < 16; y++)
		{
			for(int x = 0; x < mx; x++)
			{
				int x1 = x * 32;
				int y1 = y * 32;
				draw::imager(x1, y1, res::TisGROUND32, offset + y*mx + x, 0);
			}
		}
		draw::cursor(res::ADVMCO, 0);
		char temp[32];
		szprint(temp, "%1i", offset + (hot::mouse.y / 32)*mx + (hot::mouse.x / 32));
		draw::text(600, 0, temp);
		int id = draw::input();
		switch(id)
		{
		case KeyEscape:
		case Cancel:
			return;
		case KeyUp:
			if(offset)
				offset -= mx;
			break;
		case KeyDown:
			offset += mx;
			break;
		}
	}
}

static void palview()
{
	while(true)
	{
		int index = -1;
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		for(int i = 0; i < 256; i++)
		{
			rect rc = {(i % 16) * 16 + 10, (i / 16) * 16 + 10};
			rc.x2 = rc.x1 + 14;
			rc.y2 = rc.y1 + 14;
			draw::rectf(rc.x1, rc.y1, rc.x2, rc.y2, i);
			if(hot::mouse.in(rc))
				index = i;
		}
		if(index != -1)
		{
			draw::state push;
			draw::font = res::SMALFONT;
			char temp[32];
			sznum(temp, index, 2, 0, 16);
			draw::text(320, 10, temp);
		}
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case KeyEscape:
		case Cancel:
			return;
		}
	}
}

static void shadowview()
{
	while(true)
	{
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		auto x = 200;
		auto y = 200;
		auto index = 96;
		auto icn = res::OBJNTWSH;
		draw::line(x - 32, y, x + 32, y, 0x0C);
		draw::line(x, y - 32, x, y + 32, 0x0C);
		x -= 2 * 32;
		y -= 3 * 32;
		for(int iy = 0; iy < 4; iy++)
		{
			for(int ix = -2; ix <= 1; ix++)
			{
				int x1 = x + ix * 32;
				int y1 = y + iy * 32;
				if(iy == 3)
					x1 += 32;
				draw::image(x1, y1, res::OBJNTWSH, index + iy * 4 + (ix + 2));
			}
		}
		icn = res::OBJNTOWN;
		draw::image(x + 2 * 32, y, res::OBJNTOWN, index);
		for(int ii = 0; ii < 5; ++ii)
			draw::image(x + ii * 32, y + 1 * 32, icn, index + 1 + ii);
		for(int ii = 0; ii < 5; ++ii)
			draw::image(x + ii * 32, y + 2 * 32, icn, index + 6 + ii);
		for(int ii = 0; ii < 5; ++ii)
			draw::image(x + ii * 32, y + 3 * 32, icn, index + 11 + ii);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case KeyEscape:
		case Cancel:
			return;
		}
	}
}

static int search_image()
{
	static struct resources_list : public list
	{

		bool	need_update;
		int		data[res::Empthy];
		char    name[32];

		static int compare(const void* p1, const void* p2)
		{
			return strcmp(rsname(*((int*)p1)), rsname(*((int*)p2)));
		}

		void prerender()
		{
			if(!need_update)
				return;
			maximum = 0;
			origin = 0;
			memset(data, 0, sizeof(data));
			for(res::tokens i = res::FntBIGFONT; (int)i < res::Empthy; i = res::tokens((int)i + 1))
			{
				const char* p = rsname(i);
				if(!strstr(p, ".ICN"))
					continue;
				if(strstr(p, name))
					data[maximum++] = i;
			}
			qsort(data, maximum, sizeof(data[0]), compare);
			need_update = false;
		}

		void row(int x, int y, int index) const
		{
			draw::state push;
			draw::font = res::SMALFONT;
			char temp[48];
			int id = data[index];
			zcpy(temp, rsname(id));
			*((char*)strstr(temp, ".ICN")) = 0;
			y += 6;
			x += 4;
			draw::text(x, y, temp);
			sznum(temp, res::getcount(res::tokens(id)));
			draw::text(x + 150, y, temp);
		}

	} resources;
	draw::screenshoot surface;
	char temp[260];
	int w1 = res::width(res::LISTBOXS, 0);
	int x = draw::width - w1;
	int y = 0;
	resources.need_update = true;
	while(true)
	{
		surface.restore();
		zcpy(temp, resources.name);
		draw::image(x, y, res::LISTBOXS, 0);
		draw::edit(x + w1 / 2, y + 5, resources.name, sizeof(resources.name));
		if(strcmp(temp, resources.name) != 0)
			resources.need_update = true;
		resources.boxs(x, y + 20, 9);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case Cancel:
		case KeyEscape:
			return -1;
		case KeyEnter:
			return resources.data[resources.current];
		case InputSymbol:
			break;
		}
	}
}

static int view()
{
	res::tokens resdata[res::Empthy];
	memset(resdata, 0, sizeof(resdata));
	int max_resource = res_read(resdata);
	int current_resource = 0;//find_index(resdata, max_resource, res::P_FLAG32);
	int current_frame = 0;
	int mode = 2;
	bool border_image = false;
	while(true)
	{
		char temp[256];
		char temp2[64];
		res::tokens icn = resdata[current_resource];
		int max_frame = res::getcount(icn);
		if(current_frame > max_frame)
			current_frame = max_frame - 1;
		draw::rectf(0, 0, draw::width - 1, draw::height - 1, 0x12);
		int x = draw::width / 2;
		int y = draw::height / 2;
		if(mode == 0)
		{
			x = 0;
			y = 0;
		}
		draw::image(x, y, icn, current_frame, (mode <= 1) ? AFNoOffset : 0);
		if(border_image)
		{
			draw::line(x - 32, y, x + 32, y, 0xBD);
			draw::line(x, y - 32, x, y + 32, 0xBD);
		}
		zcpy(temp2, rsname(icn));
		if(zfind(temp2, '.') != -1)
			temp2[zfind(temp2, '.')] = 0;
		szprint(temp, "icon \'%1\' is %2i of %3i, frame (%4i of %5i)", temp2, current_resource, max_resource, current_frame, max_frame);
		draw::text(10, 10, temp, -1);
		szprint(temp, "x=%1i, y=%2i, sx=%3i, sy=%4i", res::ox(icn, current_frame), res::oy(icn, current_frame), res::width(icn, current_frame), res::height(icn, current_frame));
		draw::text(10, 30, temp, -1);
		draw::cursor(res::ADVMCO, 0);
		int id = draw::input();
		switch(id)
		{
		case KeyLeft:
			current_frame--;
			break;
		case KeyRight:
			current_frame++;
			break;
		case KeyUp:
			current_resource++;
			current_frame = 0;
			break;
		case Shift + KeyUp:
			current_resource += 50;
			current_frame = 0;
			break;
		case KeyDown:
			current_resource--;
			current_frame = 0;
			break;
		case Shift + KeyDown:
			current_resource -= 50;
			current_frame = 0;
			break;
		case Ctrl + Alpha + 'F':
			draw::execute(Accept);
			break;
		case Alpha + 'G':
			grassview();
			break;
		case Alpha + 'P':
			palview();
			break;
		case Alpha + 'M':
			mapview();
			break;
		case Alpha + 'C':
			mode = (mode + 1) % 3;
			break;
		case Alpha + 'B':
			border_image = !border_image;
			break;
		case Alpha + 'S':
			shadowview();
			break;
		case Accept:
			id = search_image();
			if(id != -1)
				current_resource = find_index(resdata, max_resource, res::tokens(id));
			break;
		case Cancel:
			return 0;
		}
		if(current_frame >= max_frame)
			current_frame = max_frame - 1;
		if(current_frame < 0)
			current_frame = 0;
		if(current_resource >= max_resource)
			current_resource = max_resource - 1;
		if(current_resource < 0)
			current_resource = 0;
	}
}

int main()
{
	draw::font = res::FONT;
	draw::create("Heroes II - Resource Viewer", 200, false);
	view();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	return main();
}