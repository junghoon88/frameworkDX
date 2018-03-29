#pragma once

#define g_pTextureManager cTextureManager::GetInstance()


class cTextureManager
{
private:
	SINGLETONE(cTextureManager);

private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;

public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(string& sFullPath);
	void Destroy(void);

};

