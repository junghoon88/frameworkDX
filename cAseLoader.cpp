#include "stdafx.h"
#include "cAseLoader.h"

#include "Asciitok.h"
#include "cMtlTex.h"
#include "cFrame.h"

cAseLoader::cAseLoader()
	: m_fp(NULL)
{
}


cAseLoader::~cAseLoader()
{
}

cFrame* cAseLoader::Load(IN char* szFullPath)
{
	cFrame* pRoot = NULL;
	fopen_s(&m_fp, szFullPath, "r");

	while (char* szToken = GetToken())
	{
		if (IsEqual(szToken, ID_SCENE))
		{
			ProcessScene();
		}
		else if (IsEqual(szToken, ID_MATERIAL_LIST))
		{
			ProcessMATERIAL_LIST();
		}
		else if (IsEqual(szToken, ID_GEOMETRY))
		{
			cFrame* pFrame = ProcessGEOMOBJECT();
			if (pRoot == NULL)
			{
				pRoot = pFrame;
				Set_SceneFrame(pRoot);
			}
		}
	}

	fclose(m_fp);

	for each(auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}

	pRoot->CalcOriginalLocalTM(NULL);


	return pRoot;
}

char* cAseLoader::GetToken(void)
{
	int nReadCnt = 0;
	bool isQuote = false;

	while (true)
	{
		char c = fgetc(m_fp);

		if (feof(m_fp)) break;

		if (c == '\"')
		{
			if (isQuote) break;
			isQuote = true;
			continue;
		}

		if (!isQuote && IsWhite(c))
		{
			if (nReadCnt == 0)
				continue;
			else
				break;
		}

		m_szToken[nReadCnt++] = c;
	}

	if (nReadCnt == 0)
		return NULL;

	m_szToken[nReadCnt] = '\0';

	return m_szToken;
}

int cAseLoader::GetInteger(void)
{
	return atoi(GetToken());
}

float cAseLoader::GetFloat(void)
{
	return (float)atof(GetToken());
}

bool cAseLoader::IsWhite(IN char c)
{
	return (c < 33);
}

bool cAseLoader::IsEqual(IN char* str1, IN char* str2)
{
	return !strcmp(str1, str2);
}

void cAseLoader::SkipBlock(void)
{
	int nLevel = 0;//괄호 개수
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL_LIST(void)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MATERIAL_COUNT))
		{
			//초기화
			for each(auto p in m_vecMtlTex)
			{
				SAFE_RELEASE(p);
			}

			m_vecMtlTex.resize(GetInteger());
		}
		else if (IsEqual(szToken, ID_MATERIAL))
		{
			int nIndex = GetInteger();
			m_vecMtlTex[nIndex] = new cMtlTex;
			ProcessMATERIAL(m_vecMtlTex[nIndex]);
		}
	}
	while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL(OUT cMtlTex* pMtlTex)
{
	int nLevel = 0;

	D3DMATERIAL9	stMtl;
	ZeroMemory(&stMtl, sizeof(D3DMATERIAL9));

	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_AMBIENT))
		{
			stMtl.Ambient.r = GetFloat();
			stMtl.Ambient.g = GetFloat();
			stMtl.Ambient.b = GetFloat();
			stMtl.Ambient.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_DIFFUSE))
		{
			stMtl.Diffuse.r = GetFloat();
			stMtl.Diffuse.g = GetFloat();
			stMtl.Diffuse.b = GetFloat();
			stMtl.Diffuse.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_SPECULAR))
		{
			stMtl.Specular.r = GetFloat();
			stMtl.Specular.g = GetFloat();
			stMtl.Specular.b = GetFloat();
			stMtl.Specular.a = 1.0f;
		}
		else if (IsEqual(szToken, ID_MAP_DIFFUSE))
		{
			ProcessMAP_DIFFUSE(pMtlTex);
		}
	} while (nLevel > 0);

	pMtlTex->SetMaterial(stMtl);
}

void cAseLoader::ProcessMAP_DIFFUSE(OUT cMtlTex* pMtlTex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_BITMAP))
		{
			szToken = GetToken();
			pMtlTex->SetTexture(g_pTextureManager->GetTexture(szToken));
		}
	} while (nLevel > 0);

}

