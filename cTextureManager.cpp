#include "stdafx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{

}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(char* szFullPath)
{
	if (m_mapTexture.find(szFullPath) == m_mapTexture.end())
	{
		D3DXCreateTextureFromFileA(g_pD3DDevice, szFullPath, &m_mapTexture[szFullPath]);

		//TCHAR strWChar[1000];
		//MultiByteToWideChar(CP_ACP, NULL, szFullPath, -1, strWChar, 1000);
		//D3DXCreateTextureFromFile(g_pD3DDevice, strWChar, &m_mapTexture[szFullPath]);
	}

	return m_mapTexture[szFullPath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(string& sFullPath)
{
	return GetTexture((char*)sFullPath.c_str());
}

void cTextureManager::Destroy(void)
{
	//auto 로 디버깅으로 볼땐 NULL로 안보임;
	for each(auto it in m_mapTexture)
	{
		SAFE_RELEASE(it.second);
	}
	m_mapTexture.clear();

	//map<string, LPDIRECT3DTEXTURE9>::iterator iter = m_mapTexture.begin();
	//for (; iter != m_mapTexture.end(); ++iter)
	//{
	//	SAFE_RELEASE((*iter).second);
	//}
	//m_mapTexture.clear();
}
