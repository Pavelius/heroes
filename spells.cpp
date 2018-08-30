#include "main.h"

static struct spell {
	const char*     name;
	int             spellpoints;
	int             level;
	int             portrait;
	int             value;
	const char*     description;
} objects[LastSpell - FirstSpell + 1] = {{"�������� ���", 9, 3, 8, 1, "���������� �������� ��� ���� ������� � ��������� �������, ������� ���� ������� ������."},
{"�������� �����", 15, 4, 9, 15, "���������� ������ ��������� ���� �������� ��� ������ ������ ����� ��������� ������ �����. ������� %1i ����� �� ������ ������� ���� �����."},
{"������", 7, 2, 4, 25, "������ ������������� ������� �������� ��������� �������� ������ %1i ����� �� ������� ���� �����."},
{"���� ������", 15, 4, 5, 40, "������������� ������ �������� ��������� ��������, ����� �������� �������� �������� � ��������� �����, ����� ��������� �������� �������� � ��������� ����� � �.�. �������� - ��� ���������� ����� �������� ������������� �������."},
{"��������", 9, 3, 10, 0, "���������� ��������, ������� �� ������� � ����� �������� ������� �� ���� �����."},
{"�������", 6, 1, 6, 5, "������� ��� ���������� ���������� � ������ ������ ����� � ��������������� ������ �� %1i ����� �� ������� ���� �����."},
{"����� �������", 15, 3, 2, 5, "������� ��� ���������� ���������� � ��������������� ������ �� %1i ����� �� ������� ���� ����� � ���� ����� �������."},
{"���������", 12, 4, 13, 50, "�������� ������������ ��� ������� �������� �� ����� ���."},
{"�������� ���������", 15, 5, 12, 50, "�������� �������� ������������ ��� ������� ��������."},
{"���������", 3, 1, 14, 2, "���������� �������� �������� �� %1i."},
{"����� ���������", 10, 3, 14, 2, "���������� �������� ���� ����� �������� �� %1i."},
{"����������", 3, 1, 1, 0, "��������� ���� �� �������� �� ��������."},
{"����� ����������", 15, 4, 1, 0, "��������� ���� ������ �� �������� �� ��������."},
//
{"����������", 6, 2, 21, 0, "���, ���������� ����� ���������� ������ ����������� �� ������."},
{"�������������", 3, 1, 7, 0, "��������� ����� ������� ������������ �����������."},
{"����� �������������", 12, 3, 7, 0, "��� ���� ������� ����� �������� ������������ ����."},
{"�������� ����", 3, 1, 31, 3, "�������� ������ ���������� �������� �� %1i."},
{"�������� ����", 6, 2, 30, 5, "�������� ������ ���������� �������� �� % 1i. ��� ���������� ������ �������� ����."},
{"���������", 3, 1, 3, 0, "��������� ��������� ����� ������� ����������� �����������."},
{"����� ���������", 12, 3, 3, 0, "��� ��������� ������ ������� ����������� �����������."},
{"������ �����", 9, 3, 22, 10, "������� ����������� ���� ��������� � �����. ���� ����� %1i �� ������ ������� ���� �����."},
{"������ ����", 12, 4, 23, 20, "������� ����������� ���� ��������� � �����. ���� ����� %1i �� ������ ������� ���� �����. ��� ���������� ������ ������� �����."},
{"���������", 7, 3, 17, 0, "�������� ��������� �������� �� ���� ����������."},
{"�������� �����", 5, 1, 18, 0, "������� ��� ���������� � ��������� ����."},
{"����� �����������", 12, 3, 18, 0, "������� ��� ���������� �� ���� �������."},
{"��������� ������", 3, 1, 38, 10, "��������� ������ ������� ���� ���������� ��������. ���� ����� %1i �� ������ ������� ���� �����."},
{"���������", 12, 3, 19, 0, "���������� �������� ��������� ��� ������."},
{"���������", 20, 5, 16, 50, "������� ����� �������� ��� ���� ����� ������ �������� ����������� ��� �������������. ���� ����� %1i �� ������ ������� ���� �����."},
{"���� ���������", 15, 4, 11, 25, "�������� ������ ������������ �� ���� �����, ������ %1i ����� �� ������ ������� ���� ����� ������� ��������."},
{"����������� �����", 15, 4, 24, 25, "����������� ����� ������������ � ���� � ������� ���� �������� ��������� %1i ����� �� ������ ������� ���� �����."},
{"�������", 9, 3, 20, 0, "��������� �������� ������������ � ������ ����������� ��������� � �������� �� �����."},
{"������", 15, 5, 37, 50, "��������� ���� �� ���� ����� ��� ��������� ��� ��� ��������, ���� ���������� ��� ����� ����� ��� %1i �� ������� ���� �����."},
{"��� ������", 6, 2, 36, 20, "���������� ��������� ����� � ���������� ������. ������� %1i ����� �� ������ ������� ���� �����."},
{"������ ������", 9, 3, 35, 10, "���������� ��������� ����� �� ���� ������� ������ ������, �� �� ������� �����. ������� %1i ����� �� ������ ������� ���� �����."},
{"��� ����������", 7, 2, 34, 3, "������� ������ ���������� ������ �� %1i."},
{"����� ������", 6, 2, 28, 5, "������� %1i �� ������ ������� ���� ����� ����� ���� ����� (�� ���������) �� ���� ���."},
{"����� ������", 10, 3, 29, 10, "������� %1i �� ������ ������� ���� ����� ����� ���� ����� (�� ���������) �� ���� ���. ��� ���������� ������ ���������� ����� ������."},
{"������ ��������", 6, 2, 32, 6, "�������� �� %1i ����� ���������� ������ ������ ��������."},
{"����� �����", 3, 1, 27, 3, "����������� ����� ������ �� %1i."},
{"������� �������", 10, 3, 25, 50, "�������� ������ � ��� ���������."},
{"�������", 25, 5, 26, 0, "Creates an illusionary unit that duplicates one of your existing units.  This illusionary unit does the same damages as the original, but will vanish if it takes any damage."},
{"���", 3, 1, 15, 2, "������� �������� ����� �� ������������� ����, ������������ � ��������� �����."},
{"����� ���", 7, 4, 15, 0, "Halves damage received from ranged attacks for all of your units."},
{"����� ���������� �����", 30, 5, 56, 3, "Summons Earth Elementals to fight for your army."},
{"����� ���������� �������", 30, 5, 57, 3, "Summons Air Elementals to fight for your army."},
{"����� ���������� ����", 30, 5, 58, 3, "Summons Fire Elementals to fight for your army."},
{"����� ���������� ����", 30, 5, 59, 3, "Summons Water Elementals to fight for your army."},
{"�������������", 15, 4, 33, 0, "Damages castle walls."},
// Adventure spells
{"�������� �����", 1, 1, 39, 0, "Causes all mines across the land to become visible."},
{"�������� �������", 1, 2, 40, 0, "Causes all res::tokens across the land to become visible."},
{"�������� ���������", 2, 2, 41, 0, "Causes all artifacts across the land to become visible."},
{"�������� ������", 2, 3, 42, 0, "Causes all towns and castles across the land to become visible."},
{"�������� ������", 2, 3, 43, 0, "Causes all Heroes across the land to become visible."},
{"�������� ���", 3, 4, 44, 0, "Causes the entire land to become visible."},
{"�������� �����", 3, 4, 45, 0, "Allows the caster to view detailed information on enemy Heroes."},
{"������� �������", 5, 2, 46, 0, "Summons the nearest unoccupied, friendly boat to an adjacent shore location.  A friendly boat is one which you just built or were the most recent player to occupy."},
{"������", 10, 5, 47, 0, "Allows the caster to magically transport to a nearby location."},
{"��������� �����", 10, 4, 48, 0, "Returns the caster to any town or castle currently owned."},
{"��������� ������", 20, 5, 49, 0, "Returns the hero to the town or castle of choice, provided it is controlled by you."},
{"�������", 6, 2, 50, 3, "Visions predicts the likely outcome of an encounter with a neutral army camp."},
{"����������", 8, 2, 51, 4, "Haunts a mine you control with Ghosts.  This mine stops producing res::tokens.  (If I can't keep it, nobody will!)"},
{"������ �����", 15, 4, 52, 4, "Sets Earth Elementals to guard a mine against enemy armies."},
{"������ �������", 15, 4, 53, 4, "Sets Air Elementals to guard a mine against enemy armies."},
{"������ ������", 15, 4, 54, 4, "Sets Fire Elementals to guard a mine against enemy armies."},
{"������ ����", 15, 4, 55, 4, "Sets Water Elementals to guard a mine against enemy armies."},
{"����������", 0, 0, 0, 0, "Stone spell from Medusa."},
};
static bsmeta::field fields[] = {
	BSREQ(spell, name, Name, Text),
	BSREQ(spell, description, Text, Text),
	BSREQ(spell, spellpoints, SpellPoints, Number),
	BSREQ(spell, level, Level, Number),
	BSREQ(spell, portrait, Portrait, Number),
	BSREQ(spell, value, Value, Number),
{0}
};
BSMETA(spell, "Spells", "����������", FirstSpell);

