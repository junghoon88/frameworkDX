#include "stdafx.h"
#include "cFrame.h"


cFrame::cFrame()
	: m_pMtlTex(NULL)
	, m_pVB(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cFrame::~cFrame()
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pVB);
}

void cFrame::Setup(void)
{
}

void cFrame::Update(int nKeyFrame, D3DXMATRIXA16* pmatParent)
{
	D3DXMATRIXA16 matR, matT;

	CalcLocalR(nKeyFrame, matR);
	CalcLocalT(nKeyFrame, matT);

	m_matLocalTM = matR * matT;

	m_matWorldTM = m_matLocalTM;

	if (pmatParent)
	{
		m_matWorldTM *= (*pmatParent);
	}

	for each(auto c in m_vecChild)
	{
		c->Update(nKeyFrame, &m_matWorldTM);
	}
}

void cFrame::Render(void)
{
	if (m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);

#if USE_VERTEXBUFFER
		g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
		
#if 0 && USE_INDEXBUFFER

#else
#endif
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumVertex);

#else
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PNT_VERTEX));
#endif
	}

	for each(auto c in m_vecChild)
	{
		c->Render();
	}
}

void cFrame::AddChild(cFrame * pChild)
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy(void)
{
	for each(auto c in m_vecChild)
	{
		c->Destroy();
	}
	this->Release();
}

void cFrame::CalcOriginalLocalTM(D3DXMATRIXA16 * pmatParent)
{
	m_matLocalTM = m_matWorldTM;
	if (pmatParent)
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pmatParent);
		m_matLocalTM = m_matWorldTM * matInvParent;
	}

	for each(auto c in m_vecChild)
	{
		c->CalcOriginalLocalTM(&m_matWorldTM);
	}
}

void cFrame::CalcLocalT(IN int nKeyFrame, OUT D3DXMATRIXA16& matT)
{
	D3DXMatrixIdentity(&matT);
	if (m_vecPosTrack.empty())
	{
		matT._41 = m_matLocalTM._41;
		matT._42 = m_matLocalTM._42;
		matT._43 = m_matLocalTM._43;
		return;
	}

	if (nKeyFrame <= m_vecPosTrack.front().n)
	{
		//시작 프레임보다 작을때?
		matT._41 = m_vecPosTrack.front().v.x;
		matT._42 = m_vecPosTrack.front().v.y;
		matT._43 = m_vecPosTrack.front().v.z;
		return;
	}
	else if (nKeyFrame >= m_vecPosTrack.back().n)
	{
		matT._41 = m_vecPosTrack.back().v.x;
		matT._42 = m_vecPosTrack.back().v.y;
		matT._43 = m_vecPosTrack.back().v.z;
		return;
	}

	int nNextIndex = 0;
	for (size_t i = 0; i < m_vecPosTrack.size(); i++)
	{
		if (nKeyFrame < m_vecPosTrack[i].n)
		{
			nNextIndex = i;
			break;
		}
	}

	int nPrevIndex = nNextIndex - 1;
	float t = (nKeyFrame - m_vecPosTrack[nPrevIndex].n) / (float)(m_vecPosTrack[nNextIndex].n - m_vecPosTrack[nPrevIndex].n);

	D3DXVECTOR3 v;
	D3DXVec3Lerp(&v, &m_vecPosTrack[nPrevIndex].v, &m_vecPosTrack[nNextIndex].v, t);
	
	D3DXMatrixTranslation(&matT, v.x, v.y, v.z);
}

void cFrame::CalcLocalR(IN int nKeyFrame, OUT D3DXMATRIXA16& matR)
{
	D3DXMatrixIdentity(&matR);
	if (m_vecRotTrack.empty())
	{
		matR = m_matLocalTM;
		matR._41 = 0.0f;
		matR._42 = 0.0f;
		matR._43 = 0.0f;
		return;
	}

	if (nKeyFrame <= m_vecRotTrack.front().n)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.front().q);
		return;
	}
	else if (nKeyFrame >= m_vecRotTrack.back().n)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.back().q);
		return;
	}

	int nNextIndex = 0;
	for (size_t i = 0; i < m_vecRotTrack.size(); i++)
	{
		if (nKeyFrame < m_vecRotTrack[i].n)
		{
			nNextIndex = i;
			break;
		}
	}

	int nPrevIndex = nNextIndex - 1;
	float t = (nKeyFrame - m_vecRotTrack[nPrevIndex].n) / (float)(m_vecRotTrack[nNextIndex].n - m_vecRotTrack[nPrevIndex].n);

	D3DXQUATERNION q;
	D3DXQuaternionSlerp(&q, &m_vecRotTrack[nPrevIndex].q, &m_vecRotTrack[nNextIndex].q, t);

	D3DXMatrixRotationQuaternion(&matR, &q);

}

int cFrame::GetKeyFrame(void)
{
	int first = m_dwFirstFrame * m_dwTickPerFrame;
	int last = m_dwLastFrame * m_dwTickPerFrame;

	return GetTickCount() % (last - first) + first;
}

void cFrame::BuildVB(vector<ST_PNT_VERTEX>& vecVertex)
{
#if USE_VERTEXBUFFER
	if (vecVertex.size() == 0)
	{
		assert(false, "vertex buffer size = 0");
	}

	D3DXMATRIXA16	matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &m_matWorldTM);

	for (size_t i = 0; i < vecVertex.size(); i++)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}


	m_nNumVertex = vecVertex.size() / 3;
	g_pD3DDevice->CreateVertexBuffer(
		vecVertex.size() * sizeof(ST_PNT_VERTEX),
		0,//D3DUSAGE_DYNAMIC,
		ST_PNT_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL
	);

	ST_PNT_VERTEX* pV = NULL;
	m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
	//memcpy 를 사용하면 vector 가 동적배열이라 이상한 값이 들어갈 가능성이 있지 않을까?
	//memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	for (int i = 0; i < vecVertex.size(); i++)
	{
		pV[i] = vecVertex[i];
	}
	m_pVB->Unlock();

	for each (auto c in m_vecChild)
	{
		c->Setup();
	}

#endif

}

void cFrame::BuildVIB(vector<D3DXVECTOR3>& vecV, vector<WORD>& vecVI, 
						vector<D3DXVECTOR2>& vecT, vector<WORD>& vecTI, 
						vector<D3DXVECTOR3>& vecN)
{
	return;

	if (vecVertex.size() == 0)
	{
		assert(false, "vertex buffer size = 0");
	}

	if (vecT.size() == 0)
		return;

	vector<ST_PNT_VERTEX> vecVertex;
	vecVertex.resize(vecT);

	//IndexBuffer 를 사용하려면 텍스쳐에 대한 버텍스만 나열해야하는데
	//텍스쳐의 인덱스, 버텍스의 인덱스의 상관관계가 없음
	//버텍스, 인덱스를 각각 하나씩 통합해야 사용할 수 있을 듯...
	for (int i = 0; i < vecT.size(); i++)
	{
		//vecVertex[i].p = vecV[]
	}
}

