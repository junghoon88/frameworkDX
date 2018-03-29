#include "stdafx.h"
#include "cMainGame.h"

#include "cCamera.h"
#include "cLight.h"

//새로운 콘솔창을 띄워서 printf, cout 으로 출력가능함
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")


cMainGame::cMainGame()
	: m_pCamera(NULL) 
	, m_pLight(NULL)
{
}


cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pLight);
}

void cMainGame::Setup(void)
{
	//Camera
	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL);
	//m_pCamera->Setup(&m_pCubeMan->GetPosition());

	m_pLight = new cLight;
	m_pLight->Setup();
}

void cMainGame::Update(void)
{

	//Camera
	if (m_pCamera)	m_pCamera->Update();
}

void cMainGame::Render(void)
{
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(45, 120, 110), 1.0f, 0);
	g_pD3DDevice->BeginScene();


	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(0, 0, 0, 0);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}
