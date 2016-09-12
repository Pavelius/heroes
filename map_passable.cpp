#include "main.h"

struct mapobject_t
{
	const char*	name;
};

static mapobject_t objects[LastObject-FirstObject+1] = {
	{""},
	{"Alchemist lab"},
	{"Sign"},
	{""},
	{"Skeleton"}, // 84
	{"Demon's cave"},
	{"Treasure chest"},
	{"Faery Ring"},
	{"Campfire"}, // 88
	{"Fountain"},
	{"Gazebo"},
	{"Ancient Lamp"},
	{"Graveyard"}, // 8C
	{"Archer house"},
	{"Goblin hut"},
	{"Dwarf cottage"},
	//
};

bool map::grass::ispassable(unsigned char index)
{
	static const unsigned char disabled[] = {54, 55, 56, 57, 58, 65, 66, 67, 68};
	static const unsigned char restricted[] = {5, 7, 31, 33, 34, 37, 38, 40, 41, 43, 45, 47, 49, 59,
		60, 61, 62, 63, 69, 70, 71, 72, 73, 74, 75, 77, 78, 83, 84, 85, 89, 90, 93, 95,
		96, 97, 99, 100, 101, 103, 104, 106, 107, 109, 110, 112, 114, 115, 116, 121, 122,
		123, 125, 126, 127, 129, 130, 131, 135, 136, 139, 140, 142, 144, 146, 148, 149, 150};
	for(auto a : disabled)
		if(a==index)
			return false;
	for(auto a : restricted)
		if(a==index)
			return false;
	return true;
}

bool map::dirt::ispassable(unsigned char index)
{
	static const unsigned char disabled[] = {23, 24, 25, 26, 27, 28, 29, 30, 32, 33, 34, 35, 36, 37, 48, 49, 50, 51};
	static const unsigned char restricted[] = {7, 9, 12, 13, 15, 16, 17, 18, 19, 20, 21, 22, 40, 41, 42, 43, 44, 45,
		53, 54, 55, 56, 57, 58, 60, 61, 63, 64, 66, 67, 69, 71, 73, 74, 76, 77, 79, 80, 82, 83, 85,
		86, 88, 89, 90, 92, 93, 98, 99, 101, 102, 104, 105, 118, 123, 127, 130, 133, 134, 137, 139,
		152, 189};
	for(auto a : disabled)
		if(a==index)
			return false;
	for(auto a : restricted)
		if(a==index)
			return false;
	return true;
}

bool map::desert::ispassable(unsigned char index)
{
	static const unsigned char disabled[] = {61, 89, 90, 91, 92, 93, 125, 126};
	static const unsigned char restricted[] = {3, 6, 9, 12, 14, 15, 17, 18, 20, 21, 22, 24, 26, 28, 30, 31, 32, 34,
		36, 39, 40, 42, 45, 48, 49, 51, 53, 72, 76, 81, 83, 94, 95, 97, 98, 99, 100, 110, 111, 112, 116, 121, 127, 128, 130};
	for(auto a : disabled)
		if(a==index)
			return false;
	for(auto a : restricted)
		if(a==index)
			return false;
	return true;
}

bool map::crack::ispassable(unsigned char index)
{
	static const unsigned char disabled[] = {58, 59, 63, 64, 65, 76, 77, 78, 80, 91, 102, 113, 124,
		135, 182, 183, 185, 221, 222, 223, 227, 228, 229, 230, 238, 241, 242, 245};
	static const unsigned char restricted[] = {5, 6, 10, 11, 14, 16, 17, 18, 21, 22, 24, 25, 29, 30, 31, 32, 34,
		35, 37, 38, 40, 41, 42, 43, 46, 49, 52, 55, 57, 62, 67, 68, 69, 71, 72, 136, 148, 159,
		170, 181, 186, 187, 188, 202, 224, 225, 226, 231, 232, 233, 234, 235, 243, 244, 246};
	for(auto a : disabled)
		if(a==index)
			return false;
	for(auto a : restricted)
		if(a==index)
			return false;
	return true;
}

