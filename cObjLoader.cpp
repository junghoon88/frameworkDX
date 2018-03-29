#include "stdafx.h"
#include "cObjLoader.h"

#include "cGroup.h"
#include "cMtlTex.h"

cObjLoader::cObjLoader()
{
}


cObjLoader::~cObjLoader()
{
}

void cObjLoader::Load_jhkim(OUT vector<cGroup*>& vecGroup, IN char* szFolder, IN char* szFile)
{
	LoadMtlLib_jhkim(szFolder, szFile);

	char fileName[1000];
	char buffer[1000];

	strcpy(fileName, szFolder);
	strcat(fileName, "\\");
	strcat(fileName, szFile);
	strcat(fileName, ".obj");

	FILE* fp = NULL;
	fopen_s(&fp, fileName, "r");

	vector<D3DXVECTOR3> vecV;
	vector<D3DXVECTOR3> vecVn;
	vector<D3DXVECTOR2> vecVt;

	vector<ST_PNT_VERTEX> vecVertex;

	cGroup* g = new cGroup;


	while (fp)
	{
		if (feof(fp))
		{
			break;
		}

		fgets(buffer, 1000, fp);

		if (buffer[0] == '#')
			continue;


		if (buffer[0] == 'v')
		{
			if (buffer[1] == ' ')
			{
				//Vertex
				float p[3] = { 0.0f, 0.0f, 0.0f };
				int index = 0;

				char* token = strtok(buffer, " ");
				while (true)
				{
					token = strtok(NULL, " ");

					if (token == NULL) break;
					p[index++] = atoi(token);
				}
				vecV.push_back(D3DXVECTOR3(p[0], p[1], p[2]));
			}
			else if (buffer[1] == 't')
			{
				//Vertex Texture
				float t[3] = { 0.0f, 0.0f, 0.0f };
				int index = 0;

				char* token = strtok(buffer, " ");
				while (true)
				{
					token = strtok(NULL, " ");

					if (token == NULL) break;
					t[index++] = atof(token);
				}
				vecVt.push_back(D3DXVECTOR2(t[0], t[1]));
			}
			else if (buffer[1] == 'n')
			{
				//Vertex Normal
				float n[3] = { 0.0f, 0.0f, 0.0f };
				int index = 0;

				char* token = strtok(buffer, " ");
				while (true)
				{
					token = strtok(NULL, " ");

					if (token == NULL) break;
					n[index++] = atof(token);
				}
				vecVn.push_back(D3DXVECTOR3(n[0], n[1], n[2]));
			}
		}
		else if (buffer[0] == 'f')
		{
			char* token = strtok(buffer, " /");
			int vIndex[3] = { 0, 0, 0 };
			int i = 0;
			while (true)
			{
				token = strtok(NULL, " /");

				if (token == NULL) break;
				vIndex[i++] = atoi(token) - 1;
				
				if (i == 3)
				{
					ST_PNT_VERTEX v;
					v.p = vecV[vIndex[0]];
					v.t = vecVt[vIndex[1]];
					v.n = vecVn[vIndex[2]];

					vecVertex.push_back(v);

					i = 0;
				}
			}
		}
		else if (buffer[0] == 'u' && buffer[1] == 's' && buffer[2] == 'e'
				&& buffer[3] == 'm' && buffer[4] == 't' && buffer[5] == 'l')
		{
			char* token = strtok(buffer, " \n");
			token = strtok(NULL, " \n");

			map<string, cMtlTex*>::iterator iter = m_mapMtlTex.find(token);
			if (iter != m_mapMtlTex.end())
			{
				g->SetMtlTex((*iter).second);
			}
		}
	}

	g->SetVertex(vecVertex);

	vecGroup.push_back(g);
}

