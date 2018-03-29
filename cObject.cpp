#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
	: m_ulRefCount(1)
{
	g_pObjectManager->AddObject(this);
}


cObject::~cObject()
{
	g_pObjectManager->RemoveObject(this);
}

void cObject::AddRef(void)
{
	++m_ulRefCount;
}

void cObject::Release(void)
{
	--m_ulRefCount;

	if (m_ulRefCount == 0)
	{
		delete this;
	}
}
