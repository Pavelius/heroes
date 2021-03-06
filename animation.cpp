#include "main.h"

struct combat_monsters_info {
	res::tokens		icn;
	short int		idle[2];
	short int		move[2];
	short int		fly1[2];
	short int		fly2[2];
	short int		fly3[2];
	short int		shot0[2];
	short int		shot1[2];
	short int		shot2[2];
	short int		shot3[2];
	short int		attk0[2];
	short int		attk1[2];
	short int		attk2[2];
	short int		attk3[2];
	short int		wcne[2];
	short int		kill[2];
	res::tokens		wav_attk;
	res::tokens		wav_kill;
	res::tokens		wav_move;
	res::tokens		wav_wnce;
};

combat_monsters_info combat_monsters[] = {{res::PEASANT, {1, 4}, {5, 8}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{0, 0}, {0, 0}, {0, 0}, {0, 0}, /*Attack*/{0, 0}, {16, 6}, {22, 6}, {28, 6}, {13, 3}, {34, 4}, res::WavPSNTATTK, res::WavPSNTKILL, res::WavPSNTMOVE, res::WavPSNTWNCE},
{res::ARCHER, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 3}, {39, 3}, {42, 3}, {13, 3}, {45, 6}, res::WavARCHATTK, res::WavARCHKILL, res::WavARCHMOVE, res::WavARCHWNCE},
{res::ARCHER2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 3}, {39, 3}, {42, 3}, {13, 3}, {45, 6}, res::WavARCHATTK, res::WavARCHKILL, res::WavARCHMOVE, res::WavARCHWNCE},
{res::PIKEMAN, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {17, 3}, {20, 4}, {24, 6}, {13, 4}, {30, 6}, res::WavPIKEATTK, res::WavPIKEKILL, res::WavPIKEMOVE, res::WavPIKEWNCE},
{res::PIKEMAN2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {17, 3}, {20, 4}, {24, 6}, {13, 4}, {30, 6}, res::WavPIKEATTK, res::WavPIKEKILL, res::WavPIKEMOVE, res::WavPIKEWNCE},
{res::SWORDSMN, {39, 6}, {2, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {12, 2}, {19, 5}, {14, 5}, {24, 5}, {36, 3}, {29, 7}, res::WavSWDMATTK, res::WavSWDMKILL, res::WavSWDMMOVE, res::WavSWDMWNCE},
{res::SWORDSM2, {39, 6}, {2, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {12, 2}, {19, 5}, {14, 5}, {24, 5}, {36, 3}, {29, 7}, res::WavSWDMATTK, res::WavSWDMKILL, res::WavSWDMMOVE, res::WavSWDMWNCE},
{res::CAVALRYR, {19, 4}, {1, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {8, 1}, {12, 3}, {9, 3}, {15, 2}, {28, 2}, {23, 5}, res::WavCAVLATTK, res::WavCAVLKILL, res::WavCAVLMOVE, res::WavCAVLWNCE},
{res::CAVALRYB, {19, 4}, {1, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {8, 1}, {12, 3}, {9, 3}, {15, 2}, {28, 2}, {23, 5}, res::WavCAVLATTK, res::WavCAVLKILL, res::WavCAVLMOVE, res::WavCAVLWNCE},
{res::PALADIN, {1, 11}, {12, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 2}, {22, 3}, {25, 3}, {28, 3}, {31, 2}, {34, 5}, res::WavPLDNATTK, res::WavPLDNKILL, res::WavPLDNMOVE, res::WavPLDNWNCE},
{res::PALADIN2, {1, 11}, {12, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 2}, {22, 3}, {25, 3}, {28, 3}, {31, 2}, {34, 5}, res::WavPLDNATTK, res::WavPLDNKILL, res::WavPLDNMOVE, res::WavPLDNWNCE},
// Barbarian
{res::GOBLIN, {33, 7}, {1, 9}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{0, 0}, {0, 0}, {0, 0}, {0, 0}, /*Attack*/{10, 3}, {17, 4}, {13, 4}, {21, 4}, {25, 4}, {29, 4}, res::WavGBLNATTK, res::WavGBLNKILL, res::WavGBLNMOVE, res::WavGBLNWNCE},
{res::ORC, {1, 4}, {5, 8}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{16, 13}, {0, 0}, {0, 0}, {0, 0}, /*Attack*/{28, 2}, {30, 3}, {33, 4}, {37, 3}, {13, 3}, {40, 4}, res::WavORC_ATTK, res::WavORC_KILL, res::WavORC_MOVE, res::WavORC_WNCE},
{res::ORC2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {16, 13}, {0, 0}, {0, 0}, {0, 0}, {28, 2}, {30, 3}, {33, 4}, {37, 3}, {13, 3}, {40, 4}, res::WavORC_ATTK, res::WavORC_KILL, res::WavORC_MOVE, res::WavORC_WNCE},
{res::WOLF, {20, 6}, {7, 6}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 3}, {2, 5}, {16, 4}, {26, 3}, {28, 5}, res::WavWOLFATTK, res::WavWOLFKILL, res::WavWOLFMOVE, res::WavWOLFWNCE},
{res::OGRE, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 6}, {22, 7}, {29, 8}, {13, 3}, {37, 4}, res::WavOGREATTK, res::WavOGREKILL, res::WavOGREMOVE, res::WavOGREWNCE},
{res::OGRE2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 6}, {22, 7}, {29, 8}, {13, 3}, {37, 4}, res::WavOGREATTK, res::WavOGREKILL, res::WavOGREMOVE, res::WavOGREWNCE},
{res::TROLL, {16, 7}, {1, 15}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{23, 5}, {0, 0}, {0, 0}, {0, 0}, /*Attack*/{0, 0}, {48, 6}, {23, 6}, {29, 5}, {54, 3}, {57, 9}, res::WavTRLLATTK, res::WavTRLLKILL, res::WavTRLLMOVE, res::WavTRLLWNCE},
{res::TROLL2, {16, 7}, {1, 15}, {0, 0}, {0, 0}, {0, 0}, {23, 5}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {48, 6}, {23, 6}, {29, 5}, {54, 3}, {57, 9}, res::WavTRLLATTK, res::WavTRLLKILL, res::WavTRLLMOVE, res::WavTRLLWNCE},
{res::CYCLOPS, {30, 9}, {1, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 2}, {11, 2}, {21, 2}, {0, 0}, {14, 3}, {8, 3}, {18, 3}, {23, 2}, {25, 5}, res::WavCYCLATTK, res::WavCYCLKILL, res::WavCYCLMOVE, res::WavCYCLWNCE},
// Sorcerer
{res::SPRITE, {16, 9}, {0, 0}, {1, 3}, {4, 4}, {7, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {30, 6}, {25, 5}, {36, 5}, {9, 2}, {11, 5}, res::WavSPRTATTK, res::WavSPRTKILL, res::WavSPRTMOVE, res::WavSPRTWNCE},
{res::DWARF, {44, 5}, {1, 9}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 7}, {10, 10}, {27, 9}, {36, 8}, {49, 7}, res::WavDWRFATTK, res::WavDWRFKILL, res::WavDWRFMOVE, res::WavDWRFWNCE},
{res::DWARF2, {44, 5}, {1, 9}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 7}, {10, 10}, {27, 9}, {36, 8}, {49, 7}, res::WavDWRFATTK, res::WavDWRFKILL, res::WavDWRFMOVE, res::WavDWRFWNCE},
{res::ELF, {42, 6}, {1, 12}, {0, 0}, {0, 0}, {0, 0}, {13, 10}, {0, 0}, {0, 0}, {0, 0}, {23, 3}, {26, 3}, {29, 3}, {32, 4}, {36, 2}, {38, 4}, res::WavELF_ATTK, res::WavELF_KILL, res::WavELF_MOVE, res::WavELF_WNCE},
{res::ELF2, {42, 6}, {1, 12}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{13, 10}, {0, 0}, {0, 0}, {0, 0}, {23, 3}, {26, 3}, {29, 3}, {32, 4}, {36, 2}, {38, 4}, res::WavELF_ATTK, res::WavELF_KILL, res::WavELF_MOVE, res::WavELF_WNCE},
{res::DRUID, {46, 4}, {3, 11}, {0, 0}, {0, 0}, {0, 0}, {14, 8}, {35, 4}, {22, 4}, {39, 5}, {14, 8}, {35, 4}, {22, 4}, {39, 4}, {44, 2}, {26, 9}, res::WavDRUIATTK, res::WavDRUIKILL, res::WavDRUIMOVE, res::WavDRUIWNCE},
{res::DRUID2, {46, 4}, {3, 11}, {0, 0}, {0, 0}, {0, 0}, {14, 8}, {35, 4}, {22, 4}, {39, 5}, {14, 8}, {35, 4}, {22, 4}, {39, 4}, {44, 2}, {26, 9}, res::WavDRUIATTK, res::WavDRUIKILL, res::WavDRUIMOVE, res::WavDRUIWNCE},
{res::UNICORN, {1, 9}, {10, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {18, 9}, {27, 7}, {34, 7}, {41, 4}, {45, 7}, res::WavUNICATTK, res::WavUNICKILL, res::WavUNICMOVE, res::WavUNICWNCE},
{res::PHOENIX, {30, 5}, {0, 0}, {1, 3}, {4, 6}, {0, 0}, {0, 0}, {25, 3}, {23, 2}, {28, 2}, {0, 0}, {13, 4}, {10, 4}, {17, 4}, {35, 2}, {37, 11}, res::WavPHOEATTK, res::WavPHOEKILL, res::WavPHOEMOVE, res::WavPHOEWNCE},
// Warlock
{res::CENTAUR, {65, 6}, {2, 6}, /*Fly*/{0, 0}, {0, 0}, {0, 0}, /*Shoot*/{18, 4}, {27, 3}, {22, 5}, {30, 3}, {33, 1}, {38, 4}, {34, 4}, {42, 4}, {46, 2}, {48, 7}, res::WavCNTRATTK, res::WavCNTRKILL, res::WavCNTRMOVE, res::WavCNTRWNCE},
{res::GARGOYLE, {1, 4}, {0, 0}, /*Fly*/{5, 2}, {7, 4}, {11, 1}, /*Shoot*/{0, 0}, {0, 0}, {0, 0}, {0, 0}, {14, 1}, {15, 4}, {19, 4}, {23, 4}, {12, 2}, {27, 5}, res::WavGARGATTK, res::WavGARGKILL, res::WavGARGMOVE, res::WavGARGWNCE},
{res::GRIFFIN, {16, 9}, {0, 0}, {1, 3}, {3, 3}, {5, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {7, 1}, {8, 2}, {14, 2}, {11, 3}, {9, 2}, {25, 9}, res::WavGRIFATTK, res::WavGRIFKILL, res::WavGRIFMOVE, res::WavGRIFWNCE},
{res::MINOTAUR, {1, 5}, {6, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 3}, {20, 3}, {16, 4}, {23, 4}, {27, 2}, {29, 6}, res::WavMINOATTK, res::WavMINOKILL, res::WavMINOMOVE, res::WavMINOWNCE},
{res::MINOTAU2, {1, 5}, {6, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 3}, {20, 3}, {16, 4}, {23, 4}, {27, 2}, {29, 6}, res::WavMINOATTK, res::WavMINOKILL, res::WavMINOMOVE, res::WavMINOWNCE},
{res::HYDRA, {28, 15}, {1, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {9, 8}, {9, 8}, {9, 8}, {17, 5}, {21, 7}, res::WavHYDRATTK, res::WavHYDRKILL, res::WavHYDRMOVE, res::WavHYDRWNCE},
{res::DRAGGREE, {41, 13}, {0, 0}, {1, 4}, {5, 3}, {8, 5}, {13, 1}, {17, 3}, {23, 3}, {30, 2}, {13, 1}, {14, 3}, {20, 3}, {26, 4}, {32, 3}, {35, 6}, res::WavDRGNATTK, res::WavDRGNKILL, res::WavDRGNMOVE, res::WavDRGNWNCE},
{res::DRAGRED, {41, 13}, {0, 0}, {1, 4}, {5, 3}, {8, 5}, {13, 1}, {17, 3}, {23, 3}, {30, 2}, {13, 1}, {14, 3}, {20, 3}, {26, 4}, {32, 3}, {35, 6}, res::WavDRGNATTK, res::WavDRGNKILL, res::WavDRGNMOVE, res::WavDRGNWNCE},
{res::DRAGBLAK, {41, 13}, {0, 0}, {1, 4}, {5, 3}, {8, 5}, {13, 1}, {17, 3}, {23, 3}, {30, 2}, {13, 1}, {14, 3}, {20, 3}, {26, 4}, {32, 3}, {35, 6}, res::WavDRGNATTK, res::WavDRGNKILL, res::WavDRGNMOVE, res::WavDRGNWNCE},
// Wizard
{res::HALFLING, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {17, 5}, {22, 2}, {26, 1}, {27, 2}, {29, 1}, {30, 2}, {32, 3}, {35, 2}, {13, 4}, {37, 4}, res::WavHALFATTK, res::WavHALFKILL, res::WavHALFMOVE, res::WavHALFWNCE},
{res::BOAR, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {15, 1}, {16, 3}, {19, 3}, {22, 2}, {13, 2}, {24, 5}, res::WavBOARATTK, res::WavBOARKILL, res::WavBOARMOVE, res::WavBOARWNCE},
{res::GOLEM, {34, 6}, {1, 12}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {22, 4}, {13, 5}, {18, 4}, {32, 4}, {26, 6}, res::WavGOLMATTK, res::WavGOLMKILL, res::WavGOLMMOVE, res::WavGOLMWNCE},
{res::GOLEM2, {34, 6}, {1, 12}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {22, 4}, {13, 5}, {18, 4}, {32, 4}, {26, 6}, res::WavGOLMATTK, res::WavGOLMKILL, res::WavGOLMMOVE, res::WavGOLMWNCE},
{res::ROC, {18, 7}, {0, 0}, {1, 3}, {4, 3}, {7, 1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {14, 3}, {8, 3}, {11, 3}, {25, 3}, {25, 9}, res::WavROC_ATTK, res::WavROC_KILL, res::WavROC_MOVE, res::WavROC_WNCE},
{res::MAGE1, {1, 7}, {43, 8}, {0, 0}, {0, 0}, {0, 0}, {18, 1}, {21, 2}, {19, 2}, {23, 4}, {27, 1}, {34, 3}, {28, 6}, {37, 6}, {8, 3}, {11, 7}, res::WavMAGEATTK, res::WavMAGEKILL, res::WavMAGEMOVE, res::WavMAGEWNCE},
{res::MAGE2, {1, 7}, {43, 8}, {0, 0}, {0, 0}, {0, 0}, {18, 1}, {21, 2}, {19, 2}, {23, 4}, {27, 1}, {34, 3}, {28, 6}, {37, 6}, {8, 3}, {11, 7}, res::WavMAGEATTK, res::WavMAGEKILL, res::WavMAGEMOVE, res::WavMAGEWNCE},
{res::TITANBLU, {1, 6}, {7, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {19, 5}, {14, 5}, {24, 5}, {29, 2}, {31, 7}, res::WavTITNATTK, res::WavTITNKILL, res::WavTITNMOVE, res::WavTITNWNCE},
{res::TITANBLA, {1, 6}, {7, 7}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {45, 5}, {38, 5}, {50, 5}, {0, 0}, {19, 5}, {14, 5}, {24, 5}, {29, 2}, {31, 7}, res::WavTITNATTK, res::WavTITNKILL, res::WavTITNMOVE, res::WavTITNWNCE},
// Necromancer
{res::SKELETON, {35, 4}, {3, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {20, 4}, {12, 4}, {16, 4}, {24, 3}, {27, 8}, res::WavSKELATTK, res::WavSKELKILL, res::WavSKELMOVE, res::WavSKELWNCE},
{res::ZOMBIE, {14, 12}, {1, 13}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {26, 5}, {31, 5}, {36, 5}, {40, 3}, {47, 7}, res::WavZOMBATTK, res::WavZOMBKILL, res::WavZOMBMOVE, res::WavZOMBWNCE},
{res::ZOMBIE2, {14, 12}, {1, 13}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {26, 5}, {31, 5}, {36, 5}, {40, 3}, {47, 7}, res::WavZOMBATTK, res::WavZOMBKILL, res::WavZOMBMOVE, res::WavZOMBWNCE},
{res::MUMMYW, {1, 4}, {5, 12}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {21, 3}, {24, 3}, {27, 3}, {17, 4}, {30, 10}, res::WavMUMYATTK, res::WavMUMYKILL, res::WavMUMYMOVE, res::WavMUMYWNCE},
{res::MUMMY2, {1, 4}, {5, 12}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {21, 3}, {24, 3}, {27, 3}, {17, 4}, {30, 10}, res::WavMUMYATTK, res::WavMUMYKILL, res::WavMUMYMOVE, res::WavMUMYWNCE},
{res::VAMPIRE, {1, 4}, {0, 0}, {5, 4}, {9, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {15, 4}, {19, 4}, {23, 5}, {13, 3}, {28, 8}, res::WavVAMPATTK, res::WavVAMPKILL, res::WavVAMPMOVE, res::WavVAMPWNCE},
{res::VAMPIRE2, {1, 4}, {0, 0}, {5, 4}, {9, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {15, 4}, {19, 4}, {23, 5}, {13, 3}, {28, 8}, res::WavVAMPATTK, res::WavVAMPKILL, res::WavVAMPMOVE, res::WavVAMPWNCE},
{res::LICH, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 5}, {18, 5}, {23, 5}, {0, 0}, {13, 5}, {18, 5}, {23, 5}, {28, 3}, {31, 7}, res::WavLICHATTK, res::WavLICHKILL, res::WavLICHMOVE, res::WavLICHWNCE},
{res::LICH2, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {13, 5}, {18, 8}, {23, 5}, {0, 0}, {13, 5}, {18, 5}, {23, 5}, {28, 3}, {31, 7}, res::WavLICHATTK, res::WavLICHKILL, res::WavLICHMOVE, res::WavLICHWNCE},
{res::DRAGBONE, {22, 7}, {0, 0}, {1, 4}, {5, 3}, {8, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {47, 5}, {12, 4}, {16, 4}, {20, 2}, {41, 6}, res::WavBONEATTK, res::WavBONEKILL, res::WavBONEMOVE, res::WavBONEWNCE},
// Neutral
{res::ROGUE, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 8}, {24, 10}, {34, 9}, {13, 3}, {43, 7}, res::WavROGUATTK, res::WavROGUKILL, res::WavROGUMOVE, res::WavROGUWNCE},
{res::NOMAD, {1, 9}, {10, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {18, 2}, {26, 5}, {20, 6}, {31, 4}, {35, 2}, {37, 5}, res::WavNMADATTK, res::WavNMADKILL, res::WavNMADMOVE, res::WavNMADWNCE},
{res::GHOST, {1, 3}, {0, 0}, {0, 0}, {4, 5}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {10, 3}, {13, 3}, {16, 4}, {20, 2}, {20, 8}, res::WavGHSTATTK, res::WavGHSTKILL, res::WavGHSTMOVE, res::WavGHSTWNCE},
{res::GENIE, {1, 9}, {0, 0}, {10, 1}, {11, 4}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {26, 5}, {16, 5}, {21, 5}, {31, 2}, {33, 7}, res::WavGENIATTK, res::WavGENIKILL, res::WavGENIMOVE, res::WavGENIWNCE},
{res::MEDUSA, {17, 7}, {1, 16}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {65, 10}, {41, 13}, {54, 11}, {36, 2}, {24, 12}, res::WavMEDSATTK, res::WavMEDSKILL, res::WavMEDSMOVE, res::WavMEDSWNCE},
{res::EELEM, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 5}, {25, 6}, {13, 3}, {31, 11}, res::WavEELMATTK, res::WavEELMKILL, res::WavEELMMOVE, res::WavEELMWNCE},
{res::AELEM, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 5}, {25, 6}, {13, 3}, {31, 11}, res::WavAELMATTK, res::WavAELMKILL, res::WavAELMMOVE, res::WavAELMWNCE},
{res::FELEM, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 5}, {25, 6}, {13, 3}, {31, 11}, res::WavFELMATTK, res::WavFELMKILL, res::WavFELMMOVE, res::WavFELMWNCE},
{res::WELEM, {1, 4}, {5, 8}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {16, 4}, {20, 5}, {25, 6}, {13, 3}, {31, 11}, res::WavWELMATTK, res::WavWELMKILL, res::WavWELMMOVE, res::WavWELMWNCE},
// Random
{res::Empthy, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, res::Empthy, res::Empthy, res::Empthy, res::Empthy},
{res::Empthy, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, res::Empthy, res::Empthy, res::Empthy, res::Empthy},
{res::Empthy, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, res::Empthy, res::Empthy, res::Empthy, res::Empthy},
{res::Empthy, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, res::Empthy, res::Empthy, res::Empthy, res::Empthy},
{res::Empthy, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, res::Empthy, res::Empthy, res::Empthy, res::Empthy},
};

const unsigned animation_rate = 1000 / 8;

animation::animation() :icn(res::Empthy), frame(0), start(0), count(0), wait(0), flags(0) {
	rec = 0;
	pos.x = 0;
	pos.y = 0;
}

animation::animation(res::tokens icn, int start, int count) :start(start), wait(0), flags(0) {
	rec = 0;
	if(count == -1)
		count = res::getcount(icn);
	pos.x = 0;
	pos.y = 0;
	this->icn = icn;
	this->frame = start;
	this->count = count;
}

animation::animation(int rec, int action) :icn(res::Empthy), wait(0), flags(0) {
	this->rec = 0;
	pos.x = 0;
	pos.y = 0;
	set(rec, action, 0);
}

void animation::clear() {
	rec = 0;
	pos.clear();
	frame = start = count = wait = 0;;
	flags = 0;
	icn = res::Empthy;
}

void animation::painting(point pt, unsigned paint_flags) const {
	draw::image(pt.x + pos.x, pt.y + pos.y, icn, frame, flags);
}

void animation::set(int id, int value, int param) {
	wait = 0;
	if(id >= Peasant && id <= MonsterRnd) {
		combat_monsters_info& e = combat_monsters[id - Peasant];
		icn = e.icn;
		switch(value) {
		case ActorWait:
			switch(id) {
			case Zombie:
			case MutantZombie:
				start = e.idle[0] + 7;
				count = 5;
				break;
			default:
				start = e.idle[0];
				count = 1;
				break;
			}
			break;
		case ActorWarn:
			start = e.idle[0];
			count = e.idle[1];
			break;
		case Move:
			start = e.move[0];
			count = e.move[1];
			break;
		case Damage:
			start = e.wcne[0];
			count = e.wcne[1];
			break;
		case Killed:
			start = e.kill[0];
			count = e.kill[1];
			break;
		case Attack:
			switch(param) {
			case 0:
				start = e.attk0[0];
				count = e.attk0[1];
				break;
			case 1:
				start = e.attk1[0];
				count = e.attk1[1];
				break;
			case 2:
				start = e.attk2[0];
				count = e.attk2[1];
				break;
			default:
				start = e.attk3[0];
				count = e.attk3[1];
				break;
			}
			break;
		case Shoot:
			switch(param) {
			case 0:
				start = e.shot0[0];
				count = e.shot0[1];
				break;
			case 1:
				start = e.shot1[0];
				count = e.shot1[1];
				break;
			case 2:
				start = e.shot2[0];
				count = e.shot2[1];
				break;
			default:
				start = e.shot3[0];
				count = e.shot3[1];
				break;
			}
			break;
		case Fly:
			switch(param) {
			case 0:
				start = e.fly1[0];
				count = e.fly1[1];
				break;
			case 1:
				start = e.fly2[0];
				count = e.fly2[1];
				break;
			case 2:
				start = e.fly3[0];
				count = e.fly3[1];
				break;
			}
			break;
		default:
			break;
		}
	} else if(id >= Barbarian && id <= Wizard) {
		switch(id) {
		case Barbarian:
			icn = res::CMBTHROB;
			switch(value) {
			case ActorWait:
				start = 15;
				count = 1;
				break;
			case ActorWarn:
				start = xrand(15, 18);
				count = 1;
				break;
			case ActorPalmFace:
				start = 1;
				count = 5;
				break;
			case CastSpell:
				start = 6;
				count = 9;
				break;
			default:
				break;
			}
			break;
		case Knight:
			icn = res::CMBTHROK;
			switch(value) {
			case ActorWait:
				start = 15;
				count = 5;
				break;
			case ActorPalmFace:
				start = 1;
				count = 5;
				break;
			case CastSpell:
				start = 12;
				count = 2;
				break;
			default:
				break;
			}
			break;
		case Necromancer:
			icn = res::CMBTHRON;
			switch(value) {
			case ActorWait:
				start = 19;
				count = 1;
				break;
			case ActorWarn:
				start = xrand(17, 19);
				count = 1;
				break;
			case ActorPalmFace:
				start = 1;
				count = 5;
				break;
			case CastSpell:
				start = 6;
				count = 9;
				break;
			default:
				break;
			}
			break;
		case Sorcerer:
			icn = res::CMBTHROS;
			switch(value) {
			case ActorWait:
				start = 13;
				count = 4;
				break;
			case ActorPalmFace:
				start = 1;
				count = 5;
				break;
			case CastSpell:
				start = 6;
				count = 7;
				break;
			default:
				break;
			}
			break;
		case Warlock:
			icn = res::CMBTHROW;
			switch(value) {
			case ActorWait:
				start = 14;
				count = 3;
				break;
			case ActorPalmFace:
				start = 1;
				count = 5;
				break;
			case CastSpell:
				start = 6;
				count = 8;
				break;
			default:
				break;
			}
			break;
		case Wizard:
			icn = res::CMBTHROZ;
			switch(value) {
			case ActorWait:
				start = 16;
				count = 3;
				break;
			case ActorPalmFace:
				start = 1;
				count = 5;
				break;
			case CastSpell:
				start = 12;
				count = 7;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	} else if(id >= BarbarianCaptain && id <= WizardCaptain) {
		icn = res::CMBTCAPB;
		switch(id) {
		case BarbarianCaptain:
			icn = res::CMBTCAPB;
			break;
		case KnightCaptain:
			icn = res::CMBTCAPK;
			break;
		case NecromancerCaptain:
			icn = res::CMBTCAPN;
			break;
		case SorcererCaptain:
			icn = res::CMBTCAPS;
			break;
		case WarlockCaptain:
			icn = res::CMBTCAPW;
			break;
		case WizardCaptain:
			icn = res::CMBTCAPZ;
			break;
		default:
			break;
		}
		switch(value) {
		case ActorWait:
			start = 1;
			count = 1;
			break;
		case ActorPalmFace:
			start = 1;
			count = 1;
			break;
		case CastSpell:
			start = 3;
			count = 6;
			break;
		default:
			break;
		}
	} else if(id == CursorCombat) {
		// res::CMSECO
		// 0 - None
		// 1 - Move
		// 2 - Fly
		// 3 - Shoot
		// 4 - Hero (Helmet)
		// 5 - Info
		// 6 - Small cursor
		// 7 - Sword (to right up)
		// 8 - Sword (to right)
		// 9 - Sword (to right down)
		// 10 - Sword (to left down)
		// 11 - Sword (to left)
		// 12 - Sword (to left up)
		// 13 - Sword (to up)
		// 14 - Sword (to down)
		// 15 - Broken arrow
		switch(value) {
		case HexLeft:
			icn = res::CMSECO;
			start = 8;
			pos.x = -25;
			pos.y = -7;
			break;
		case HexRight:
			icn = res::CMSECO;
			start = 11;
			pos.x = -5;
			pos.y = -7;
			break;
		case HexLeftUp:
			icn = res::CMSECO;
			start = 9;
			pos.x = -20;
			pos.y = -20;
			break;
		case HexLeftDown:
			icn = res::CMSECO;
			start = 7;
			pos.x = -20;
			pos.y = -5;
			break;
		case HexRightUp:
			icn = res::CMSECO;
			start = 10;
			pos.x = -5;
			pos.y = -20;
			break;
		case HexRightDown:
			icn = res::CMSECO;
			start = 12;
			pos.x = -5;
			pos.y = -5;
			break;
		case Information:
			icn = res::CMSECO;
			start = 5;
			pos.x = -7;
			pos.y = -7;
			break;
		case Move:
			icn = res::CMSECO;
			start = 1;
			pos.x = -7;
			pos.y = -14;
			break;
		case Fly:
			icn = res::CMSECO;
			start = 2;
			pos.x = -7;
			pos.y = -14;
			break;
		case Shoot:
			icn = res::CMSECO;
			start = 3;
			pos.x = -7;
			pos.y = -7;
			break;
		case Empthy:
			icn = res::CMSECO;
			start = 0;
			pos.x = -7;
			pos.y = -7;
			break;
		case Hero:
			icn = res::CMSECO;
			start = 4;
			pos.x = -7;
			pos.y = -7;
			break;
		default:
			if(value >= FirstSpell && value <= LastSpell) {
				icn = res::SPELLS;
				start = bsget(value, Portrait);
				pos.x = -res::width(icn, start) / 2;
				pos.y = -res::height(icn, start) / 2;
			} else {
				icn = res::CMSECO;
				start = 6;
			}
			break;
		}
		count = 1;
	} else if(id == CursorAdventure) {
		// res::CMSECO
		// 0 - Arrow
		// 1 - Compas
		// 2 - Helmet
		// 3 - Caslte
		// 4 - Horse
		// 5 - Sword
		// 6 - Boat
		// 7 - Anchor
		// 8 - Change heroes
		// 9 - Jump horse
		// 10 - Horse 2
		// 11 - Sword 2
		// etc
		icn = res::ADVMCO;
		switch(value) {
		case Hero:
			start = 2;
			pos.x = -res::width(icn, start) / 2;
			pos.y = -res::height(icn, start) / 2;
			break;
		case Attack:
			start = 5 + param * 6;
			pos.x = -res::width(icn, start) / 2;
			pos.y = -res::height(icn, start) / 2;
			break;
		case MoveTo:
			start = 4 + param * 6;
			pos.x = -res::width(icn, start) / 2;
			pos.y = -res::height(icn, start) / 2;
			break;
		case MakeAction:
			start = 9 + param * 6;
			pos.x = -res::width(icn, start) / 2;
			pos.y = -res::height(icn, start) / 2;
			break;
		case CastleOnMap:
			start = 3;
			pos.x = -res::width(icn, start) / 2;
			pos.y = -res::height(icn, start) / 2;
			break;
		default:
			start = 0;
			pos.x = 0;
			pos.y = 0;
			break;
		}
		count = 1;
	} else if(id == AnimationType) {
		pos.x = 0;
		pos.y = 0;
		switch(value) {
		case SpellBless: icn = res::BLESS; break;
		case SpellBlind: icn = res::BLIND; break;
		case SpellCure: icn = res::MAGIC01; break;
		case SpellSlow: icn = res::MAGIC02; break;
		case SpellShield: icn = res::SHIELD; break;
		case SpellHaste: icn = res::HASTE; break;
		case SpellCurse: icn = res::CURSE; break;
		case SpellAntimagic: icn = res::MAGIC06; break;
		case SpellStoneSkin: icn = res::STONSKIN; break;
		case SpellSteelSkin: icn = res::STELSKIN; break;
		case SpellParalyze: icn = res::PARALYZE; break;
		case SpellHypnotize: icn = res::HYPNOTIZ; break;
		case SpellDragonSlayer: icn = res::DRAGSLAY; break;
		case SpellBerserker: icn = res::BERZERK; break;
		case Morale: icn = param ? res::MORALEG : res::MORALEB; break;
		default: icn = res::MAGIC07; break; // As dispell magic
		}
		switch(value) {
		case SpellBless:
		case SpellCurse:
		case SpellBerserker:
			pos.y = -24;
			break;
		case Morale:
			pos.y += 24;
			break;
		}
		start = 0;
		count = res::getcount(icn);
	}
	frame = start;
}

bool animation::hasanimation(tokens id, int param) const {
	animation test;
	test.set(rec, id, param);
	return test.count != 0;
}

bool animation::incframe() {
	if(wait) {
		wait--;
		return false;
	}
	if(++frame >= start + count) {
		frame = start;
		return true;
	}
	return false;
}

void animation::update() {
	incframe();
}

point animation::getzpos() const {
	return pos;
}

rect animation::getrect() const {
	rect result;
	result.x1 = pos.x + res::ox(icn, frame);
	result.y1 = pos.y + res::oy(icn, frame);
	result.x2 = result.x1 + res::width(icn, frame);
	result.y2 = result.y1 + res::height(icn, frame);
	return result;
}

point animation::getoffset(int start) const {
	point result = {0, 0};
	if(start == -1)
		start = this->start;
	for(int i = start; i < frame; i++) {
		result.x = res::ox(icn, i);
		result.y = res::oy(icn, i);
	}
	return result;
}

int animation::fly(point* result, point from, point to, int step) {
	auto p = result;
	int x1 = from.x; int y1 = from.y;
	int x2 = to.x; int y2 = to.y;
	const int dx = iabs(x2 - x1);
	const int dy = iabs(y2 - y1);
	int ns = (dx > dy ? dx : dy) / 2;
	for(int i = 0; i <= (dx > dy ? dx : dy); i++) {
		if(dx > dy) {
			x1 < x2 ? ++x1 : --x1;
			ns -= dy;
		} else {
			y1 < y2 ? ++y1 : --y1;
			ns -= dx;
		}
		if(ns < 0) {
			if(dx > dy) {
				y1 < y2 ? ++y1 : --y1;
				ns += dx;
			} else {
				x1 < x2 ? ++x1 : --x2;
				ns += dy;
			}
		}
		if((i % step) == 0) {
			p->x = x1;
			p->y = y1;
			p++;
		}
	}
	return p - result;
}

animation* animation::find(drawable** objects, int rec) {
	while(*objects) {
		if((*objects)->getid() == rec)
			return static_cast<animation*>(*objects);
		objects++;
	}
	return 0;
}

animation::state::state(animation* a) : a(a), flags(0) {
	if(a)
		flags = a->flags;
}

animation::state::~state() {
	if(a)
		a->flags = flags;
}