void cObjLoader::LoadMtlLib_jhkim(char* szFolder, char* szFile)
{
	char fileName[1000];
	char buffer[1000];

	cMtlTex* pMtl = new cMtlTex;

	string				MtlName = "";
	D3DMATERIAL9		stMtl;
	stMtl.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stMtl.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stMtl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stMtl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	stMtl.Power = 5.0f;

	LPDIRECT3DTEXTURE9	pTexture = NULL;


	strcpy(fileName, szFolder);
	strcat(fileName, "\\");
	strcat(fileName, szFile);
	strcat(fileName, ".mtl");

	FILE* fp = NULL;
	fopen_s(&fp, fileName, "r");

	while (fp)
	{
		if (feof(fp))
		{
			break;
		}

		fgets(buffer, 1000, fp);

		if (buffer[0] == '#')
			continue;

		if (buffer[0] == 'K')
		{
			if (buffer[1] == 'a')
			{
				float a[3] = { 0.0f, 0.0f, 0.0f };
				int index = 0;

				char* token = strtok(buffer, " ");
				while (true)
				{
					token = strtok(NULL, " ");

					if (token == NULL) break;
					a[index++] = atof(token);
				}

				stMtl.Ambient = D3DXCOLOR(a[0], a[1], a[2], 1.0f);
			}
			else if (buffer[1] == 'd')
			{
				float d[3] = { 0.0f, 0.0f, 0.0f };
				int index = 0;

				char* token = strtok(buffer, " ");
				while (true)
				{
					token = strtok(NULL, " ");

					if (token == NULL) break;
					d[index++] = atof(token);
				}

				stMtl.Diffuse = D3DXCOLOR(d[0], d[1], d[2], 1.0f);
			}
			else if (buffer[1] == 's')
			{
				float s[3] = { 0.0f, 0.0f, 0.0f };
				int index = 0;

				char* token = strtok(buffer, " ");
				while (true)
				{
					token = strtok(NULL, " ");

					if (token == NULL) break;
					s[index++] = atof(token);
				}

				stMtl.Specular = D3DXCOLOR(s[0], s[1], s[2], 1.0f);
			}
		}
		else if (buffer[0] == 'd')
		{
			char* token = strtok(buffer, " ");
			token = strtok(NULL, " ");

			stMtl.Power = atof(token);
		}
		else if (buffer[0] == 'm' && buffer[1] == 'a' && buffer[2] == 'p' 
				&& buffer[3] == '_' && buffer[4] == 'K' && buffer[5] == 'd')
		{
			char* token = strtok(buffer, " \n");
			token = strtok(NULL, " \n");

			strcpy(fileName, szFolder);
			strcat(fileName, "\\");
			strcat(fileName, token);


			// MULTIBYTE CODE - > UNICODE
			TCHAR strWChar[1000];
			MultiByteToWideChar(CP_ACP, NULL, fileName, -1, strWChar, 1000);
			D3DXCreateTextureFromFile(g_pD3DDevice, strWChar, &pTexture);
		}
		else if (buffer[0] == 'n' && buffer[1] == 'e' && buffer[2] == 'w'
			&& buffer[3] == 'm' && buffer[4] == 't' && buffer[5] == 'l')
		{
			char* token = strtok(buffer, " \n");
			token = strtok(NULL, " \n");

			MtlName = token;
		}
	}

	if (MtlName != "" && pTexture != NULL)
	{
		pMtl->SetMaterial(stMtl);
		pMtl->SetTexture(pTexture);

		m_mapMtlTex.insert(make_pair(MtlName, pMtl));
	}
}


void cObjLoader::Load(OUT vector<cGroup*>& vecGroup, IN char* szFolder, IN char* szFile)
{
	vector<D3DXVECTOR3> vecV;
	vector<D3DXVECTOR3> vecVn;
	vector<D3DXVECTOR2> vecVt;

	vector<ST_PNT_VERTEX> vecVertex;

	string sFullPath = string(szFolder);
	sFullPath += string("/") + string(szFile);

	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	string sMtlName;


	while (fp)
	{
		if (feof(fp))
		{
			break;
		}

		char szTemp[1024];
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '#')
			continue;
		else if (szTemp[0] == 'm')
		{
			char szMtlFile[1024];
			//%*s 는 무시함
			sscanf_s(szTemp, "%*s %s", szMtlFile, 1024); //1024 를 넣어줘야하나? 
			LoadMtlLib(szFolder, szMtlFile);
		}
		else if (szTemp[0] == 'g')
		{
			if (!vecVertex.empty())
			{
				cGroup* pGroup = new cGroup;
				pGroup->SetVertex(vecVertex);
				pGroup->SetMtlTex(m_mapMtlTex[sMtlName]);
				vecGroup.push_back(pGroup);
				vecVertex.clear();
			}
		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == ' ')
			{
				//Vertex
				float x, y, z;
				sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
			else if (szTemp[1] == 't')
			{
				//Vertex Texture
				float u, v;
				sscanf_s(szTemp, "%*s %f %f %*f", &u, &v);
				vecVt.push_back(D3DXVECTOR2(u, v));
			}
			else if (szTemp[1] == 'n')
			{
				//Vertex Normal
				float x, y, z;
				sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecVn.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szTemp[0] == 'u')
		{
			char szMtlName[1024];
			sscanf_s(szTemp, "%*s %s", szMtlName, 1024);
			sMtlName = string(szMtlName);
		}
		else if (szTemp[0] == 'f')
		{
			int nIndex[3][3];
			sscanf_s(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&nIndex[0][0], &nIndex[0][1], &nIndex[0][2],
				&nIndex[1][0], &nIndex[1][1], &nIndex[1][2],
				&nIndex[2][0], &nIndex[2][1], &nIndex[2][2]);

			for (int i = 0; i < 3; i++)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[ nIndex[i][0] - 1];
				v.t = vecVt[nIndex[i][1] - 1];
				v.n = vecVn[nIndex[i][2] - 1];

				vecVertex.push_back(v);
			}
		}
	}

	fclose(fp);

	for each(auto it in m_mapMtlTex)
	{
		SAFE_RELEASE(it.second);
	}
	m_mapMtlTex.clear();
}

