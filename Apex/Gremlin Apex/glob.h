#pragma once
#include "include.hpp"
#define Assert( _exp ) ((void)0)
enum hitboxes {
	body,
	head
};

int xa = GetSystemMetrics(SM_CXSCREEN);
 int ya = GetSystemMetrics(SM_CYSCREEN);

namespace options {
	// AIMBOT
	bool aimbot = false;
	float aimbotFov = 50.0f;
	float aimbotSmoothing = 1.0f;
	bool visCheck = false;
	float maxDist = 250.f;
	const char* hitBoxes[]{ "Body", "Head" };
	int hitbox = 0;
	float aimPitchStr = 1.f;
	float aimYawStr = 1.f;
	bool drawCircle = false;
	const char* keyBinds[]{ "Mouse 1", "Mouse 2", "M1 & M2", "Left alt"};
	int keyBind = 1;
	int curKey;
	bool instantBullet = false;
	float bulletspped = 1000;

	// ESP
	 bool box = false;
	 bool cornerBox = false;
	 bool healthBar = false;
	 bool linesToPlayer = false;
	 bool glow = false;
	 const char* glowTypes[3] = { "Normal", "Health" , "Visable Colors" };
	 int glowType = 0;
	 bool drawDist = false;
	 float espMaxDist = 1000.f;
	 bool playerinfo = false;
	 int ski = 1;

	// LOOT
	 bool itemgl = false;
	 int rarity = 0;

	 // MISC
	 bool noRecoil = false;
	 float yawStr = 0.f;
	 float pitchStr = 0.f;
	 bool thirdperson = false;
	 bool changer = false;
	 bool noSmoke = false;

	//COLORS
	 float cbox[3] = { 0, 0, 0 };
	 float boxCol[4] = { 0, 0, 0, 0 };
	 float gcol[4] = { 0, 0, 0 };
	 int glowCol[3] = { 0, 0, 0 };
}


namespace PoopSets {

	const DWORD Local_Player = 0x1edb678;

	const DWORD Cl_EntityList = 0x1b2a578;
	const DWORD Local_Entity_Handle = 0x1352c7c;
	const DWORD M_VecAbsOrigin = 0x014c;
	const DWORD M_bConstrainBetweenEndpoints = 0x0f50; //m_nForceBone  0x0e88 + 0x48
	const DWORD demfuckingBone = 0x0e98 + 0x48;
	const DWORD ItemId = 0x1648;
	const DWORD M_IName = 0x0589;
	const DWORD m_scriptNameIndex = 0x0690;
	const DWORD m_nSkin = 0x0e54;
	const DWORD m_vecPunchWeapon_Angle = 0x24a8;
	const DWORD ViewMatrix = 0x11a210;
	const DWORD ViewRender = 0x7664e80;
	const DWORD m_ammoPoolCapacity = 0x25a4;
	const DWORD viewAngles = m_ammoPoolCapacity - 0x14;
	const DWORD camera_origin = 0x1f48;
	const DWORD M_ITeamNum = 0x044c; //updated 
	const DWORD M_BleedOutState = 0x2738; //updated
	const DWORD M_IWorldModelIndex = 0x1658; //updated
	const DWORD Glow_Type = 0x2C4;
	const DWORD Glow_Enabled = 0x3c8;
	const DWORD Glow_Through_Walls = 0x3d0;
	const DWORD m_highlightFunctionBits = 0x02c0; //item_glow //updated

	const DWORD m_vecAbsVelocity = 0x0140;

	const DWORD m_ActiveState = 0x166c; //updated

	const DWORD CurrentWeapon = 0x1a14; //m_latestPrimaryWeapons //updated
	const DWORD m_ammoInClip = 0x1644; //updated

	const DWORD m_iHealth = 0x043c; //updated
	const DWORD m_iMaxHealth = 0x0578; //updated 
	const DWORD m_shieldHealth = 0x170; //updated
	const DWORD m_shieldHealthMax = 0x0174; //updated
	const DWORD CPlayerarmorType = 0x45bc; //updated