bool map::multi::ispassable(unsigned char index)
{
	static const unsigned char restricted[] = {2, 4, 58, 63, 64, 65, 70, 72, 73, 89, 104};
	for(auto a : restricted)
		if(a==index)
			return false;
	return true;
}

bool map::multi::ispassable2(unsigned char index)
{
	static const unsigned char disabled[] = {46, 76, 77, 124, 125, 126, 221, 213};
	static const unsigned char restricted[] = {16, 18, 19, 25, 27, 51, 52, 53, 55, 57, 78, 79, 81, 98, 105, 128, 136, 187, 207, 209, 214, 215, 217};
	for(auto a : disabled)
		if(a==index)
			return false;
	for(auto a : restricted)
		if(a==index)
			return false;
	return true;
}

bool map::swamp::ispassable(unsigned char index)
{
	static const unsigned char disabled[] = {88, 89, 90, 91, 94, 95, 96, 97, 98, 108, 109, 110, 112,
		113, 115, 116, 118, 119, 122, 123, 143, 144};
	static const unsigned char restricted[] = {32, 33, 67, 74, 82, 85, 100, 101, 102, 103, 104, 105,
		126, 128, 129, 131, 133, 134, 135, 137, 138, 139, 145, 146, 147, 148, 149, 150,
		151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 166, 167, 171, 172, 176, 177,
		179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
		196, 198, 199, 200, 201, 203, 205, 208, 209, 212, 213};
	for(auto a : disabled)
		if(a==index)
			return false;
	for(auto a : restricted)
		if(a==index)
			return false;
	return true;
}


bool map::tree::isshadow(unsigned char index)
{
	static const unsigned char shadows[] = {0, 3, 7, 10, 13, 17, 20, 23, 26, 29, 32, 34};
	for(auto a : shadows)
		if(a==index)
			return true;
	return false;
}

bool map::tree::ispassable(unsigned char index)
{
	if(isshadow(index))
		return true;
	return (5 == index || 15 == index || 22 == index || 27 == index);
}

bool map::mountain::ispassable(unsigned char index)
{
	if(isshadow(index))
		return true;
	static const unsigned char disabled[] = {6, 7, 8, 9, 14, 15, 16, 28, 29, 30, 31, 33, 34, 35, 50, 51, 52, 65, 77, 78, 87, 94, 99, 112};
	for(auto a : disabled)
		if(a==index)
			return false;
	return true;
}

bool map::mountain::ispassable3(unsigned char index)
{
	if(isshadow(index))
		return true;
	static const unsigned char disabled[] = {6, 7, 8, 9, 14, 15, 16, 25, 28, 29, 30, 31, 33, 34, 35, 43, 50, 51, 52, 53, 54, 65, 77, 78, 87, 94, 99, 112};
	for(auto a : disabled)
		if(a==index)
			return false;
	return true;
}

bool map::mountain::ispassable2(unsigned char index)
{
	if(isshadow(index))
		return true;
	static const unsigned char disabled[] = {6, 7, 8, 9, 14, 15, 16, 28, 29, 30, 31, 33, 34, 35, 50, 51, 52, 65, 73, 77, 78, 84, 86, 87, 94, 99, 112};
	for(auto a : disabled)
		if(a==index)
			return false;
	return true;
}

bool map::mountain::isshadow(unsigned char index)
{
	static const unsigned char shadows[] = {0, 5, 11, 17, 21, 26, 32, 38, 42, 45, 49, 52, 55, 59, 62, 65, 68, 71, 74, 75, 79, 80};
	for(auto a : shadows)
		if(a==index)
			return true;
	return false;
}

bool map::mountain::isshadow2(unsigned char index)
{
	static const unsigned char shadows[] = {0, 5, 11, 17, 21, 26, 32, 38, 42, 46, 47, 53, 57, 58, 62,
		68, 72, 75, 79, 82, 85, 89, 92, 95, 98, 101, 104, 105, 109, 110};
	for(auto a : shadows)
		if(a==index)
			return true;
	return false;
}

