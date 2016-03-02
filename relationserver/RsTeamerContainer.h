
#pragma once
#include "TeamerInfo.h"
#include "TSimpleContainer.h"
#include "rsproto.h"
namespace Team
{
	class RsTeam;
	class RsSubTeam;
	class RsTeamerContainer : public TSimpleContainer<TeamerInfo>
	{
	private:
		RsTeam* m_pTeam;

// 		typedef std::vector<RsSubTeam*> SUBTEAM_VEC;
// 		SUBTEAM_VEC m_SubTeams;

		typedef std::multimap<UINT32, TeamerInfo*> TeamerServerMap;
		/** <服务器ID, 队员>*/
		TeamerServerMap m_OnlineInfos;
	public:
		RsTeamerContainer();
		~RsTeamerContainer();

		bool RefreshOnlineInfos(const TeamerInfo* pInfo);
		//每个服务器发一次
		bool BroadCast(UINT16 wMsgId, char* pBuf, UINT32 dwExcludeSvrId = 0) const;
	
		bool BroadCastMsg(UINT32 dwMsgId, const char* pStrDyn = NULL) const;
	
		bool BroadCastMsgEx(UINT32 dwMsgId, const char* pStrDyn = NULL) const;

		bool DealTransfer(SS_TRANSFER* pData, UINT32 dwExcludeSvrId) const;

		bool GetOnlineTeamers(std::vector<TeamerInfo*>& routMembers) const;

		virtual TeamerInfo* onAdd(TeamerInfo* pVal);
		virtual TeamerInfo* onRemove(TeamerInfo* pVal);

		RsTeam* setTeam(RsTeam* pTeam);
		RsTeam* GetTeam() const;

	protected:
		bool removeFromOnlineInfos(const TeamerInfo* pInfo);

		

	};
}
