#pragma once

// A structure for our custom vertex type. We added texture coordinates
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position; // The position
	D3DCOLOR color;    // The color
	D3DXVECTOR2 uv;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class Direct3D
{
private:
	Direct3D() {};
	Direct3D(const Direct3D&) = delete;
	void operator =(const Direct3D&) = delete;

public:
	static Direct3D& GetInstance()
	{
		static Direct3D instance;
		return instance;
	}

public:
	LPDIRECT3D9             pD3D = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       pd3dDevice = NULL; // Our rendering device
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL; // Buffer to hold vertices
	LPD3DXLINE pLine = NULL;
	LPD3DXFONT pFont = NULL;

	HWND m_hWnd = NULL;

	int screenWidth = 0, screenHeight = 0;
	int resolutionWidth = 640, resolutionHeight = 360;

	DWORD curTime = 0;
	DWORD prevTime = 0;
	float deltaTime = 0.0f, fixedDeltaTime = 1.0f / 60.0f, renderTime = 1.0f / 60.0f;
	float fixedTotalTime = 0.0f, renderTotalTime = 0.0f;

	int totalFps = 0, fps = 0;
	float fpsTotalTime = 0.0f;

	bool isCamHandling = false;
	D3DXVECTOR2 camPos = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 camScale = D3DXVECTOR2(1.0f, 1.0f);
	D3DXVECTOR2 camQuaken = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 camDestPos = camPos;
	D3DXVECTOR2 camDestScale = camScale;
	D3DXVECTOR2 camDestQuaken = camQuaken;

	D3DXMATRIX matProj, matView;

public:
	HRESULT InitD3D(HWND hWnd, int screenW, int screenH);
	void CleanUp();
	void GameLoop();
	void DrawLine(const D3DXVECTOR2& p1, const D3DXVECTOR2& p2);
};