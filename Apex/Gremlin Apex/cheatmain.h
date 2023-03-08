#pragma once
#include "glob.h"
#include "vec.h"
#include <map>
#include <Xinput.h>
#pragma comment(lib,"XInput.lib")

#pragma comment(lib,"Xinput9_1_0.lib")

bool pause = false;

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	DWORD _controllerNum;
public:
	CXBOXController(int playerNumber)
	{
		// Set the Controller Number
		_controllerNum = playerNumber - 1;
	}

	XINPUT_STATE GetState()
	{
		// Zeroise the state
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		// Get the state
		XInputGetState(_controllerNum, &_controllerState);

		return _controllerState;
	}
	bool IsConnected()
	{
		// Zeroise the state
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		// Get the state
		DWORD Result = XInputGetState(_controllerNum, &_controllerState);

		if (Result == ERROR_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void Vibrate(int leftVal, int rightVal)
	{
		// Create a Vibraton State
		XINPUT_VIBRATION Vibration;

		// Zeroise the Vibration
		ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

		// Set the Vibration Values
		Vibration.wLeftMotorSpeed = leftVal;
		Vibration.wRightMotorSpeed = rightVal;

		// Vibrate the controller
		XInputSetState(_controllerNum, &Vibration);
	}
	bool isLeftTrigger() {
		if (_controllerState.Gamepad.bLeftTrigger > 0) return true;
		else return false;
	}
};

struct bone_t
{
	BYTE pad[0xCC];
	float x;
	BYTE pad2[0xC];
	float y;
	BYTE pad3[0xC];
	float z;
};

Vector3 GetBonePos(uintptr_t ent, int id)
{
	Vector3 pos = read<Vector3>(ent + PoopSets::M_VecAbsOrigin);
	uintptr_t bones = read<uintptr_t>(ent + PoopSets::demfuckingBone);
	Vector3 bone = {};
	UINT32 boneloc = (id * 0x30);
	bone_t bo = {};
	bo = read<bone_t>(bones + boneloc);

	bone.x = bo.x + pos.x;
	bone.y = bo.y + pos.y;
	bone.z = bo.z + pos.z;
	return bone;
}

uintptr_t GetEntityById(int Ent)
{
	uintptr_t EntityList = mem::find_image() + PoopSets::Cl_EntityList;
	uintptr_t BaseEntity = read<DWORD64>(EntityList);
	if (!BaseEntity)
		return NULL;
	return read<uintptr_t>(EntityList + (Ent << 5));
}


Vector3 getBonePositionByHitbox(int id, uintptr_t entity)
{
	Vector3 origin = read<Vector3>(entity + PoopSets::M_VecAbsOrigin);

	//BoneByHitBox
	uint64_t Model = *(uint64_t*)(entity + PoopSets::OFFSET_STUDIOHDR);

	//get studio hdr
	uint64_t StudioHdr = read<uint64_t>(Model + 0x8);


	uint16_t HitboxCache = read<uint16_t>(StudioHdr + 0x34);
	uint64_t HitBoxsArray = StudioHdr + ((uint16_t)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1)));

	uint16_t IndexCache = read<uint16_t>(HitBoxsArray + 0x4);
	int HitboxIndex = ((uint16_t)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));

	uint16_t Bone = read<uint16_t>(HitBoxsArray + HitboxIndex + (id * 0x20));

	if (Bone < 0 || Bone > 255)
		return Vector3();

	//hitpos
	uint64_t BoneArray = *(uint64_t*)(entity + PoopSets::demfuckingBone);

	matrix3x4_t Matrix = read<matrix3x4_t>(BoneArray + Bone * sizeof(matrix3x4_t));

	return Vector3(Matrix.m_flMatVal[0][3] + origin.x, Matrix.m_flMatVal[1][3] + origin.y, Matrix.m_flMatVal[2][3] + origin.z);
}



