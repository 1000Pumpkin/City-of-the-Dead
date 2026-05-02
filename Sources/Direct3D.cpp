#include "Core.h"

HRESULT Direct3D::InitD3D(HWND hWnd, int screenW, int screenH)
{
	m_hWnd = hWnd;
	screenWidth = screenW;
	screenHeight = screenH;

#ifdef _DEBUG
	if (!AllocConsole())
		return E_FAIL;

	FILE* fDummy;
	freopen_s(&fDummy, "CON", "r", stdin);
	freopen_s(&fDummy, "CON", "w", stdout);
	freopen_s(&fDummy, "CON", "w", stderr);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG


	// Create the D3D object.
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice. Since we are now
	// using more complex geometry, we will create a device with a zbuffer.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = screenWidth;
	d3dpp.BackBufferHeight = screenHeight;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Create the D3DDevice
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &pd3dDevice)))
	{
		return E_FAIL;
	}

	// Create the vertex buffer.
	if (FAILED(pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED, &pVB, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(D3DXCreateLine(pd3dDevice, &pLine)))
	{
		return E_FAIL;
	}

	if (FAILED(D3DXCreateFont(pd3dDevice, 0, 0, FW_DONTCARE, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"¸¼Àº °íµñ", &pFont)))
	{
		return E_FAIL;
	}

	// Turn off culling
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Turn off D3D lighting
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Turn on the zbuffer
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	curTime = prevTime = timeGetTime();

	SceneManager::GetInstance().AddScene(L"Loading", std::shared_ptr<Scene>(new LoadingScene));
	SceneManager::GetInstance().ChangeScene(L"Loading");

	return S_OK;
}

void Direct3D::CleanUp()
{
	if (pVB != NULL)
		pVB->Release();

	if (pd3dDevice != NULL)
		pd3dDevice->Release();

	if (pD3D != NULL)
		pD3D->Release();

	if (pLine != NULL)
		pLine->Release();

	if (pFont != NULL)
		pFont->Release();

#ifdef _DEBUG
	FreeConsole();
#endif // _DEBUG

}

void Direct3D::GameLoop()
{
	curTime = timeGetTime();

	auto dt = curTime - prevTime;
	deltaTime = dt / 1000.0f;

	fpsTotalTime += deltaTime;
	if (fpsTotalTime >= 1.0f)
	{
		fpsTotalTime = 0.0f;
		totalFps = fps;
		fps = 0;
	}
	++fps;

	KeyManager::GetInstance().Update(deltaTime);

	fixedTotalTime += deltaTime;
	if (fixedTotalTime > 2.0f)
		fixedTotalTime = 2.0f;
	while (fixedTotalTime > fixedDeltaTime)
	{
		fixedTotalTime -= fixedDeltaTime;

		SceneManager::GetInstance().FixedUpdate(fixedDeltaTime);

		camPos += (camDestPos - camPos) * 5.0f * fixedDeltaTime;
		camScale += (camDestScale - camScale) * 10.0f * fixedDeltaTime;
		camQuaken += (camDestQuaken - camQuaken) * 10.0f * fixedDeltaTime;
	} 

	SceneManager::GetInstance().Update(deltaTime);

	renderTotalTime += deltaTime;
	if (renderTotalTime >= renderTime)
	{
		// Clear the backbuffer and the zbuffer	
		pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		// Begin the scene
		if (SUCCEEDED(pd3dDevice->BeginScene()))
		{
			D3DXMatrixOrthoLH(&matProj, (float)resolutionWidth, (float)resolutionHeight, 0.1f, 1000.0f);
			D3DXMATRIX matCamScale, matCamPos;
			D3DXVECTOR2 sign(rand() % 2 == 0 ? -1.0f : 1.0f, rand() % 2 == 0 ? -1.0f : 1.0f);
			D3DXMatrixScaling(&matCamScale, camScale.x, camScale.y, 1.0f);
			D3DXMatrixTranslation(&matCamPos, -camPos.x + sign.x * camQuaken.x, -camPos.y + sign.y * camQuaken.y, 0.0f);
			matProj = matCamPos * matCamScale * matProj;
			pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
			SceneManager::GetInstance().Render();

			D3DXMatrixOrthoLH(&matProj, (float)resolutionWidth, (float)resolutionHeight, 0.1f, 1000.0f);
			pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
			SceneManager::GetInstance().UiRender();
			KeyManager::GetInstance().UiRender();
#ifdef _DEBUG
			WCHAR buffer[255];
			swprintf_s(buffer, L"Fps : %d DeltaTime : %f", totalFps, deltaTime);
			RECT rc = { 0.0f, 0.0f, 0.0f, 0.0f };
			pFont->DrawTextW(NULL, buffer, -1, &rc, DT_NOCLIP, 0xffffffff);
#endif // _DEBUG

			// End the scene
			pd3dDevice->EndScene();
		}

		// Present the backbuffer contents to the display
		pd3dDevice->Present(NULL, NULL, NULL, NULL);

		renderTotalTime = 0.0f;
	}

	prevTime = curTime;
}

void Direct3D::DrawLine(const D3DXVECTOR2& p1, const D3DXVECTOR2& p2)
{
	pLine->SetWidth(1.0f);
	pLine->SetAntialias(true);
	pLine->Begin();
	auto lines = { D3DXVECTOR3(p1.x, p1.y, 0.0f), D3DXVECTOR3(p2.x, p2.y, 0.0f) };
	D3DXMATRIX matLine = matView * matProj;
	pLine->DrawTransform(lines.begin(), lines.size(), &matLine, 0xffff0000);
	pLine->End();
}
