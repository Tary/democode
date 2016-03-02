#pragma once
#include "ETeamDef.h"
namespace Team
{

	template<class FxObjFactory_OBJ>
	class AutoReleaseTool
	{
	public:
		AutoReleaseTool(FxObjFactory_OBJ* pObj);
		~AutoReleaseTool();
		FxObjFactory_OBJ* operator = (FxObjFactory_OBJ* pObj);
		FxObjFactory_OBJ* Get() const;
		void UnManage();
	private:
		void release();

		FxObjFactory_OBJ* m_pObj;
	};

	template<class FxObjFactory_OBJ>
	Team::AutoReleaseTool<FxObjFactory_OBJ>::AutoReleaseTool(FxObjFactory_OBJ* pObj) : m_pObj(pObj)
	{

	}

	template<class FxObjFactory_OBJ>
	FxObjFactory_OBJ* Team::AutoReleaseTool<FxObjFactory_OBJ>::Get() const
	{
		return m_pObj;
	}

	template<class FxObjFactory_OBJ>
	void Team::AutoReleaseTool<FxObjFactory_OBJ>::release()
	{
		if (NULL != m_pObj)
		{
			delete m_pObj;
			m_pObj = NULL;
		}
	}

	template<class FxObjFactory_OBJ>
	FxObjFactory_OBJ* Team::AutoReleaseTool<FxObjFactory_OBJ>::operator=(FxObjFactory_OBJ* pObj)
	{
		if (m_pObj != pObj)
		{
			release();
			m_pObj = pObj;
		}

		return m_pObj;
	}

	template<class FxObjFactory_OBJ>
	Team::AutoReleaseTool<FxObjFactory_OBJ>::~AutoReleaseTool()
	{
		release();
	}

	template<class FxObjFactory_OBJ>
	void Team::AutoReleaseTool<FxObjFactory_OBJ>::UnManage()
	{
		m_pObj = NULL;
	}

	template<class FxObjFactory_OBJ>
	Team::AutoReleaseTool<FxObjFactory_OBJ>::AutoReleaseTool(FxObjFactory_OBJ* pObj) : m_pObj(pObj)
	{

	}

	template<>
	void Team::AutoReleaseTool<RsTeam>::release()
	{
		if (NULL != m_pObj)
		{
			FxObjFactory::Instance()->T_ReleaseTeam(m_pObj);
		}
	}	
}


