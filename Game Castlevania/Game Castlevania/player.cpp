#include <time.h>
#include <d3dx9.h>
#include "player.h"
#include "utils.h"

#define SIMON_IMAGE_PATH L"resources\\images\\simon.bmp"
//#define KITTY_IMAGE_RIGHT L"kitty_right.bmp"
//#define KITTY_IMAGE_LEFT L"kitty_left.bmp"
#define KITTY_IMAGE_JUMP L"resources\\images\\simon_jump.bmp"
#define KITTY_IMAGE_LEFT L"resources\\images\\simon_left.bmp"
#define KITTY_SPEED 0.35f
#define GROUND_Y 300

#define BACKGROUND_FILE L"bg.bmp" 
//#define BACKGROUND_FILE L"Stage12.png"

#define ANIMATE_RATE 30

Player::Player(HINSTANCE hInstance, LPWSTR Name, int Mode, int IsFullScreen, int FrameRate) :
CGame(hInstance, Name, Mode, IsFullScreen, FrameRate)
{
	kitty_jump = NULL;
	kitty_left = NULL;
	MOVE_STATE = STAND;
}

Player::~Player()
{
	delete kitty_left;
	delete kitty_jump;
}

void Player::LoadResources(LPDIRECT3DDEVICE9 d3ddv)
{
	srand((unsigned)time(NULL));

	// TO-DO: not a very good place to initial sprite handler
	D3DXCreateSprite(d3ddv, &_SpriteHandler);

	Background = CreateSurfaceFromFile(_d3ddv, BACKGROUND_FILE);

	HRESULT res = D3DXCreateSprite(_d3ddv, &_SpriteHandler);

	kitty_x = 50;
	kitty_y = GROUND_Y;

	kitty_vx = 0;
	kitty_vx_last = 1.0f;
	kitty_vy = 0;

	//kitty_jump = new CSprite(_SpriteHandler, KITTY_IMAGE_JUMP, 32, 48, 1, 1);
	kitty_left = new CSprite(_SpriteHandler, KITTY_IMAGE_LEFT, 32, 64, 3, 3);
}

void Player::RenderFrame(LPDIRECT3DDEVICE9 d3ddv, int t)
{
	// TO-DO: should enhance CGame to put a separate virtual method for updating game status


	// NOTES: If there is a class for kitty, this should be Kitty->Update(t);
	// Update kitty status
	kitty_x += kitty_vx * t;
	kitty_y += kitty_vy * t;

	// Animate kitty if she is running
	DWORD now = GetTickCount();
	if (now - last_time > 1000 / ANIMATE_RATE)
	{
		if (kitty_vx > 0) kitty_left->Next();
		if (kitty_vx < 0) kitty_left->Next();

		last_time = now;
	}

	// Simulate fall down
	if (kitty_y < GROUND_Y) kitty_vy += 0.5f;
	else
	{
		kitty_y = GROUND_Y;
		kitty_vy = 0;
	}

	// Background
	d3ddv->StretchRect(
		Background,			// from 
		NULL,				// which portion?
		_BackBuffer,		// to 
		NULL,				// which portion?
		D3DTEXF_NONE);


	_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

	// Kitty
	// If there is a class for kitty, this should be Kitty->Render();
	//if (MOVE_STATE == MOVE)
	{
		if (kitty_vx > 0)
		{
			kitty_left->Render(_BackBuffer, kitty_x, kitty_y, true);
		}
		else if (kitty_vx < 0)
		{

			kitty_left->Render(_BackBuffer, kitty_x, kitty_y);

		}
		else if (kitty_vx_last < 0)
			kitty_left->Render(_BackBuffer, kitty_x, kitty_y);
		else
			kitty_left->Render(_BackBuffer, kitty_x, kitty_y, true);
	}
	
	//if (MOVE_STATE == JUMP)
	{
		//kitty_jump->Render(_BackBuffer, kitty_x, kitty_y);
	}

	_SpriteHandler->End();
}

void Player::ProcessInput(LPDIRECT3DDEVICE9 d3ddv, int t)
{
	if (IsKeyDown(DIK_RIGHT))
	{
		MOVE_STATE = MOVE;
		kitty_vx = KITTY_SPEED;
		kitty_vx_last = kitty_vx;
	}
	else
		if (IsKeyDown(DIK_LEFT))
		{
		MOVE_STATE = MOVE;
		kitty_vx = -KITTY_SPEED;
		kitty_vx_last = kitty_vx;
		}
		else
		{
			MOVE_STATE = STAND;
			kitty_vx = 0;
			kitty_left->Reset();
		}
}

void Player::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_SPACE:
		MOVE_STATE = JUMP;
		if (kitty_y >= GROUND_Y) kitty_vy -= 3.0f;			// start jump if is not "on-air"
		break;
	}
}


