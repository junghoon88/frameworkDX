#pragma once

class cFrame;
class cMtlTex;

class cAseLoader
{
public:
	cAseLoader();
	~cAseLoader();

private:
	FILE*					m_fp;
	char					m_szToken[1024];
	vector<cMtlTex*>		m_vecMtlTex;
	map<string, cFrame*>	m_mapFrame;

public:
	cFrame* Load(IN char* szFullPath);

private:
	char*		GetToken(void);
	int			GetInteger(void);
	float		GetFloat(void);
	bool		IsWhite(IN char c);
	bool		IsEqual(IN char* str1, IN char* str2);
	void		SkipBlock(void);
	void		ProcessMATERIAL_LIST(void);
	void		ProcessMATERIAL(OUT cMtlTex* pMtlTex);
	void		ProcessMAP_DIFFUSE(OUT cMtlTex* pMtlTex);
	cFrame*		ProcessGEOMOBJECT(void);
	void		ProcessMESH(OUT cFrame* pFrame);
	void		ProcessMESH_VERTEX_LIST(OUT vector<D3DXVECTOR3>& vecV);
	void		ProcessMESH_FACE_LIST(OUT vector<WORD>& vecVI);
	void		ProcessMESH_TVERTEX_LIST(OUT vector<D3DXVECTOR2>& vecT);
	void		ProcessMESH_TFACE_LIST(IN vector<WORD>& vecTI);
	void		ProcessMESH_NORMALS(OUT vector<D3DXVECTOR3>& vecN);
	void		ProcessNODE_TM(OUT cFrame* pFrame);

	//animation
	DWORD		m_dwFirstFrame;
	DWORD		m_dwLastFrame;
	DWORD		m_dwFrameSpeed;
	DWORD		m_dwTicksPerFrame;
	void		ProcessScene(void);
	void		Set_SceneFrame(OUT cFrame* pRoot);

	void		ProcessTM_ANIMATION(OUT cFrame* pFrame);
	void		ProcessCONTROL_POS_TRACK(OUT cFrame* pFrame);
	void		ProcessCONTROL_ROT_TRACK(OUT cFrame* pFrame);
};

