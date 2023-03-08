#pragma once
#include <fstream>
#include <Windows.h>
#include "glob.h"

class opt {
public:
	// AIMBOT
	bool aimbot;
	float aimbotFov;
	float aimbotSmoothing;
	bool visCheck;
	float maxDist;
	int hitbox;
	float aimPitchStr;
	float aimYawStr;
	bool drawCircle;
	int keyBind;
	int curKey;
	bool instantBullet;
	float bulletspped;

	// ESP
	bool box;
	bool cornerBox;
	bool healthBar;
	bool linesToPlayer;
	bool glow;
	int glowType;
	bool drawDist;
	float espMaxDist;
	bool playerinfo;
	int ski;

	// LOOT
	bool itemgl;
	int rarity;

	// MISC
	bool noRecoil;
	float yawStr;
	float pitchStr;
	bool thirdperson;
	bool changer;

	//COLORS
	float cbox[3];
	int boxCol[3];
	float gcol[3];
	int glowCol[3];
};

namespace configs {

	void saveConfig() {
		opt config;

		config.aimbot = options::aimbot;
		config.aimbotFov = options::aimbotFov;
		config.aimbotSmoothing = options::aimbotSmoothing;
		config.visCheck = options::visCheck;
		config.maxDist = options::maxDist;
		config.hitbox = options::hitbox;
		config.aimPitchStr = options::aimPitchStr;
		config.aimYawStr = options::aimYawStr;
		config.drawCircle = options::drawCircle;
		config.keyBind = options::keyBind;
		config.curKey = options::curKey;
		config.instantBullet = options::instantBullet;
		config.bulletspped = options::bulletspped;

		// ESP
		config.box = options::box; 
		config.cornerBox = options::cornerBox;
		config.healthBar = options::healthBar;
		config.linesToPlayer = options::linesToPlayer;
		config.glow = options::glow;
		config.glowType = options::glowType;
		config.drawDist = options::drawDist;
		config.espMaxDist = options::espMaxDist;
		config.playerinfo = options::playerinfo;
		config.ski = options::ski;

		// LOOT
		config.itemgl = options::itemgl;
		config.rarity = options::rarity;

		// MISC
		config.noRecoil = options::noRecoil;
		config.yawStr = options::yawStr;
		config.pitchStr = options::pitchStr;
		config.thirdperson = options::thirdperson;
		config.changer = options::changer;

		//COLORS
		config.cbox[0] = options::cbox[0];
		config.cbox[1] = options::cbox[1];
		config.cbox[2] = options::cbox[2];

		config.boxCol[0] = options::boxCol[0];
		config.boxCol[1] = options::boxCol[1];
		config.boxCol[2] = options::boxCol[2];

		config.gcol[0] = options::gcol[0];
		config.gcol[1] = options::gcol[1];
		config.gcol[2] = options::gcol[2];

		config.glowCol[0] = options::glowCol[0];
		config.glowCol[1] = options::glowCol[1];
		config.glowCol[2] = options::glowCol[2];

		std::ofstream oFile;
		oFile.open("config.dat", std::ios::binary | std::ios::app);
		oFile.write(reinterpret_cast<char*> (&config), sizeof(opt));
		oFile.close();
	}

	void loadConfig() {
		opt config;
		std::ifstream iFile;
		iFile.open("config.dat", std::ios::binary);
		iFile.read(reinterpret_cast<char*> (&config), sizeof(opt));
		iFile.close();

		options::aimbot = config.aimbot;
		options::aimbotFov = config.aimbotFov;
		options::aimbotSmoothing = config.aimbotSmoothing;
		options::visCheck = config.visCheck;
		options::maxDist = config.maxDist;
		options::hitbox = config.hitbox;
		options::aimPitchStr = config.aimPitchStr;
		options::aimYawStr = config.aimYawStr;
		options::drawCircle = config.drawCircle;
		options::keyBind = config.keyBind;
		options::curKey = config.curKey;
		options::instantBullet = config.instantBullet;
		options::bulletspped = config.bulletspped;

		// ESP
		options::box = config.box;
		options::cornerBox = config.cornerBox;
		options::healthBar = config.healthBar;
		options::linesToPlayer = config.linesToPlayer;
		options::glow = config.glow;
		options::glowType = config.glowType;
		options::drawDist = config.drawDist;
		options::espMaxDist = config.espMaxDist;
		options::playerinfo = config.playerinfo;
		options::ski = config.ski;

		// LOOT
		options::itemgl = config.itemgl;
		options::rarity = config.rarity;

		// MISC
		options::noRecoil = config.noRecoil;
		options::yawStr = config.yawStr;
		options::pitchStr = config.pitchStr;
		options::thirdperson = config.thirdperson;
		options::changer = config.changer;

		//COLORS
		options::cbox[0] = config.cbox[0];
		options::cbox[1] = config.cbox[1];
		options::cbox[2] = config.cbox[2];

		options::boxCol[0] = config.boxCol[0];
		options::boxCol[1] = config.boxCol[1];
		options::boxCol[2] = config.boxCol[2];

		options::gcol[0] = config.gcol[0];
		options::gcol[1] = config.gcol[1];
		options::gcol[2] = config.gcol[2];

		options::glowCol[0] = config.glowCol[0];
		options::glowCol[1] = config.glowCol[1];
		options::glowCol[2] = config.glowCol[2];
	}
}