Vector3 ProjectWorldToScreen(const struct Vector3 pos) {

	uintptr_t render = read<uintptr_t>(mem::find_image() + PoopSets::ViewRender);
	uintptr_t Matrixa = read<uintptr_t>(PoopSets::ViewMatrix + render);

	BaseMatrix M = read<BaseMatrix>(Matrixa);
	struct Vector3 out;
	float _x = M.at[0] * pos.x + M.at[1] * pos.y + M.at[2] * pos.z + M.at[3];
	float _y = M.at[4] * pos.x + M.at[5] * pos.y + M.at[6] * pos.z + M.at[7];
	out.z = M.at[12] * pos.x + M.at[13] * pos.y + M.at[14] * pos.z + M.at[15];

	if (out.z < 0.1f)
		return { 0, 0,0 };

	_x *= 1.f / out.z;
	_y *= 1.f / out.z;

	out.x = ImGui::GetIO().DisplaySize.x * .5f;
	out.y = ImGui::GetIO().DisplaySize.y * .5f;

	out.x += 0.5f * _x * ImGui::GetIO().DisplaySize.x + 0.5f;
	out.y -= 0.5f * _y * ImGui::GetIO().DisplaySize.y + 0.5f;

	return out;

}


Vector3 getCamPos(uintptr_t local) {
	return read<Vector3>(local + PoopSets::camera_origin);
}

float previousPitch = 0;
float previousYaw = 0;

DWORD64 GetWeapon(DWORD64 entity)
{
	DWORD64 WeaponHandle = read<DWORD64>(entity + 0x1a14);
	WeaponHandle &= 0xffff;
	return read<DWORD64>(mem::find_image() + PoopSets::Cl_EntityList + (WeaponHandle << 5));
}

void noRecoil() {
	if (pause) return;
	if (options::noRecoil && !options::aimbot) {
		uintptr_t localPlayer = read<DWORD64>(mem::find_image() + PoopSets::Local_Player);
		if (localPlayer != NULL) {
			if (read<int>(localPlayer + PoopSets::M_BleedOutState) > 0) return;
			Vector2 angles;
			angles.y = read<float>(localPlayer + PoopSets::viewAngles); //works
			angles.x = read<float>(localPlayer + PoopSets::viewAngles + sizeof(float)); // works
			Vector2 punch = read<Vector2>(localPlayer + PoopSets::m_vecPunchWeapon_Angle);
			if (punch.y < 0 || punch.x < 0) {
				angles.x -= (punch.y - previousYaw) * options::yawStr;
				angles.y -= (punch.x - previousPitch) * options::pitchStr;
				//NormalizeAngles(angles);
				write<float>(localPlayer + PoopSets::viewAngles, angles.y);
				write<float>(localPlayer + PoopSets::viewAngles + sizeof(float), angles.x);
				previousPitch = punch.x;
				previousYaw = punch.y;
			}
		}
	}
	/// this is here because I dont wanna create a new thread or put it in the esp loop
	if (options::thirdperson) {
		uintptr_t localPlayer = read<DWORD64>(mem::find_image() + PoopSets::Local_Player);
		write<int>(mem::find_image() + PoopSets::thirdperson_override, 1);
		write<int>(localPlayer + PoopSets::m_thirdPersonShoulderView, 1);
	}
	else {
		uintptr_t localPlayer = read<DWORD64>(mem::find_image() + PoopSets::Local_Player);
		write<int>(mem::find_image() + PoopSets::thirdperson_override, -1);
		write<int>(localPlayer + PoopSets::m_thirdPersonShoulderView, 0);
	}
	if (options::instantBullet) {
		uintptr_t localPlayer = read<DWORD64>(mem::find_image() + PoopSets::Local_Player);
		uintptr_t Weapon = GetWeapon(localPlayer);
		write<float>(Weapon + PoopSets::m_flProjectileSpeed, options::bulletspped);
		write<int>(Weapon + PoopSets::m_flProjectileScale, 1.2f);
	}
}

Vector3 getFakeHead(uintptr_t ent) {
	Vector3 pos = { };
	if (ent == NULL)
		return pos;
	pos = read<Vector3>(ent + PoopSets::M_VecAbsOrigin);
	pos.z += 65;
	return pos;
}
Vector3 getFakeHitbox(uintptr_t ent) {
	Vector3 pos = { };
	if (ent == NULL)
		return pos;
	pos = read<Vector3>(ent + PoopSets::M_VecAbsOrigin);
	if (options::hitbox == hitboxes::body)
		pos.z += 45; // change this for height of scuffed body
	else
		pos.z += 55;
	return pos;
}

float visableTime(uintptr_t ent) {
	return read<float>(ent + PoopSets::lastVisibleTime);
}

