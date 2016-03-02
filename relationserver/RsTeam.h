#pragma once
#include "ETeamDef.h"
#include "RsTeamerContainer.h"
#include "rsproto.h"
#include "RsOrganization.h"
#include "TripVote/T_TripVote.h"
namespace Team
{
	class TeamerInfo;
	class RsGroupGang;
	class RsSubTeam;
	class RsTeam : public RsOrganization, public VoteNS::IFVoteOrganization
	{
	public:
		RsTeam();
		~RsTeam();
		//////////////////////////////////////////////////////////////////////////
		void TurnToGroup();
		void TurnToTeam();

		virtual void Dismiss();

		TeamerInfo* Get(size_t idx) const;

		TeamerInfo* Remove(size_t idx);

		TeamerInfo* Remove(TeamerInfo* pVal);

		bool Move(TeamerInfo* pVal, ESubTeamID eSubIdx, size_t posInSub);

		TeamerInfo* Add(TeamerInfo* pVal, size_t idx = AUTO_INCREASE_IDX);

		virtual void Init();

		virtual void Reset();

		virtual int GetObjType() const;

		const RsTeamerContainer* GetMembers() const;

		virtual bool IsMember(UINT32 dwRoleID) const;
		bool IsMember(const TeamerInfo* pInfo) const;

		bool IsEnemyWithMember(const TeamerInfo* pInfo) const;

		TeamerInfo* GetMember(UINT32 dwRoleID) const;

		bool UpdateTeam(const TeamerInfo* pStateChangeRole);

		bool LeaveTeam(const TeamerInfo* pTargetTeamInfo);

		bool SyncTeamerPosition();

		bool BroadCast(UINT16 wMsgId, char* pBuf, UINT32 dwExcludeSvrId = 0) const;

		bool BroadCastMsg(UINT32 dwMsgId, const char* pStrDyn = NULL) const;

		bool BroadCastMsgEx(UINT32 dwMsgId, const char* pStrDyn = NULL) const;

		bool DealTransfer(SS_TRANSFER* pData, UINT32 dwExcludeSvrId) const;
		
		virtual bool AssignNewLeader(const TeamerInfo* pPreferInfo = NULL);


		ESubTeamID GetSubTeamID(const TeamerInfo* pVal) const;
		UINT8 GetIndexOfSubTeam(const TeamerInfo* pVal) const;

		void RefuseAllJoin(EErrorCode reason);
		void DropAllRecommend();

		bool UpdateFriendShip();
	protected:
		virtual FxRole_RS* voteLeader(ERoleState eRequestState, const TeamerInfo* pPreferInfo = NULL) const;

		void updateSubteamToGroup();
		void updateSubteamToTeam();
		bool addToSubteam(TeamerInfo* pVal);
		bool removeFromSubteam(TeamerInfo* pVal);
	protected:friend class TeamUtils;
		UINT32 getFriendShip() const;

		void resetSyncPosTime();
		void updateSyncPosTime();

		


	public:
		//////////////////////////////////////////////////////////////////////////
		//VOTE
		VoteNS::T_TripVote* GetVote() const;
		virtual VoteNS::IFVoteMember* GetVoteLeader() const;

		virtual bool GetOnLineRoles4Vote(VoteNS::VoteMemberVEC& vecMembers) const;

		virtual void OnVoteComplate(bool bSuccess);

		virtual void BroadCastVoteMsg(EErrorCode err);

		virtual void SendVoteInit(VoteNS::IFVoteMember* pMember, const SRoleTripVoteSceneInfo& sInfo);
		
		//////////////////////////////////////////////////////////////////////////
	private:
		RsTeamerContainer m_Members;
		UINT32 m_dwFriendsShip;

		typedef std::vector<RsSubTeam*> SUBTEAM_VEC;
		SUBTEAM_VEC m_SubTeams;

		time_t m_LastSyncPosTime;

		VoteNS::T_TripVote m_TripVote;

		UINT32 m_dwPreLeaderID;
	};
}