bool game::ishostile(int rec) {
	switch(rec) {
	case SpellCurse:
	case SpellBlind:
	case SpellSlow:
	case SpellBerserker:
	case SpellParalyze:
	case SpellDisruptingRay:
	case SpellMagicArrow:
	case SpellLightingBolt:
	case SpellColdRay:
	case SpellColdRing:
	case SpellChainLighting:
	case SpellFireBall:
	case SpellFireBlast:
	case SpellMeteorShower:
	case SpellMassCurse:
	case SpellMassSlow:
		return true;
	default:
		return false;
	}
}

bool game::isfriendly(int rec) {
	switch(rec) {
	case SpellStoneSkin:
	case SpellSteelSkin:
	case SpellBless:
	case SpellBloodLust:
	case SpellDragonSlayer:
	case SpellHaste:
	case SpellShield:
	case SpellMassHaste:
	case SpellMassBless:
	case SpellMassShield:
	case SpellMassCure:
		return true;
	default:
		return false;
	}
}

int game::gettarget(int rec) {
	switch(rec) {
	case SpellLightingBolt:
	case SpellChainLighting:
	case SpellTeleport:
	case SpellCure:
	case SpellResurrect:
	case SpellRessurectTrue:
	case SpellHaste:
	case SpellSlow:
	case SpellBlind:
	case SpellBless:
	case SpellStoneSkin:
	case SpellSteelSkin:
	case SpellCurse:
	case SpellAntimagic:
	case SpellDispel:
	case SpellMagicArrow:
	case SpellBerserker:
	case SpellParalyze:
	case SpellHypnotize:
	case SpellColdRay:
	case SpellDisruptingRay:
	case SpellDragonSlayer:
	case SpellBloodLust:
	case SpellAnimateDead:
	case SpellMirrorImage:
	case SpellShield:
		return TargetUnit;
	default:
		return 0;
	}
}

