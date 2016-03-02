#include "RsTeamerContainer.h"
#include "net/gsmgr.h"
#include "RsTeam.h"
#include "RsSubTeam.h"
#include "TeamUtils.h"



namespace Team
{

	bool RsTeamerContainer::BroadCast(UINT16 wMsgId, char* pBuf, UINT32 dwExcludeSvrId /*= 0*/) const
	{
		TeamerServerMap::const_iterator iter = m_OnlineInfos.begin();
		for (; iter != m_OnlineInfos.end(); iter = m_OnlineInfos.upper_bound(iter->first))
		{
			UINT32 dwSvrId = iter->first;
			if (dwSvrId == dwExcludeSvrId || NULL == iter->second)
			{
				continue;
			}

			FxGSMgr::Instance()->SendToGS(dwSvrId, GetTeam()->GetObjId(), wMsgId, pBuf);
		}
		return true;
	}

	bool RsTeamerContainer::BroadCastMsg(UINT32 dwMsgId, const char* pStrDyn) const
	{
		TeamerServerMap::const_iterator iter = m_OnlineInfos.begin();
		for (; iter != m_OnlineInfos.end(); iter++)
		{
			UINT32 dwSvrId = iter->first;
			TeamerInfo* pInfo = iter->second;
			if (NULL == pInfo)
			{
				continue;
			}

			pInfo->SendMsg(dwMsgId, pStrDyn);
		}
		return true;
	}

	bool RsTeamerContainer::BroadCastMsgEx(UINT32 dwMsgId, const char* pStrDyn) const
	{
		TeamerServerMap::const_iterator iter = m_OnlineInfos.begin();
		for (; iter != m_OnlineInfos.end(); iter++)
		{
			UINT32 dwSvrId = iter->first;
			TeamerInfo* pInfo = iter->second;
			if (NULL == pInfo)
			{
				continue;
			}

			pInfo->SendMsgEx(dwMsgId, pStrDyn);
		}
		return true;
	}

	bool RsTeamerContainer::DealTransfer(SS_TRANSFER* pData, UINT32 dwExcludeSvrId) const
	{
		if (GetTeam()->GetObjType() == pData->byDestType)
		{
			BroadCast(SSID_TRANSFER, (char*)pData, dwExcludeSvrId);
		}
		else
		{
			USR_IMPORTANT("Gang OrgType Type: %d, Transfer Type: %d", ObjType_Team_T, pData->byDestType);
		}
		return true;
	}

	TeamerInfo* RsTeamerContainer::onAdd(TeamerInfo* pVal)
	{
		if (NULL != pVal)
		{
			pVal->setContainer(this);
			RefreshOnlineInfos(pVal);
		}

		return pVal;
	}

	TeamerInfo* RsTeamerContainer::onRemove(TeamerInfo* pVal)
	{
		if (NULL != pVal)
		{
			removeFromOnlineInfos(pVal);
			pVal->setContainer(NULL);
		}

		return pVal;
	}

	RsTeamerContainer::RsTeamerContainer() : m_pTeam(NULL)
	{
	}

	RsTeamerContainer::~RsTeamerContainer()
	{
		
	}

	RsTeam* RsTeamerContainer::setTeam(RsTeam* pTeam)
	{
		m_pTeam = pTeam;
		return m_pTeam;
	}

	RsTeam* RsTeamerContainer::GetTeam() const
	{
		return m_pTeam;
	}

	bool RsTeamerContainer::removeFromOnlineInfos(const TeamerInfo* pInfo)
	{
		TeamerServerMap::const_iterator iter = m_OnlineInfos.begin();
		for (; iter != m_OnlineInfos.end(); ++iter)
		{
			if (iter->second == pInfo)
			{
				m_OnlineInfos.erase(iter);
				return true;
			}
		}

		return false;
	}

	bool RsTeamerContainer::RefreshOnlineInfos(const TeamerInfo* pInfo)
	{
		do 
		{
			if (NULL == pInfo || !Find(pInfo))
			{
				break;
			}

			if (pInfo->GetRoleState() == RoleState_InGame)
			{
				TeamerServerMap::const_iterator iter = m_OnlineInfos.begin();
				for (; iter != m_OnlineInfos.end(); ++iter)
				{
					if (iter->second == pInfo)
					{
						if (iter->first != pInfo->GetRole()->GetSvrId())
						{//服务器ID 不同删除
							m_OnlineInfos.erase(iter);
							iter = m_OnlineInfos.end();
						}
						break;
					}
				}

				if (iter == m_OnlineInfos.end())
				{
					m_OnlineInfos.insert(TeamerServerMap::value_type(pInfo->GetRole()->GetSvrId(), const_cast<TeamerInfo*>(pInfo)));
					return true;
				}
			}
			else if (pInfo->GetRoleState() == RoleState_Logout)
			{
				return removeFromOnlineInfos(pInfo);
			}

		} while (false);
		return false;
	}

	bool RsTeamerContainer::GetOnlineTeamers(std::vector<TeamerInfo*>& routMembers) const
	{
		TeamerServerMap::const_iterator iter = m_OnlineInfos.begin();
		for (; iter != m_OnlineInfos.end(); ++iter)
		{
			TeamerInfo* pInfo = iter->second;
			if (NULL == pInfo)
			{
				continue;
			}

			routMembers.push_back(pInfo);
		}

		return true;
	}

}
