#pragma once

class cMtlTex;
class cGroup;

class cObjLoader
{
public:
	cObjLoader();
	~cObjLoader();

private:
	map<string, cMtlTex*> m_mapMtlTex;

public:
	void Load_jhkim(OUT vector<cGroup*> &vecGroup, IN char* szFolder, IN char* szFile);
	void LoadMtlLib_jhkim(char* szFolder, char* szFile);

	void Load(OUT vector<cGroup*> &vecGroup, IN char* szFolder, IN char* szFile);
	void LoadMtlLib(char* szFolder, char* szFile);
	void LoadSurface(OUT vector<D3DXVECTOR3>& vecSurface, IN char* szFolder, IN char* szFile, IN D3DXMATRIXA16* pMat = NULL);

};

