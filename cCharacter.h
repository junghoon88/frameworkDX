#pragma once

class cCharacter
{
public:
	cCharacter();
	virtual ~cCharacter();

protected:
	float					m_fRotY;
	D3DXVECTOR3				m_vPosition;
	D3DXVECTOR3				m_vDirection;
	D3DXMATRIXA16			m_matWorld;

	float					m_fMoveSpeed;
	bool					m_isMoving;
	bool					m_isJumping; // 떨어질때도 사용
	float					m_fJumpPower;
	float					m_fGravity;

	iMap*					m_pMap;

public:
	virtual void Setup(void);
	virtual void Update(iMap* pMap);
	virtual void Render(void);
	virtual D3DXVECTOR3& GetPosition(void);


public:
	inline void SetPosition(D3DVECTOR pos) { m_vPosition = pos; }
	inline void SetDirection(D3DVECTOR dir) { m_vDirection = dir; }

};