	const DWORD m_flProjectileSpeed = 0x1ef0;
	const DWORD m_flProjectileScale = 0x1ef8;
	const DWORD OFFSET_ZOOM_FOV = 0x16b8 + 0xb8; //m_playerData + m_curZoomFOV
	const DWORD m_thirdPersonShoulderView = 0x36c8; //updated 
	const DWORD thirdperson_override = 0x01b0f000 + 0x6c; //updated
	const DWORD m_playerMoveSpeedScale = 0x3214; //updated
	const DWORD m_flModelScale = 0x0f08; //updated
	const DWORD m_skydiveIsDiving = 0x46a8; //updated

	const DWORD NameList = 0xbbda330;
	const DWORD lastVisibleTime = 0x1a70;
	const DWORD OFFSET_STUDIOHDR = 0x10f0;
}

struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}

	float* operator[](int i) { Assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
	const float* operator[](int i) const { Assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
	float* Base() { return &m_flMatVal[0][0]; }
	const float* Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

namespace rainbow {
	float rainbowSpeed = 0.0001;
	static float staticHue = 0;
	ImColor cRainbow;
	int red;
	int blue;
	int green;

	void HSVtoRGB(float H, float S, float V) {
		if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
			return;
		}
		float s = S / 100;
		float v = V / 100;
		float C = s * v;
		float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
		float m = v - C;
		float r, g, b;
		if (H >= 0 && H < 60) {
			r = C, g = X, b = 0;
		}
		else if (H >= 60 && H < 120) {
			r = X, g = C, b = 0;
		}
		else if (H >= 120 && H < 180) {
			r = 0, g = C, b = X;
		}
		else if (H >= 180 && H < 240) {
			r = 0, g = X, b = C;
		}
		else if (H >= 240 && H < 300) {
			r = X, g = 0, b = C;
		}
		else {
			r = C, g = 0, b = X;
		}
		red = (r + m) * 255;
		green = (g + m) * 255;
		blue = (b + m) * 255;
	}

	void create(){
		staticHue -= rainbowSpeed;
		if (staticHue < -1.f) staticHue += 1.f;
		for (int i = 0; i < 1920; i++)
		{
			float hue = staticHue + (1.f / (float)1920) * i;
			if (hue < 0.f) hue += 1.f;
			cRainbow = ImColor::HSV(hue, 1.f, 1.f);
			HSVtoRGB(hue, 1.f, 1.f);
		}
	}
	void run() {
		while (true) {
			create();
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
}

namespace m_rand {
	int m_randNum;

	void genRand() {
		srand(time(0));
		while (true) {
			m_randNum = (rand() % 10) + 1;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	float m_rand() {
		if (m_randNum > 5) return 0.5f;
		else return -0.5f;
	}
}


namespace visable {
	float time[200];
	
	uintptr_t GetEntityById(int Ent)
	{
		uintptr_t EntityList = mem::find_image() + PoopSets::Cl_EntityList;
		uintptr_t BaseEntity = read<DWORD64>(EntityList);
		if (!BaseEntity)
			return NULL;
		return read<uintptr_t>(EntityList + (Ent << 5));
	}

	void updateTime() {
		while (true) {
			if (!options::aimbot && !options::glow) continue;
			for (int i = 0; i < 200; i++) {
				uintptr_t localPlayer = read<DWORD64>(mem::find_image() + PoopSets::Local_Player);
				uintptr_t Entity = GetEntityById(i);
				if (Entity == NULL) continue;
				if (read<int>(Entity + PoopSets::m_iHealth) <= 0) continue;
				if (Entity == localPlayer) continue;
				if (read<int>(Entity + PoopSets::M_ITeamNum) == read<int>(localPlayer + PoopSets::M_ITeamNum)) continue;
				if (read<int>(Entity + PoopSets::M_BleedOutState) > 0) continue;
				time[i] = read<float>(Entity + PoopSets::lastVisibleTime);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
}