cFrame* cAseLoader::ProcessGEOMOBJECT(void)
{
	int nLevel = 0;

	cFrame* pFrame = new cFrame;

	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_NODE_NAME))
		{
			m_mapFrame[GetToken()] = pFrame;
		}
		else if (IsEqual(szToken, ID_NODE_PARENT))
		{
			m_mapFrame[GetToken()]->AddChild(pFrame);
		}
		else if (IsEqual(szToken, ID_NODE_TM))
		{
			ProcessNODE_TM(pFrame);
		}
		else if (IsEqual(szToken, ID_MESH))
		{
			ProcessMESH(pFrame);
		}
		else if (IsEqual(szToken, ID_TM_ANIMATION))
		{
			ProcessTM_ANIMATION(pFrame);
		}
		else if (IsEqual(szToken, ID_MATERIAL_REF))
		{
			int nMtlIndex = GetInteger();
			pFrame->SetMtlTex(m_vecMtlTex[nMtlIndex]);
		}
	} while (nLevel > 0);

	return pFrame;
}

void cAseLoader::ProcessMESH(OUT cFrame* pFrame)
{
	//버텍스
	vector<D3DXVECTOR3>		vecV;
	vector<WORD>			vecVI;
	//텍스쳐
	vector<D3DXVECTOR2>		vecT;
	vector<WORD>			vecTI;
	//법선
	vector<D3DXVECTOR3>		vecN;

	vector<ST_PNT_VERTEX>	vecVertex;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_NUMVERTEX))
		{
			int nNumVertex = GetInteger();
			vecV.resize(nNumVertex);
			vecN.resize(nNumVertex);
		}
		else if (IsEqual(szToken, ID_MESH_NUMFACES))
		{
			int nNumFace = GetInteger();
			vecVI.resize(nNumFace * 3);
			vecVertex.resize(nNumFace * 3);
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX_LIST))
		{
			ProcessMESH_VERTEX_LIST(vecV);
		}
		else if (IsEqual(szToken, ID_MESH_FACE_LIST))
		{
			ProcessMESH_FACE_LIST(vecVI);
		}
		//Texture
		else if (IsEqual(szToken, ID_MESH_NUMTVERTEX))
		{
			vecT.resize(GetInteger());
		}
		else if (IsEqual(szToken, ID_MESH_NUMTVFACES))
		{
			vecTI.resize(GetInteger() * 3);
		}
		else if (IsEqual(szToken, ID_MESH_TVERTLIST))
		{
			ProcessMESH_TVERTEX_LIST(vecT);
		}
		else if (IsEqual(szToken, ID_MESH_TFACELIST))
		{
			ProcessMESH_TFACE_LIST(vecTI);
		}
		else if (IsEqual(szToken, ID_MESH_NORMALS))
		{
			ProcessMESH_NORMALS(vecN);
		}
	} while (nLevel > 0);


	if (vecVI.size() < vecTI.size())
	{
		assert(false, "버텍스보다 텍스쳐가 많음");
	}

	for (int i = 0; i < vecVertex.size(); i++)
	{
		vecVertex[i].p = vecV[vecVI[i]];
		vecVertex[i].n = vecN[vecVI[i]];
		
		if (vecT.empty()) continue;
		vecVertex[i].t = vecT[vecTI[i]];
	}


	//World 좌표-> Local 좌표로 변환하기 위해서
	//World 의 역행렬을 곱함
	//D3DXMATRIXA16	matInvWorld;
	//D3DXMatrixInverse(&matInvWorld, 0, &pFrame->GetWorldTM());
	//
	//for (size_t i = 0; i < vecVertex.size(); i++)
	//{
	//	D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
	//	D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	//}
	
	//pFrame->SetVertex(vecVertex);
	pFrame->BuildVB(vecVertex);


}

