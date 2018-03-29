#pragma once
#include "cObject.h"
#include "cMtlTex.h"

class cFrame : public cObject
{
public:
	cFrame();
	~cFrame();

private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matLocalTM, LocalTM);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorldTM, WorldTM);
	SYNTHESIZE_PASS_BY_REF(vector<ST_POS_SAMPLE>, m_vecPosTrack, PosTrack);
	SYNTHESIZE_PASS_BY_REF(vector<ST_ROT_SAMPLE>, m_vecRotTrack, RotTrack);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

private:
	vector<cFrame*> m_vecChild;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	int							m_nNumVertex;
	int							m_nNumIndex;



public:
	void Setup(void);
	void Update(int nKeyFrame, D3DXMATRIXA16* pmatParent);
	void Render(void);
	void AddChild(cFrame* pChild);
	void Destroy(void);
	void CalcOriginalLocalTM(D3DXMATRIXA16* pmatParent);

public:
	DWORD m_dwFirstFrame;
	DWORD m_dwLastFrame;
	DWORD m_dwFrameSpeed;
	DWORD m_dwTickPerFrame;

	void CalcLocalT(IN int nKeyFrame, OUT D3DXMATRIXA16& matT);
	void CalcLocalR(IN int nKeyFrame, OUT D3DXMATRIXA16& matR);
	int GetKeyFrame(void);

public:
	void BuildVB(vector<ST_PNT_VERTEX>& vecVertex);
	void cFrame::BuildVIB(vector<D3DXVECTOR3>& vecV, vector<WORD>& vecVI,
		vector<D3DXVECTOR2>& vecT, vector<WORD>& vecTI,
		vector<D3DXVECTOR3>& vecN);
};

