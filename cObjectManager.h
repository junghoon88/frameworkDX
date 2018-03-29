#pragma once

#define g_pObjectManager cObjectManager::GetInstance()

class cObject;

class cObjectManager
{
private:
	SINGLETONE(cObjectManager);

private:
	std::set<cObject*> m_setObject;

public:
	void AddObject(cObject* pObject)
	{
		m_setObject.insert(pObject);
	}

	void RemoveObject(cObject* pObject)
	{
		m_setObject.erase(pObject);
	}

	void Destroy(void)
	{
		m_setObject.clear();
		assert(m_setObject.empty());
	}
};