bool map::towns::ispassable(unsigned char index0)
{
	int index = index0 % 32;
	// 13, 29, 45, 61, 77, 93, 109, 125, 141, 157, 173, 189
	if(13 == index || 29 == index)
		return true;
	// town/castle
	else if((5 < index && index < 13) || (13 < index && index < 16) ||
		(21 < index && index < 29) || (29 < index))
		return false;
	return true;
}

int map::grass::action(unsigned char id)
{
	switch(id)
	{
	case 6:	return AbandoneMine;
	case 30: return FaerieRing;
	default: return 0;
	}
}

int action2(unsigned char object, unsigned char id)
{
	switch(res::map(object))
	{
	case res::OBJNGRAS:
		return map::grass::action(id);
	//case res::OBJNDIRT:
	//	return map::dirt::ispassable(id);
	//case res::OBJNDSRT:
	//	return map::desert::ispassable(id);
	//case res::OBJNCRCK:
	//	return map::crack::ispassable(id);
	//case res::OBJNMULT:
	//	return map::multi::ispassable(id);
	//case res::OBJNMUL2:
	//	return map::multi::ispassable2(id);
	//case res::OBJNSWMP:
	//	return map::swamp::ispassable(id);
	case res::TREJNGL:
	case res::TREEVIL:
	case res::TRESNOW:
	case res::TREFIR:
	case res::TREFALL:
	case res::TREDECI:
		return map::tree::ispassable(id);
	//case res::MTNSNOW:
	//case res::MTNSWMP:
	//case res::MTNLAVA:
	//case res::MTNDSRT:
	//case res::MTNMULT:
	//	return map::mountain::ispassable(id);
	//case res::MTNGRAS:
	//	return map::mountain::ispassable3(id);
	//case res::MTNDIRT:
	//	return map::mountain::ispassable2(id);
	//case res::OBJNTOWN:
	//	return map::towns::ispassable(id);
	default:
		return 0;
	}
}

int map::action(int index)
{
	if(index==-1)
		return 0;
	//for(auto& e : map::show::objects[index])
	//{
	//	if(!e[0])
	//		break;
	//	int r = action2(e[0], e[1]);
	//	if(r)
	//		return r;
	//}
	return 0;
}

bool ispassable2(int object, int id)
{
	switch(res::map(object))
	{
	case res::OBJNGRAS:
		return map::grass::ispassable(id);
	case res::OBJNDIRT:
		return map::dirt::ispassable(id);
	case res::OBJNDSRT:
		return map::desert::ispassable(id);
	case res::OBJNCRCK:
		return map::crack::ispassable(id);
	case res::OBJNMULT:
		return map::multi::ispassable(id);
	case res::OBJNMUL2:
		return map::multi::ispassable2(id);
	case res::OBJNSWMP:
		return map::swamp::ispassable(id);
	case res::TREJNGL:
	case res::TREEVIL:
	case res::TRESNOW:
	case res::TREFIR:
	case res::TREFALL:
	case res::TREDECI:
		return map::tree::ispassable(id);
	case res::MTNSNOW:
	case res::MTNSWMP:
	case res::MTNLAVA:
	case res::MTNDSRT:
	case res::MTNMULT:
		return map::mountain::ispassable(id);
	case res::MTNGRAS:
		return map::mountain::ispassable3(id);
	case res::MTNDIRT:
		return map::mountain::ispassable2(id);
	case res::OBJNTOWN:
		return map::towns::ispassable(id);
	default:
		return true;
	}
}

bool map::ispassable(int index)
{
	if(index==-1)
		return false;
	//for(auto& e : map::show::objects[index])
	//{
	//	if(!e[0])
	//		break;
	//	if(!ispassable2(e[0], e[1]))
	//		return false;
	//}
	return true;
}