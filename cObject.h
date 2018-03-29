#pragma once
class cObject
{
public:
	cObject();
	virtual ~cObject();

protected:
	ULONG	m_ulRefCount;

public:
	virtual void AddRef(void);
	virtual void Release(void);

};

