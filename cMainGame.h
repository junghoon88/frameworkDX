#pragma once

class cCamera;
class cLight;

class cMainGame
{
public:
	cMainGame();
	~cMainGame();

private:
	cCamera*		m_pCamera;
	cLight*			m_pLight;


	iMap*			m_pMap;

public:
	void Setup(void);
	void Update(void);
	void Render(void);

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