int game::getframe(int rec) {
	if(rec >= FirstSpell && rec <= LastSpell) {
		switch(rec) {
		case SpellBloodLust: return 9;
		case SpellBless: return 3;
		case SpellHaste: return 0;
		case SpellShield: return 10;
		case SpellStoneSkin: return 13;
		case SpellDragonSlayer: return 8;
		case SpellSteelSkin: return 14;
		case SpellAntimagic: return 12;
		case SpellCurse: return 4;
		case SpellSlow: return 1;
		case SpellBerserker: return 5;
		case SpellHypnotize: return 7;
		case SpellBlind: return 2;
		case SpellParalyze: return 6;
		case SpellStone: return 11;
		default: return 100;
		}
	}
	return 0;
}

bool game::iscombat(int rec) {
	return rec >= FirstSpell && rec <= SpellEarthquake;
}

//static int object_get(int rec, int id)
//{
//    switch(id)
//    {
//    case SingleVersion:
//        switch(rec)
//        {
//        case SpellMassHaste:
//            return SpellHaste;
//        case SpellMassBless:
//            return SpellBless;
//        case SpellMassDispel:
//            return SpellDispel;
//        case SpellMassShield:
//            return SpellShield;
//        case SpellMassCurse:
//            return SpellCurse;
//        case SpellMassCure:
//            return SpellCure;
//        case SpellMassSlow:
//            return SpellSlow;
//        default:
//            return rec;
//        }
//        break;
//    case MassEffect:
//        switch(rec)
//        {
//        case SpellMassHaste:
//        case SpellMassBless:
//        case SpellMassDispel:
//        case SpellEarthquake:
//        case SpellMassShield:
//        case SpellMassCurse:
//		case SpellMassCure:
//        case SpellMassSlow:
//        case SpellHolyWord:
//        case SpellHolyShout:
//        case SpellArmagedon:
//        case SpellElementalStorm:
//        case SpellDeathRipple:
//        case SpellDeathWave:
//            return 1;
//        default:
//            return 0;
//        }
//        break;