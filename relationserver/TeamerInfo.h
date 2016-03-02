#pragma once
#include "TeamAuthorityHolder.h"
#include "IFRoleProxy.h"
#include "ETeamDef.h"
#include "TeamStateBase.h"
#include "RequestRecord.h"
#include "TripVote/T_TripVote.h"
namespace Team
{
	class RsTeamerContainer;
	class TeamerInfo : public IFRoleProxy, public VoteNS::IFVoteMember, public VoteNS::T_ITripVoteListener
	{
	
	private:
		ERoleTeamState m_eCurStateType;
		FxRole_RS* m_pRole;
		RsTeamerContainer* m_pContainer;
		TeamStateBase* m_States[ERoleTeamStateCount];

		std::bitset<ESubscribeIdxCount> m_SubscribeInfo;
	public:
		TeamerInfo();
		~TeamerInfo();

		void Reset();

		virtual FxRole_RS* GetRole() const;

		FxRole_RS* setRole(FxRole_RS* pRole);

		void setStateType(ERoleTeamState eVal);
	
		ERoleTeamState getStateType() const;
	
		TeamStateBase* GetState() const;
		TeamStateBase* GetState(ERoleTeamState eVal) const;
		
		ESubTeamID GetSubTeamID() const;
		UINT8 GetIndexOfSubTeam() const;
	
		/**仅供GroupGang访问*/
 		RsTeamerContainer* setContainer(RsTeamerContainer* pTeamGang);
 	
 		RsTeamerContainer* GetContainer() const;

		bool UpdateStateByContainer();//返回值无意义

		virtual UINT32 GetRoleID() const;

		virtual UINT32 GetRoleOBJID() const;

		virtual char* GetRoleName() const;

		virtual ERoleState GetRoleState() const;

		virtual UINT32 GetCamp() const;

		virtual UINT8 GetJob() const;

		virtual UINT8 GetLevel() const;

		virtual bool SendGS(UINT16 wMsgId, const char* pBuff) const;

		virtual bool SendDB(UINT16 wMsgId, const char* pBuff) const;

		virtual bool SendRS(UINT16 wMsgId, const char* pBuff) const;

		virtual bool SendClient(UINT16 wMsgId, const char* pBuff) const;

		virtual TeamerInfo* GetTeamerInfo() const;

		virtual void SendMsg(UINT32 dwMsgId, const char* strDyn = NULL) const;

		virtual void SendMsgEx(UINT32 dwMsgId, const char* strDyn = NULL) const;

		virtual void OnTurnedToGroup(FxRole_RS* pTeamLeader, FxRole_RS* pGroupLeader);

		virtual void OnLeaveOrgnization();

		virtual void OnJoinTeam(FxRole_RS* pGroupLeader, UINT32 dwOrgID);

		virtual void OnJoinGroup(FxRole_RS* pGroupLeader, UINT32 dwOrgID);

		virtual void OnInvitedInTeam(FxRole_RS* pGroupLeader, UINT32 dwOrgID);

		virtual void OnInvitedInGroup(FxRole_RS* pGroupLeader, UINT32 dwOrgID);

		//////////////////////////////////////////////////////////////////////////
		EErrorCode CanInvite(UINT32 dwTargetRoleID) const;
		RequestRecord* GetInviteRecord(UINT32 dwTargetRoleID) const;
		RequestRecord* AddInviteRecord(UINT32 dwTargetRoleID);
		size_t DelInviteRecord(UINT32 dwTargetRoleID);



		//////////////////////////////////////////////////////////////////////////
		//设置订阅
		virtual void SetSubscribe(ESubscribeIdx eIdx, bool bActive);
		virtual UINT32 GetSubscribe() const;

		virtual void ClearSubscribe();

		virtual UINT32 GetAllAuthority() const;
		
		bool CheckAuthority(ETeamAuthority eAuthority) const;
		//////////////////////////////////////////////////////////////////////////
		//VOTE Listener
		virtual void OnTripVote_Clear(void);

		virtual void OnTripVote_Complete(const SRoleTripVoteSceneInfo& sInfo);

		virtual void OnTripVote_Accept(VoteNS::IFVoteMember* pMember);

		virtual void OnTripVote_Refuse(VoteNS::IFVoteMember* pMember);

		virtual void OnTripVote_TimeOut(VoteNS::IFVoteMember* pMember);

		virtual UINT32 GetVoteIdentity() const;
		virtual FxRole_RS* GetVoteRole() const;
		virtual void OnExitTripVote();
		virtual void OnInterruptTripVote();

		//////////////////////////////////////////////////////////////////////////

		time_t GetRecordTime() const;
		void RecordTime();
		void OffsetRecordTime(time_t val);

		
		void RefuseAllInvite() const;

        void RefuseAll() const;

	private:
		RequestRecordBag	m_InviteRecords;
		time_t m_RecordTime;
	};
}