void cAseLoader::ProcessMESH_VERTEX_LIST(OUT vector<D3DXVECTOR3>& vecVertex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX))
		{
			int nIndex = GetInteger();
			vecVertex[nIndex].x = GetFloat();
			vecVertex[nIndex].z = GetFloat();
			vecVertex[nIndex].y = GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_FACE_LIST(OUT vector<WORD>& vecIndex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_FACE))
		{
			int nFaceIndex = GetInteger();
			GetToken();//A:
			vecIndex[nFaceIndex  * 3 + 0] = GetInteger();
			GetToken();//B:
			vecIndex[nFaceIndex  * 3 + 2] = GetInteger();
			GetToken();//C:
			vecIndex[nFaceIndex  * 3 + 1] = GetInteger();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TVERTEX_LIST(OUT vector<D3DXVECTOR2>& vecVertexTexture)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_TVERT))
		{
			int nIndex = GetInteger();
			vecVertexTexture[nIndex].x = GetFloat();
			vecVertexTexture[nIndex].y = 1.0f - GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TFACE_LIST(IN vector<WORD>& vecIndexTexture)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_TFACE))
		{
			int nFaceIndex = GetInteger();
			//GetToken();//A:
			vecIndexTexture[nFaceIndex * 3 + 0] = GetInteger();
			//GetToken();//B:
			vecIndexTexture[nFaceIndex * 3 + 2] = GetInteger();
			//GetToken();//C:
			vecIndexTexture[nFaceIndex * 3 + 1] = GetInteger();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_NORMALS(OUT vector<D3DXVECTOR3>& vecVertex)
{
	int nFaceIndex = 0;
	int aModIndex[3] = { 0, 2, 1 };
	int nModCount = 0;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_FACENORMAL))
		{
			nFaceIndex = GetInteger();
			nModCount = 0;
		}
		else if (IsEqual(szToken, ID_MESH_VERTEXNORMAL))
		{
			int index = GetInteger();
			D3DXVECTOR3 n;
			n.x = GetFloat();
			n.z = GetFloat();
			n.y = GetFloat();
			vecVertex[index] = n; //?
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessNODE_TM(OUT cFrame* pFrame)
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld); 

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_TM_ROW0))
		{
			matWorld._11 = GetFloat();
			matWorld._13 = GetFloat();
			matWorld._12 = GetFloat();
			//matWorld._14 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW1))
		{
			matWorld._31 = GetFloat();
			matWorld._33 = GetFloat();
			matWorld._32 = GetFloat();
			//matWorld._34 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW2))
		{
			matWorld._21 = GetFloat();
			matWorld._23 = GetFloat();
			matWorld._22 = GetFloat();
			//matWorld._24 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW3))
		{
			matWorld._41 = GetFloat();
			matWorld._43 = GetFloat();
			matWorld._42 = GetFloat();
			//matWorld._44 = 1.0f;
		}
	} while (nLevel > 0);

	pFrame->SetWorldTM(matWorld);
}

void cAseLoader::ProcessScene(void)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_FIRSTFRAME))
		{
			m_dwFirstFrame = GetInteger();
		}
		else if (IsEqual(szToken, ID_LASTFRAME))
		{
			m_dwLastFrame = GetInteger();
		}
		else if (IsEqual(szToken, ID_FRAMESPEED))
		{
			m_dwFrameSpeed = GetInteger();
		}
		else if (IsEqual(szToken, ID_TICKSPERFRAME))
		{
			m_dwTicksPerFrame = GetInteger();
		}
	} while (nLevel > 0);
}

void cAseLoader::Set_SceneFrame(OUT cFrame* pRoot)
{
	pRoot->m_dwFirstFrame   = m_dwFirstFrame;
	pRoot->m_dwLastFrame    = m_dwLastFrame;
	pRoot->m_dwFrameSpeed   = m_dwFrameSpeed;
	pRoot->m_dwTickPerFrame = m_dwTicksPerFrame;
}

void cAseLoader::ProcessTM_ANIMATION(OUT cFrame * pFrame)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_POS_TRACK))
		{
			ProcessCONTROL_POS_TRACK(pFrame);
		}
		else if (IsEqual(szToken, ID_ROT_TRACK))
		{
			ProcessCONTROL_ROT_TRACK(pFrame);
		}
	} while (nLevel > 0);

}

void cAseLoader::ProcessCONTROL_POS_TRACK(OUT cFrame * pFrame)
{
	vector<ST_POS_SAMPLE> vecPosTrack;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_POS_SAMPLE))
		{
			ST_POS_SAMPLE pos;
			pos.n = GetInteger();
			pos.v.x = GetFloat();
			pos.v.z = GetFloat();
			pos.v.y = GetFloat();
			vecPosTrack.push_back(pos);
		}
	} while (nLevel > 0);
	pFrame->SetPosTrack(vecPosTrack);
}

void cAseLoader::ProcessCONTROL_ROT_TRACK(OUT cFrame * pFrame)
{
	vector<ST_ROT_SAMPLE> vecRotTrack;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_ROT_SAMPLE))
		{
			ST_ROT_SAMPLE rot;
			rot.n = GetInteger();
			rot.q.x = GetFloat();
			rot.q.z = GetFloat();
			rot.q.y = GetFloat();
			rot.q.w = GetFloat();

			//축을 되돌린다.? (복소수..)
			rot.q.x *= sinf(rot.q.w / 2.0f);
			rot.q.y *= sinf(rot.q.w / 2.0f);
			rot.q.z *= sinf(rot.q.w / 2.0f);
			rot.q.w  = cosf(rot.q.w / 2.0f);

			//이전값이랑 누적회전?
			if (!vecRotTrack.empty())
			{
				rot.q = vecRotTrack.back().q * rot.q;
			}
			
			vecRotTrack.push_back(rot);
		}
	} while (nLevel > 0);
	pFrame->SetRotTrack(vecRotTrack);
}