Vector2 getAngles(uintptr_t local) {
	Vector2 angles;
	angles.x = read<float>(local + PoopSets::viewAngles + sizeof(float));
	angles.y = read<float>(local + PoopSets::viewAngles);
	return angles;
}

float pythag(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) * pow(y2 - y1, 2));
}

int crosshairX = 1920 / 2;
int crosshairY = 1080 / 2;

uintptr_t getBestEnt(uintptr_t local) {
	float closest = INT_MAX;
	uintptr_t bestEnt = 0;
	for (int i = 0; i < 200; i++) {
		uintptr_t Entity = GetEntityById(i);
		if (Entity == NULL) continue;
		if (read<int>(Entity + PoopSets::m_iHealth) <= 0) continue;
		if (Entity == local) continue;
		if (read<int>(Entity + PoopSets::M_ITeamNum) == read<int>(local + PoopSets::M_ITeamNum)) continue;
		if (read<int>(Entity + PoopSets::M_BleedOutState) > 0) continue;
		Vector3 Head2d = ProjectWorldToScreen(getFakeHitbox(Entity));
		float length = sqrt(pow((GetSystemMetrics(SM_CXSCREEN) / 2) - Head2d.x, 2) + pow((GetSystemMetrics(SM_CYSCREEN) / 2) - Head2d.y, 2));
		float distance = dist(read<Vector3>(local + PoopSets::M_VecAbsOrigin), read<Vector3>(Entity + PoopSets::M_VecAbsOrigin));
		if (visable::time[i] == visableTime(Entity) &&options::visCheck) continue;

		if (length < closest && length <= (options::aimbotFov * 3) && distance <= (options::maxDist * 15)) {
			closest = length;
			bestEnt = Entity;
		}
	}
	return bestEnt;
}

float add(float x, float y) {
	float d = abs(y) / y;
	for (int i = 1; i <= y; i++) {
		if ((x + d) > 179.f) {
			x = -179.f;
		}
		else if ((x + d) < -179.f) {
			x = 179.f;
		}
		else x += d;
	}
	return x;
}


void drawTarget(uintptr_t ent) {
	Vector3 screenPos = ProjectWorldToScreen(getFakeHitbox(ent));
}


