#include "stdafx.h"
#include "cCharacter.h"

cCharacter::cCharacter()
{
	D3DXMatrixIdentity(&m_matWorld);

	m_fRotY			= 0.0f;
	m_vPosition		= { 0, 0, 0 };
	m_vDirection	= { 0, 0, 1 };

	m_fMoveSpeed	= 0.005f;
	//m_vNextPosition = { 0, 0, 0 };

	m_isMoving		= false;
	m_isJumping		= false;
	m_fJumpPower	= 0.0f;
	m_fGravity		= -0.0005f;
}


cCharacter::~cCharacter()
{
}

void cCharacter::Setup(void)
{
}

void cCharacter::Update(iMap* pMap)
{
	m_pMap = pMap;


	if (g_pKeyManager->isStayKeyDown('A'))
	{
		m_fRotY -= 0.01f;
	}
	if (g_pKeyManager->isStayKeyDown('D'))
	{
		m_fRotY += 0.01f;
	}

	D3DXVECTOR3 vPosition = m_vPosition;


	if (g_pKeyManager->isStayKeyDown('W'))
	{
		vPosition = m_vPosition + m_vDirection * m_fMoveSpeed;
		m_isMoving = true;
	}
	else if (g_pKeyManager->isStayKeyDown('S'))
	{
		vPosition = m_vPosition - m_vDirection * m_fMoveSpeed;
		m_isMoving = true;
	}
	else
	{
		m_isMoving = false;
	}

	//if (g_pKeyManager->isOnceKeyDown(VK_SPACE))
	//{
	//	m_fJumpPower = 0.5f;
	//	m_isJumping = true;
	//}

	if (m_isJumping)
	{
		m_fJumpPower += m_fGravity;
		//m_vNextPosition.y = m_vPosition.y + m_fJumpPower;
	}



	D3DXMATRIXA16	matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);

	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(0, 0, 1), &matR);

	if (pMap)
	{
		if (pMap->GetHeight(vPosition.x, vPosition.y, vPosition.z))
		{
			m_vPosition = vPosition;
		}
	}
	else
	{
		m_vPosition = vPosition;
	}
	
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matR * matT;
}

void cCharacter::Render(void)
{
}

D3DXVECTOR3& cCharacter::GetPosition(void)
{
	return m_vPosition;
}

