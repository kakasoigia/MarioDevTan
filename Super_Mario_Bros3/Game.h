#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "define.h"


#include <dinput.h>

#include "Scence.h"

using namespace std;

class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;
	
	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
	int game_time = 300;
	DWORD time_counter = 0;
	int coin = 0;
	long score = 000;
	int life_count = 4;
	int mario_level = 1;
	vector<int> itemList;
public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float &t,
		float &nx,
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	void SetCamX(float cam_x_FLoat)
	{
		cam_x = cam_x_FLoat;
	}
	void SetCamY(float cam_y_FLoat)
	{
		cam_y = cam_y_FLoat;
	}
	float GetCamPosX() { return cam_x; };
	float GetCamPosY() { return cam_y; };
	
	static CGame * GetInstance();
	int GetCurrentLevel() { return mario_level; };
	int GetGameTime() { return game_time; };
	~CGame();
	int GetCoinCounter() { return coin; };
	long GetScore() { return score; };
	int GetLifeCounter() { return life_count; };
	vector<int> GetItemList() { return itemList; };
	void SetCoinCounter(int coin) { this->coin = coin; };
	void SetScore(int score) { this->score = score; };
	void SetLifeCounter(int life_count) { this->life_count = life_count; };
	void SetItemList(vector<int> itemList) { this->itemList = itemList; };
	int GetMarioLevel() { return mario_level; };
	void SetMarioLevel(int level) { mario_level = level; };
};


