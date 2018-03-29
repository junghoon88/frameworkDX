#include "stdafx.h"
#include "cLight.h"


cLight::cLight()
{
}


cLight::~cLight()
{
}

void cLight::Setup(void)
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	//D3DLIGHT_DIRECTIONAL
	D3DLIGHT9 dirLight;
	dirLight.Type = D3DLIGHT_DIRECTIONAL;
	dirLight.Diffuse   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)* 0.5f;
	dirLight.Specular  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)* 0.5f * 0.3f;
	dirLight.Ambient   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)* 0.5f * 0.6f; 
	dirLight.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	g_pD3DDevice->SetLight(0, &dirLight);
	g_pD3DDevice->LightEnable(0, true);
	m_mapDirLight.insert(make_pair("mainDirLight0", dirLight));

#if 0															
	m_pointLight.Type = D3DLIGHT_POINT;           
	m_pointLight.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);         
	m_pointLight.Specular = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) * 0.3f; 
	m_pointLight.Ambient = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) * 0.6f;  
	m_pointLight.Position = D3DXVECTOR3(3.0f, 0.0f, 3.0f);          
	m_pointLight.Range = 10.0f;         
	m_pointLight.Attenuation0 = 1.0f;   
	m_pointLight.Attenuation1 = 0.5f;   
	m_pointLight.Attenuation2 = 0.25f;  

	//D3DLIGHT_SPOT
	m_spotLight.Type = D3DLIGHT_SPOT;            
	m_spotLight.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);        
	m_spotLight.Specular = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) * 0.3f;
	m_spotLight.Ambient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) * 0.6f; 
	m_spotLight.Position = D3DXVECTOR3(-3.0f, 0.0f, -3.0f);         
	m_spotLight.Direction = D3DXVECTOR3(1.0f, 0.0f, 1.0f);       
	m_spotLight.Range = 15.0f;         
	m_spotLight.Falloff = 1.0f;        
	m_spotLight.Attenuation0 = 1.0f;   
	m_spotLight.Attenuation1 = 0.5f;   
	m_spotLight.Attenuation2 = 0.25f;  
	m_spotLight.Theta = D3DX_PI / 6.0f;
	m_spotLight.Phi = D3DX_PI / 4.0f;  

#endif
}