void cObjLoader::LoadMtlLib(char * szFolder, char * szFile)
{
	string sFullPath(szFolder);
	sFullPath += string("/") + string(szFile);

	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	string sMtlName;

	while (true)
	{
		if (feof(fp)) break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'n')
		{
			char szMtlName[1024];
			sscanf_s(szTemp, "%*s %s", szMtlName, 1024);
			sMtlName = string(szMtlName);
			if (m_mapMtlTex.find(sMtlName) == m_mapMtlTex.end())
			{
				m_mapMtlTex[sMtlName] = new cMtlTex;
			}
		}
		else if (szTemp[0] == 'K')
		{
			if (szTemp[1] == 'a')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				m_mapMtlTex[sMtlName]->GetMaterial().Ambient = D3DXCOLOR(r, g, b, 1.0f);
			}
			if (szTemp[1] == 'd')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				m_mapMtlTex[sMtlName]->GetMaterial().Diffuse = D3DXCOLOR(r, g, b, 1.0f);
			}
			if (szTemp[1] == 's')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				m_mapMtlTex[sMtlName]->GetMaterial().Specular = D3DXCOLOR(r, g, b, 1.0f);
			}
		}
		else if (szTemp[0] == 'd')
		{
			float power;
			sscanf_s(szTemp, "%*s %f", &power);
			m_mapMtlTex[sMtlName]->GetMaterial().Power = power;
		}
		else if (szTemp[0] == 'm')
		{
			char szTexFile[1024];
			sscanf_s(szTemp, "%*s %s", szTexFile, 1024);
			sFullPath = string(szFolder);
			sFullPath += string("/") + string(szTexFile);

			m_mapMtlTex[sMtlName]->SetTexture(g_pTextureManager->GetTexture(sFullPath));
		}
	}
}

void cObjLoader::LoadSurface(OUT vector<D3DXVECTOR3>& vecSurface, IN char * szFolder, IN char * szFile, IN D3DXMATRIXA16 * pMat)
{
	vector<D3DXVECTOR3> vecV;
	string sFullPath = string(szFolder);
	sFullPath += string("/") + string(szFile);

	FILE* fp = NULL;
	fopen_s(&fp, sFullPath.c_str(), "r");
	string sMtlName;

	while (true)
	{
		if (feof(fp)) break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'm')
		{
			continue;
		}
		else if (szTemp[0] == 'g')
		{
			continue;
		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == ' ')
			{
				float x, y, z;
				sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
			else if (szTemp[1] == 't')
			{
				continue;
			}
			else if (szTemp[1] == 'n')
			{
				continue;
			}
		}
		else if (szTemp[0] == 'u')
		{

		}
		else if (szTemp[0] == 'f')
		{
			int nIndex[3];
			sscanf_s(szTemp, "%*s %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
				&nIndex[0], &nIndex[1], &nIndex[2]);

			for (int i = 0; i < 3; i++)
			{
				vecSurface.push_back(vecV[nIndex[i]-1]);
			}
		}
	}

	fclose(fp);

	if (pMat)
	{
		for (size_t i = 0; i < vecSurface.size(); ++i)
		{
			D3DXVec3TransformCoord(&vecSurface[i], &vecSurface[i], pMat);
		}
	}



}
