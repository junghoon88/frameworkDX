#pragma once

class cLight
{
private:
	map<string, D3DLIGHT9> m_mapDirLight;
	map<string, D3DLIGHT9> m_mapPointLight;
	map<string, D3DLIGHT9> m_mapSpotLight;

public:
	cLight();
	~cLight();

public:
	void Setup(void);
};

