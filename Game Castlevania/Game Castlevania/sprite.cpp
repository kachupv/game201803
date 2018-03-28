#include "sprite.h"
#include "trace.h"

#include <d3dx9.h>

CSprite::CSprite(LPD3DXSPRITE SpriteHandler, LPWSTR FilePath, int Width, int Height, int Count, int SpritePerRow)
{
	D3DXIMAGE_INFO info;
	HRESULT result;

	_Image = NULL;
	_SpriteHandler = SpriteHandler;

	_Width = Width;
	_Height = Height;
	_Count = Count;
	_SpritePerRow = SpritePerRow;
	_Index = 0;

	result = D3DXGetImageInfoFromFile(FilePath, &info);
	if (result != D3D_OK)
	{
		trace(L"[ERROR] Failed to get information from image file '%s'", FilePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv;
	SpriteHandler->GetDevice(&d3ddv);

	result = D3DXCreateTextureFromFileEx(
		d3ddv,
		FilePath,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 255, 0),
		&info,
		NULL,
		&_Image);

	if (result != D3D_OK)
	{
		trace(L"[ERROR] Failed to create texture from file '%s'", FilePath);
		return;
	}
}


void CSprite::Render(LPDIRECT3DSURFACE9 Target, int X, int Y, bool isRight)
{
	RECT srect;

	srect.left = (_Index % _SpritePerRow)*(_Width)+1;
	srect.top = (_Index / _SpritePerRow)*(_Height)+1;
	srect.right = srect.left + _Width;
	srect.bottom = srect.top + _Height + 1;

	D3DXVECTOR3 position((float)X - _Width / 2, (float)Y - _Height / 2, 0);
	//D3DXVECTOR3 center(0, 0, 0);

	if (isRight)
	{
		D3DXMATRIX		oldMt;
		_SpriteHandler->GetTransform(&oldMt);
		D3DXMATRIX		newMt;
		D3DXVECTOR2		center = D3DXVECTOR2((X + _Width / 2), (Y + _Height / 2));
		D3DXVECTOR2		rotate = D3DXVECTOR2(-1, 1);

		D3DXMatrixTransformation2D(&newMt, &center, 0.0f, &rotate, NULL, 0.0f, NULL);
		D3DXMATRIX		finalMt = newMt * oldMt;
		_SpriteHandler->SetTransform(&finalMt);

		_SpriteHandler->Draw(
			_Image,
			&srect,
			NULL,
			&position,
			D3DCOLOR_XRGB(255, 255, 255)
			);

		_SpriteHandler->SetTransform(&oldMt);
	}
	else
	{
		_SpriteHandler->Draw(
			_Image,
			&srect,
			NULL,
			&position,
			D3DCOLOR_XRGB(255, 255, 255)
			);
	}
}

void CSprite::Next()
{
	_Index = (_Index + _Count - 1) % _Count;
}

void CSprite::Reset()
{
	_Index = 0;
}

CSprite::~CSprite()
{
	_Image->Release();
}

void CSprite::flip(LPDIRECT3DSURFACE9 Target, int X, int Y)
{
	D3DXMATRIX		oldMt;
	_SpriteHandler->GetTransform(&oldMt);
	D3DXMATRIX		newMt;
	D3DXVECTOR2		center = D3DXVECTOR2((X + _Width / 2), (Y + _Height / 2));
	D3DXVECTOR2		rotate = D3DXVECTOR2(-1, 1);

	D3DXMatrixTransformation2D(&newMt, &center, 0.0f, &rotate, NULL, 0.0f, NULL);
	D3DXMATRIX		finalMt = newMt * oldMt;
	_SpriteHandler->SetTransform(&finalMt);

	this->Render(Target, X, Y);
	_SpriteHandler->SetTransform(&oldMt);
}