void aimbot() {
	while (true) {
		if (pause) continue;
		if (!options::aimbot) continue;

		CXBOXController xbox(1);
		xbox.GetState();

		if (GetAsyncKeyState(options::curKey) || (options::curKey == 7 && (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON) || xbox.isLeftTrigger()))) {
			uintptr_t localPlayer = read<DWORD64>(mem::find_image() + PoopSets::Local_Player);
			if (localPlayer != NULL) {
				uintptr_t ent = getBestEnt(localPlayer);
				if (ent != NULL) {
					Vector2 angles{};
					if (options::hitbox == hitboxes::body)
						angles = CalcAngle(getCamPos(localPlayer), GetBonePos(ent, 6)); // change this for height of scuffed body
					else
						angles = CalcAngle(getCamPos(localPlayer), GetBonePos(ent, 8));
					if (angles.x != 0 && angles.y != 0) {
						Vector2 punch = read<Vector2>(localPlayer + PoopSets::m_vecPunchWeapon_Angle);
						angles.x -= (punch.x - previousPitch) * options::aimPitchStr;
						angles.y -= (punch.y - previousYaw) * options::aimYawStr;

						Vector2 curAngles = getAngles(localPlayer);

						curAngles = curAngles.flip();

						Vector2 delta = angles - curAngles;

						if (abs(delta.x) > 2 && abs(delta.y) > 2) {
							angles.x += m_rand::m_rand();
						}

						delta = angles - curAngles;

						normalY(delta.y);

						delta.y /= options::aimbotSmoothing;
						delta.x /= options::aimbotSmoothing;

						write<float>(localPlayer + PoopSets::viewAngles + sizeof(float), curAngles.y + delta.y);
						write<float>(localPlayer + PoopSets::viewAngles, curAngles.x + delta.x);
						std::this_thread::sleep_for(std::chrono::milliseconds(16));
					}
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

namespace items {
	enum rarity { HEIRLOOM, LEGENDARY, EPIC, RARE, COMMON };

	struct c_prop {
		std::string item_name;
		rarity m_rarity;
	};

	std::map<uint32_t, c_prop> item_list = {
		{ 1, { ("Kraber"), rarity::HEIRLOOM }},
		{ 2, { ("Mastiff"), rarity::COMMON }},
		{ 6, { ("Mastiff Gold"), rarity::LEGENDARY }},
		{ 7, { ("LStar"), rarity::COMMON }},
		{ 11, { ("LStar Gold"), rarity::LEGENDARY }},
		{ 12, { ("Havoc"), rarity::COMMON }},
		{ 16, { ("Havoc Gold"), rarity::LEGENDARY }},
		{ 17, { ("Devotion"), rarity::COMMON }},
		{ 21, { ("Devotion Gold"), rarity::LEGENDARY }},
		{ 22, { ("Triple Take"), rarity::HEIRLOOM }},
		{ 23, { ("Flatline"), rarity::COMMON }},
		{ 27, { ("Flatline Gold"), rarity::LEGENDARY }},
		{ 28, { ("Hemlock"), rarity::COMMON }},
		{ 32, { ("Hemlock Gold"), rarity::LEGENDARY }},
		{ 33, { ("G7 Scout"), rarity::COMMON }},
		{ 37, { ("G7 Scout Gold"), rarity::LEGENDARY }},
		{ 38, { ("Alternator"), rarity::HEIRLOOM }},
		{ 39, { ("R99"), rarity::COMMON }},
		{ 43, { ("R99 Gold"), rarity::LEGENDARY }},
		{ 44, { ("Prowler"), rarity::COMMON }},
		{ 48, { ("Prowler Gold"), rarity::LEGENDARY }},
		{ 49, { ("Volt"), rarity::COMMON }},
		{ 53, { ("Volt Gold"), rarity::LEGENDARY }},
		{ 54, { ("Longbow"), rarity::COMMON }},
		{ 58, { ("Longbow Gold"), rarity::LEGENDARY }},
		{ 59, { ("Charge Rifle"), rarity::COMMON }},
		{ 63, { ("Charge Rifle Gold"), rarity::LEGENDARY }},
		{ 64, { ("Spitfire"), rarity::HEIRLOOM }},
		{ 65, { ("R301"), rarity::COMMON }},
		{ 69, { ("R301 Gold"), rarity::LEGENDARY }},
		{ 70, { ("Eva-8 Auto"), rarity::COMMON }},
		{ 74, { ("Eva-8 Auto Gold"), rarity::LEGENDARY }},
		{ 75, { ("Peacekeeper"), rarity::COMMON }},
		{ 79, { ("Peacekeeper Gold"), rarity::LEGENDARY }},
		{ 80, { ("Mozambique"), rarity::COMMON }},
		{ 84, { ("Mozambique Gold"), rarity::LEGENDARY }},
		{ 85, { ("Wingman"), rarity::COMMON }},
		{ 89, { ("Wingman Gold"), rarity::LEGENDARY }},
		{ 90, { ("P2020"), rarity::COMMON }},
		{ 94, { ("P2020 Gold"), rarity::LEGENDARY }},
		{ 95, { ("RE45"), rarity::COMMON }},
		{ 99,  {("RE45 Gold"), rarity::LEGENDARY }},
		{ 100, { ("Sentinel"), rarity::COMMON }},
		{ 104, { ("Sentinel Gold"), rarity::LEGENDARY }},
		{ 105, { ("Bocek Bow"), rarity::COMMON }},
		{ 109, { ("Bocek Bow Gold"), rarity::LEGENDARY }},
		{ 110, { ("30 30 Repeater"), rarity::COMMON }},
		{ 114, { ("30 30 Repeater Gold"), rarity::LEGENDARY }},

		{ 115, { ("Light Ammo"), rarity::COMMON }},
		{ 116, { ("Energy Ammo"), rarity::COMMON }},
		{ 117, { ("Shotgun Ammo"), rarity::COMMON }},
		{ 118, { ("Heavy Ammo"), rarity::COMMON }},
		{ 119, { ("Sniper Ammo"), rarity::COMMON }},
		{ 120, { ("Arrow"), rarity::COMMON }},

		{ 121, { ("Rampage"), rarity::COMMON }},
		{ 125, { ("Rampage Gold"), rarity::LEGENDARY }},

		{ 151, { ("Ultimate Accelerant"), rarity::RARE }},
		{ 152, { ("Pheonix Kit"), rarity::EPIC }},
		{ 153, { ("Med Kit"), rarity::RARE }},
		{ 154, { ("Syringe"), rarity::COMMON }},
		{ 155, { ("Shield Battery"), rarity::RARE }},
		{ 156, { ("Shield Cell"), rarity::COMMON }},

		{ 157, { ("Helmet"), rarity::COMMON }},
		{ 158, { ("Helmet"), rarity::RARE }},
		{ 159, { ("Helmet"), rarity::EPIC }},
		{ 160, { ("Helmet"), rarity::LEGENDARY }},

		{ 161, { ("Body Armor"), rarity::COMMON }},
		{ 162, { ("Body Armor"), rarity::RARE }},
		{ 163, { ("Body Armor"), rarity::EPIC }},
		{ 164, { ("Body Armor"), rarity::LEGENDARY }},
		{ 165, { ("Body Armor"), rarity::HEIRLOOM }},

		{ 166, { ("Evo Body Armor"), rarity::COMMON }},
		{ 167, { ("Evo Body Armor"), rarity::RARE }},
		{ 168, { ("Evo Body Armor"), rarity::EPIC }},
		{ 169, { ("Evo Body Armor"), rarity::HEIRLOOM }},

		{ 170, { ("Heat Shield"), rarity::HEIRLOOM }},

		{ 171, { ("Knockdown Shield"), rarity::COMMON }},
		{ 172, { ("Knockdown Shield"), rarity::RARE }},
		{ 173, { ("Knockdown Shield"), rarity::EPIC }},
		{ 174, { ("Knockdown Shield"), rarity::LEGENDARY }},

		{ 175, { ("Backpack"), rarity::COMMON }},
		{ 176, { ("Backpack"), rarity::RARE }},
		{ 177, { ("Backpack"), rarity::EPIC }},
		{ 178, { ("Backpack"), rarity::LEGENDARY }},

		{ 179, { ("Thermite Grenade"), rarity::COMMON }},
		{ 180, { ("Frag Grenade"), rarity::COMMON }},
		{ 181, { ("Arc Star"), rarity::COMMON }},

		{ 182, { ("1x HCOG 'Classic'"), rarity::COMMON }},
		{ 183, { ("2x HCOG 'Bruiser'"), rarity::RARE }},
		{ 184, { ("1x Holo"), rarity::COMMON }},
		{ 185, { ("1x-2x Variable Holo"), rarity::RARE }},
		{ 186, { ("1x Digital Threat"), rarity::LEGENDARY }},
		{ 187, { ("3x HCOG 'Ranger'"), rarity::EPIC }},
		{ 188, { ("2x-4x Variable AOG"), rarity::EPIC }},
		{ 189, { ("6x Sniper"), rarity::RARE }},
		{ 190, { ("4x-8x Variable Sniper"), rarity::EPIC }},
		{ 191, { ("4x-10x Digital Sniper Threat"), rarity::LEGENDARY }},

		{ 192, { ("Barrel Stabilizer"), rarity::COMMON }},
		{ 193, { ("Barrel Stabilizer"), rarity::RARE }},
		{ 194, { ("Barrel Stabilizer"), rarity::EPIC }},
		{ 195, { ("Barrel Stabilizer"), rarity::LEGENDARY }}, // Unobtainable?

		{ 196, { ("Light Magazine"), rarity::COMMON }},
		{ 197, { ("Light Magazine"), rarity::RARE }},
		{ 198, { ("Light Magazine"), rarity::EPIC }},
		{ 199, { ("Light Magazine"), rarity::LEGENDARY }},

		{ 200, { ("Heavy Magazine"), rarity::COMMON }},
		{ 201, { ("Heavy Magazine"), rarity::RARE }},
		{ 202, { ("Heavy Magazine"), rarity::EPIC }},
		{ 203, { ("Heavy Magazine"), rarity::LEGENDARY }},

		{ 204, { ("Energy Magazine"), rarity::COMMON }},
		{ 205, { ("Energy Magazine"), rarity::RARE }},
		{ 206, { ("Energy Magazine"), rarity::EPIC }},
		{ 207, { ("Energy Magazine"), rarity::LEGENDARY }},

		{ 208, { ("Sniper Magazine"), rarity::COMMON }},
		{ 209, { ("Sniper Magazine"), rarity::RARE }},
		{ 210, { ("Sniper Magazine"), rarity::EPIC }},
		{ 211, { ("Sniper Magazine"), rarity::LEGENDARY }},

		{ 212, { ("Shotgun Bolt"), rarity::COMMON }},
		{ 213, { ("Shotgun Bolt"), rarity::RARE }},
		{ 214, { ("Shotgun Bolt"), rarity::EPIC }},

		{ 215, { ("Standard Stock"), rarity::COMMON }},
		{ 216, { ("Standard Stock"), rarity::RARE }},
		{ 217, { ("Standard Stock"), rarity::EPIC }},

		{ 218, { ("Sniper Stock"), rarity::COMMON }},
		{ 219, { ("Sniper Stock"), rarity::RARE }},
		{ 220, { ("Sniper Stock"), rarity::EPIC }},

		{ 221, { ("Turbocharger"), rarity::LEGENDARY }},
		{ 222, { ("Skull Piercer"), rarity::LEGENDARY }},
		{ 223, { ("Hammer Point"), rarity::EPIC }},
		{ 224, { ("Anvil Receiver"), rarity::LEGENDARY }},
		{ 225, { ("Graffiti Mod"), rarity::COMMON }},
		{ 226, { ("Paintball 1"), rarity::HEIRLOOM }}, // TODO: Find this rarity
		{ 227, { ("Paintball 2"), rarity::HEIRLOOM }}, // TODO: Find this rarity
		{ 228, { ("Deadeye's Tempo"), rarity::EPIC }},
		{ 229, { ("Quickdraw Holster"), rarity::EPIC }}, // Might be 229
		{ 230, { ("Shatter Caps"), rarity::EPIC }},
		{ 231, { ("Boosted Loader"), rarity::LEGENDARY }},
		{ 232, { ("Mobile Respawn Beacon"), rarity::RARE }},
		{ 233, { ("Vault Key"), rarity::HEIRLOOM }},
		{ 234, { ("Steel Cut"), rarity::HEIRLOOM }}, // Might be overwritten every season/every time there's a new charm.
		{ 235, { ("Treasure Pack"), rarity::HEIRLOOM }},
		{ 236, { ("Survival Doubler"), rarity::HEIRLOOM }}, // TODO: Find this rarity
		{ 237, { ("Survival Quadrupler"), rarity::HEIRLOOM }}, // TODO: Find this rarity
		{ 238, { ("Heat Shield"), rarity::RARE }},
		{ 239, { ("Mobile Respawn Beacon 2"), rarity::RARE }},
		{ 240, { ("MRVN Arm"), rarity::EPIC }},
		{ 241, { ("Vault Key"), rarity::HEIRLOOM }}
	};
}

int get_item_id(uintptr_t entity) {
	return read<int>(entity + PoopSets::ItemId);
}



void espone() {
	if (pause) return;
	for (int i = 0; i < 200; i++)
	{
		ImDrawList* draw_list = ImGui::GetOverlayDrawList();
		uintptr_t localPlayer = read<DWORD64>(mem::find_image() + PoopSets::Local_Player);
		if (localPlayer == NULL) continue;
		uintptr_t Entity = GetEntityById(i);
		if (Entity == NULL) continue;
		if (read<int>(Entity + PoopSets::m_iHealth) <= 0) continue;
		if (Entity == localPlayer) continue;
		ImColor boxCol((int)options::glowCol[0], (int)options::glowCol[1], (int)options::glowCol[2]);
		
		Vector3 camPos = getCamPos(localPlayer);
		if (camPos.x == 0) continue;
		if (read<int>(localPlayer + PoopSets::M_BleedOutState > 0)) continue;
		int entTeam = read<int>(Entity + PoopSets::M_ITeamNum);
		int localTeam = read<int>(localPlayer + PoopSets::M_ITeamNum);
		if (entTeam == localTeam) continue;
		uintptr_t localplayerweapon = GetWeapon(localPlayer);
		if (options::changer)
		{
			write<int>(localplayerweapon + PoopSets::m_nSkin, options::ski);

		}
		float rainbowSpeed = 0.0001;
		static float staticHue = 0;
		staticHue -= rainbowSpeed;
		ImColor cRainbow;
		int wiadth = 50;
		if (staticHue < -1.f) staticHue += 1.f;
		for (int i = 0; i < wiadth; i++)
		{
			float hue = staticHue + (1.f / (float)wiadth) * i;
			if (hue < 0.f) hue += 1.f;
			cRainbow = ImColor::HSV(hue, 1.f, 1.f);
		}



		Vector3 pos3D = read<Vector3>(Entity + PoopSets::M_VecAbsOrigin);
		if (dist(pos3D, read<Vector3>(localPlayer + PoopSets::M_VecAbsOrigin)) > (options::espMaxDist) * 15) continue;
		Vector3 headPos3D = pos3D;
		headPos3D.z += 70;
		Vector3 headPos2D = ProjectWorldToScreen(headPos3D);
		Vector3 pos2D = ProjectWorldToScreen(pos3D);
		ImColor c = { 255, 255, 255 };



		options::glowCol[0] = (int)(options::gcol[0] * 255);
		options::glowCol[1] = (int)(options::gcol[1] * 255);
		options::glowCol[2] = (int)(options::gcol[2] * 255);
		int glowr, glowg, glowb = cRainbow;
		if (options::glowType == 0) {
			glowr = options::glowCol[0];
			glowg = options::glowCol[1];
			glowb = options::glowCol[2];
		}
		else if (options::glowType == 1) {
			int max = read<int>(Entity + PoopSets::m_iMaxHealth) + read<int>(Entity + PoopSets::m_shieldHealthMax);
			int current = read<int>(Entity + PoopSets::m_iHealth) + read<int>(Entity + PoopSets::m_shieldHealth);
			if (current > max * 0.6) {
				glowr = 0;
				glowg = 255;
				glowb = 0;
			}
			else if (current > max * 0.3) {
				glowr = 255;
				glowg = 255;
				glowb = 0;
			}
			else {
				glowr = 255;
				glowg = 0;
				glowb = 0;
			}
		}
		 if (options::glowType == 2) {
			if (visable::time[i] == visableTime(Entity)) {
				glowr = 0;
				glowg = 255;
				glowb = 0;
			}
			else {
				glowr = 255;
				glowg = 0;
				glowb = 0;
			}
		}
		if (options::glow)
		{
			write<int>(Entity + PoopSets::Glow_Enabled, 1); // glow enable: 1 = enabled, 2 = disabled
			write<int>(Entity + PoopSets::Glow_Through_Walls, 2); // glow through walls: 2 = enabled, 5 = disabled
			write<GlowMode>(Entity + PoopSets::Glow_Type, { 101,101,90,100 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
			write<float>(Entity + 0x1D0, glowr);
			write<float>(Entity + 0x1D4, glowg);
			write<float>(Entity + 0x1D8, glowb);

		}
		if (options::playerinfo)
		{
			int currentHealth = read<int>(Entity + PoopSets::m_iMaxHealth);

			int currentSheild = read<int>(Entity + PoopSets::m_shieldHealth);
			ImGui::SetNextWindowSize(ImVec2(200, 275));
			ImGui::SetWindowPos(ImVec2(5, 0));
			std::string Health = std::to_string(int(currentHealth));


			std::string Sheild = "Shield : [ " + std::to_string(int(currentSheild)) + " ]";
			std::string heal = "Health : [ " + std::to_string(int(currentHealth)) + " ]";
			ImGuiIO& io = ImGui::GetIO();
			ImGui::Begin("Current Player Info", nullptr, ImGuiWindowFlags_NoResize | WS_EX_TOPMOST | CS_CLASSDC | WS_EX_TOPMOST | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
			{
				ImGui::Spacing();
				ImGui::Text(Sheild.c_str());
				ImGui::Spacing();
				ImGui::Text(heal.c_str());
				ImGui::Spacing();
				ImGui::Spacing();
			}
			ImGui::End();
		}
		if (options::drawCircle) {

			draw_list->AddCircle(ImVec2(xa / 2, ya / 2), options::aimbotFov * 3, ImColor(255, 255, 255), 36);
		}
		if (options::healthBar) {
			int currentHealth = read<int>(Entity + PoopSets::m_iMaxHealth);

			int currentSheild = read<int>(Entity + PoopSets::m_shieldHealth);
			//std::string Weapon = read_wstr(Entity + PoopSets::CurrentWeapon);
			write<float>(localPlayer + PoopSets::OFFSET_ZOOM_FOV, 100.0f);
			ImGui::SetNextWindowSize(ImVec2(200, 50));
			ImGui::SetWindowPos(ImVec2(5, 0));
			std::string Health = std::to_string(int(currentHealth));

			std::string Sheild = "[ Shield : " + std::to_string(int(currentSheild)) + " ]";
			//std::string Weap = "[ " + Weapon + " ]";
			draw_list->AddText(ImVec2(headPos2D.x + 30, headPos2D.y + 30), ImColor(255, 255, 255), Health.c_str());
			draw_list->AddText(ImVec2(headPos2D.x + 30, headPos2D.y + 20), ImColor(255, 255, 255), Sheild.c_str());
			ImGuiIO& io = ImGui::GetIO();

			if (currentSheild > 0) {
				ImColor color;
				int maxSheild = read<int>(Entity + PoopSets::m_shieldHealthMax);
				write<float>(localPlayer + PoopSets::OFFSET_ZOOM_FOV, 100.0f);


			}
			else {
				ImColor color = ImColor(0, 255, 0);

				int dist = pos2D.y - headPos2D.y;
				int length = dist / 2;
				length *= (read<int>(Entity + PoopSets::m_iHealth) / read<int>(Entity + PoopSets::m_iMaxHealth));

				Vector3 newHead = headPos3D;
				newHead.z += 2;
				Vector3 pos = ProjectWorldToScreen(newHead);


				draw_list->AddLine(ImVec2(newHead.x - (dist / 4), newHead.y), ImVec2((newHead.x - dist / 4) + length, newHead.y), color, 3);
			}
		}

		if (options::cornerBox) {
			draw::DrawCorneredBox(ImVec2(headPos2D.x, headPos2D.y), ImVec2(pos2D.x, pos2D.y), ImColor(0, 0, 0), 5);
			DrawCorneredBox(ImVec2(headPos2D.x, headPos2D.y), ImVec2(pos2D.x, pos2D.y), ImColor(boxCol), 1);
		}

		if (options::box)
		{
			DrawBox(ImVec2(headPos2D.x, headPos2D.y), ImVec2(pos2D.x, pos2D.y), ImColor(0, 0, 0), 5);
			DrawBox(ImVec2(headPos2D.x, headPos2D.y), ImVec2(pos2D.x, pos2D.y), ImColor(boxCol), 1);

		}
		if (options::linesToPlayer) {
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(1920 / 2, 1080), ImVec2(pos2D.x, pos2D.y), ImColor(255, 0, 0));
		}

		if (options::drawDist) {
			Vector3 newPos = pos3D;
			newPos.z -= 3;
			Vector3 newpos2d = ProjectWorldToScreen(newPos);
			float distance = dist(pos3D, read<Vector3>(localPlayer + PoopSets::M_VecAbsOrigin));
			std::string Distance = "[ " + std::to_string(int(distance / 15)) + "M ]";
			DrawStrokeText(newpos2d.x, newpos2d.y, Distance.c_str());
		}


	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
void NoSpread() {
	while (true) {
		if (pause) continue;
		if (!options::instantBullet) continue;
		uintptr_t localPlayer = read<DWORD64>(mem::find_image() + PoopSets::Local_Player);
		uintptr_t Weapon = GetWeapon(localPlayer);
		if (read<float>(Weapon + 0x16c0) != 0.0f)
		{
			write<float>(Weapon + 0x16c0, -5.0f);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void itemGlow() {
	while (true) {
		if (options::itemgl && !pause) {
			for (int a = 0; a < 12000; a++)
			{
				uintptr_t Entity = GetEntityById(a);

				if (options::itemgl)
					write<int>(Entity + 0x2C0, 1363184265);
				else
					write<int>(Entity + 0x2C0, 0);
			}
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
}

void printPlayerInfo() {
	while (true) {
		mem::find_process(_("r5apex.exe").c_str());
		if (!mem::process_id)
			exit(1);
		uintptr_t localPlayer = read<uintptr_t>(mem::find_image() + PoopSets::Local_Player);
		int health = read<int>(localPlayer + PoopSets::m_iHealth);
		//read<int>(localPlayer + PoopSets::M_BleedOutState);
		Vector3 camPos = getCamPos(localPlayer);
		//Vector3 pos = read<Vector3>(localPlayer + PoopSets::M_VecAbsOrigin);
		if (localPlayer == NULL || (camPos.x == 0 && camPos.y == 0))
			pause = true;
		else pause = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}
