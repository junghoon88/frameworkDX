#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
	: m_vEye(0, 5, -5), m_vLookAt(0, 0, 0), m_vUp(0, 1, 0), m_pvTarget(NULL)
	, m_ptPrevMouse({ 0, 0 }), m_isLButtonDown(false), m_fCameraDistance(5.0f), m_vCamRotAngle(0, 0, 0)
{

}


cCamera::~cCamera()
{
}

void cCamera::Setup(D3DXVECTOR3 * pvTarget)
{
	m_pvTarget = pvTarget;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)rc.right / (float)rc.bottom, 1.0f, 1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

}

void cCamera::Update(void)
{
	// : camera position, rotation
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMATRIXA16 matR, matRX, matRY;
	D3DXMatrixRotationX(&matRX, m_vCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, m_vCamRotAngle.y);

	matR = matRX * matRY;

	m_vEye = D3DXVECTOR3(0, m_fCameraDistance, -m_fCameraDistance);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);

	if (m_pvTarget)
	{
		m_vLookAt = *m_pvTarget;
		m_vEye = m_vEye + *m_pvTarget;
	}

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);

	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void cCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// : event
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		m_isLButtonDown = true;
		break;

	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;

	case WM_MOUSEWHEEL:
		m_fCameraDistance += -(GET_WHEEL_DELTA_WPARAM(wParam)) / 120.0f;

		if (m_fCameraDistance < 3.0f)
		{
			m_fCameraDistance = 3.0f;
		}
		else if (m_fCameraDistance  > 30.0f)
		{
			m_fCameraDistance  = 30.0f;
		}
		break;

	case WM_MOUSEMOVE:
		if (m_isLButtonDown)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);

			float fDeltaX = (float)(ptCurrMouse.x - m_ptPrevMouse.x);
			float fDeltaY = (float)(ptCurrMouse.y - m_ptPrevMouse.y);
			m_vCamRotAngle.y += (fDeltaX / 100.0f);
			m_vCamRotAngle.x += (fDeltaY / 100.0f);

			//���ڵ�
#if 1
			if (m_vCamRotAngle.x > D3DX_PI / 4.0f)
			{
				m_vCamRotAngle.x -= fDeltaY / 100.0f;
			}
			else if (m_vCamRotAngle.x < -D3DX_PI / 4.0f)
			{
				m_vCamRotAngle.x -= fDeltaY / 100.0f;
			}
#else
			//���ڵ�
			if (m_vCamRotAngle.x >   D3DX_PI / 2.0f - 0.001f)
				m_vCamRotAngle.x =   D3DX_PI / 2.0f - 0.001f;
			if (m_vCamRotAngle.x < -(D3DX_PI / 2.0f - 0.001f))
				m_vCamRotAngle.x = -(D3DX_PI / 2.0f - 0.001f);
#endif

			m_ptPrevMouse = ptCurrMouse;
		}
		break;
	}
}
