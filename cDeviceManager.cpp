#include "stdafx.h"
#include "cDeviceManager.h"


cDeviceManager::cDeviceManager()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	// : m_pD3D create
	// : GetDeviceCaps(...)
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DCAPS9 caps;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		//하드웨어 버텍스 프로세싱 이용 가능
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}


	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = rc.right;
	d3dpp.BackBufferHeight = rc.bottom;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	// :

	// CreateDevice
	HRESULT hr = m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	);

	if (FAILED(hr))
	{
		//::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
		//return 0;
	}

}


cDeviceManager::~cDeviceManager()
{
}

LPDIRECT3DDEVICE9 cDeviceManager::GetDevice(void)
{
	return m_pD3DDevice;
}

void cDeviceManager::Destroy(void)
{
	ULONG ul = m_pD3DDevice->Release();
#ifdef _DEBUG
	assert(ul == 0,  "디바이스 이용 생성된 객체중 해제되지 않은 객체가 있음");
#endif
	//SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3D);
}
