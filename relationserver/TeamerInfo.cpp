#include "TeamerInfo.h"
#include "StateTeam.h"
#include "StateGroup.h"
#include "StateSingle.h"
#include "TeamUtils.h"
#include "net/gsbuilder.h"
namespace Team
{
	FxRole_RS* TeamerInfo::GetRole() const
	{
		return m_pRole;
	}

	FxRole_RS* TeamerInfo::setRole(FxRole_RS* pRole)
	{
		m_pRole = pRole;
		return pRole;
	}

	void TeamerInfo::setStateType(ERoleTeamState eVal)
	{
		if (m_eCurStateType == eVal || ERoleTeamStateCount <= eVal)
		{
			return;
		}
		ERoleTeamState ePreState = m_eCurStateType;
		m_eCurStateType = eVal;
		if (GetState())
		{
			GetState()->OnEnter(ePreState);
		}
	}

	ERoleTeamState TeamerInfo::getStateType() const
	{
		return m_eCurStateType;
	}

	TeamStateBase* TeamerInfo::GetState() const
	{
		if (ERoleTeamStateCount > m_eCurStateType)
		{
			return m_States[m_eCurStateType];
		}

		return NULL;
	}

	TeamStateBase* TeamerInfo::GetState(ERoleTeamState eVal) const
	{
		if (eVal >= 0 && eVal < ERoleTeamStateCount)
		{
			return m_States[eVal];
		}

		return NULL;
	}

	RsTeamerContainer* TeamerInfo::setContainer(RsTeamerContainer* pTeamGang)
	{
		m_pContainer = pTeamGang;
		UpdateStateByContainer();
		return m_pContainer;
	}

	RsTeamerContainer* TeamerInfo::GetContainer() const
	{
		return m_pContainer;
	}

	TeamerInfo::TeamerInfo()
		: m_pContainer(NULL)
		, m_pRole(NULL)
		, m_eCurStateType(ERoleTeamStateInvalid)
		, m_InviteRecords(MAX_INVITE_RECORD)
		, m_RecordTime(time(NULL))
	{
		m_States[ERoleTeamStateInTeam] = new TeamState(this);
		m_States[ERoleTeamStateInGroup] = new GroupState(this);
		m_States[ERoleTeamStateSingle] = new SingleState(this);
	}

	void TeamerInfo::Reset()
	{
		m_pContainer = NULL;
		setStateType(ERoleTeamStateSingle);
	}

	TeamerInfo::~TeamerInfo()
	{
		size_t idx = 0;
		for (; idx < ERoleTeamStateCount; ++idx)
		{
			TeamStateBase* pVal = m_States[idx];
			if (NULL == pVal)
			{
				continue;
			}
			delete pVal;

			m_States[idx] = NULL;
		}
		
	}

	UINT32 TeamerInfo::GetRoleID() const
	{
		if (m_pRole)
		{
			return m_pRole->GetRoleId();
		}
		return INVALID_ROLE_ID;
	}

	char* TeamerInfo::GetRoleName() const
	{
		if (m_pRole)
		{
			return m_pRole->m_szRoleName;
		}
		return NULL;
	}

	ERoleState TeamerInfo::GetRoleState() const
	{
		if (m_pRole)
		{
			return m_pRole->GetRoleState();
		}
		return RoleState_Invalid;
	}

	bool TeamerInfo::SendGS(UINT16 wMsgId, const char* pBuff) const
	{
		if (m_pRole)
		{
			return const_cast<FxRole_RS*>(m_pRole)->SendGS(wMsgId, const_cast<char*>(pBuff));
		}
		return false;
	}

	bool TeamerInfo::SendDB(UINT16 wMsgId, const char* pBuff) const
	{
		if (m_pRole)
		{
			return const_cast<FxRole_RS*>(m_pRole)->SendDB(wMsgId, const_cast<char*>(pBuff));
		}
		return false;
	}

	bool TeamerInfo::SendRS(UINT16 wMsgId, const char* pBuff) const
	{
		return false;
	}

	bool TeamerInfo::SendClient(UINT16 wMsgId, const char* pBuff) const
	{
		return false;
	}

	void TeamerInfo::OnTurnedToGroup(FxRole_RS* pTeamLeader, FxRole_RS* pGroupLeader)
	{
	}

	void TeamerInfo::OnLeaveOrgnization()
	{
	}

	void TeamerInfo::OnJoinTeam(FxRole_RS* pGroupLeader, UINT32 dwOrgID)
	{
	}

	void TeamerInfo::OnJoinGroup(FxRole_RS* pGroupLeader, UINT32 dwOrgID)
	{
		
	}

	void TeamerInfo::OnInvitedInTeam(FxRole_RS* pGroupLeader, UINT32 dwOrgID)
	{
	}

	void TeamerInfo::OnInvitedInGroup(FxRole_RS* pGroupLeader, UINT32 dwOrgID)
	{
	}

	UINT32 TeamerInfo::GetRoleOBJID() const
	{
		if (m_pRole)
		{
			return m_pRole->GetObjId();
		}
		return INVALID_OBJ_ID;
	}

	EErrorCode TeamerInfo::CanInvite(UINT32 dwTargetRoleID) const
	{
		RequestRecord* pRecord = m_InviteRecords.GetRecord(dwTargetRoleID);
		if (pRecord && !pRecord->IsExpire())
		{
			return TeamEC_TargetHasBeenInvited;
		}

		if (m_InviteRecords.FreeCount() <= 0)
		{
			return TeamEC_InviteTooMany;
		}

		return TeamEC_OK;
	}

	RequestRecord* TeamerInfo::AddInviteRecord(UINT32 dwTargetRoleID)
	{
		EErrorCode err = CanInvite(dwTargetRoleID);
		if (TeamEC_OK != err)
		{
			return NULL;
		}

		return m_InviteRecords.AddRecord(dwTargetRoleID, GetRoleID());
	}

	size_t TeamerInfo::DelInviteRecord(UINT32 dwTargetRoleID)
	{
		return m_InviteRecords.DelRecord(dwTargetRoleID, INVALID_ROLE_ID);
	}

	RequestRecord* TeamerInfo::GetInviteRecord(UINT32 dwTargetRoleID) const
	{
		return m_InviteRecords.GetRecord(dwTargetRoleID);
	}

	TeamerInfo* TeamerInfo::GetTeamerInfo() const
	{
		return const_cast<TeamerInfo*>(this);
	}

	void TeamerInfo::SendMsg(UINT32 dwMsgId, const char* strDyn /*= NULL*/) const
	{
		FxRSPacketBuilder opb;
		SendGS(opb.RoleMsgNtfChar(dwMsgId, strDyn), opb.GetPacketBuf());
	}

	void TeamerInfo::SendMsgEx(UINT32 dwMsgId, const char* strDyn /*= NULL*/) const
	{
		FxRSPacketBuilder opb;
		SendGS(opb.RoleMsgExNtfChar(dwMsgId, strDyn), opb.GetPacketBuf());
	}

	void TeamerInfo::SetSubscribe(ESubscribeIdx eIdx, bool bActive)
	{
		if (eIdx >= ESubscribeIdxCount)
		{
			return;
		}

		m_SubscribeInfo.set(eIdx, bActive);
	}

	UINT32 TeamerInfo::GetSubscribe() const
	{
		return (UINT32)m_SubscribeInfo.to_ulong();
	}

	void TeamerInfo::ClearSubscribe()
	{
		m_SubscribeInfo.reset();
	}

	UINT32 TeamerInfo::GetAllAuthority() const
	{
		if (GetState())
		{
			return GetState()->GetAllAuthority();
		}

		return 0;
	}

	ESubTeamID TeamerInfo::GetSubTeamID() const
	{
		RsTeam* pTeam = TeamUtils::SGetTeam(this);
		if (pTeam)
		{
			return pTeam->GetSubTeamID(this);
		}

		return ESubTeam0;
	}

	UINT8 TeamerInfo::GetIndexOfSubTeam() const
	{
		RsTeam* pTeam = TeamUtils::SGetTeam(this);
		if (pTeam)
		{
			return pTeam->GetIndexOfSubTeam(this);
		}
		
		return 0;
	}

	bool TeamerInfo::CheckAuthority(ETeamAuthority eAuthority) const
	{
		if (GetState())
		{
			return GetState()->CheckAuthority(eAuthority);
		}

		return false;
	}

	bool TeamerInfo::UpdateStateByContainer()
	{
		if (NULL == m_pContainer)
		{
			setStateType(ERoleTeamStateSingle);
		}
		else if (m_pContainer->GetTeam()->GetTeamType() == EOrganizationGroup)
		{
			setStateType(ERoleTeamStateInGroup);
		}
		else
		{
			setStateType(ERoleTeamStateInTeam);
		}

		return false;//
	}

	void TeamerInfo::OnTripVote_Clear(void)
	{
	}

	void TeamerInfo::OnTripVote_Complete(const SRoleTripVoteSceneInfo& sInfo)
	{
		if (GetRole() == NULL)
			return;
		FxRSPacketBuilder opb;
		GetRole()->SendGS(opb.RoleTripVoteSceneNtf(sInfo), opb.GetPacketBuf());
	}

	void TeamerInfo::OnExitTripVote()
	{
		if (GetRole() == NULL)
			return;
		FxRSPacketBuilder opb;
		GetRole()->SendGS(opb.RoleTripVoteDlgCloseNtf(), opb.GetPacketBuf());
	}

	void TeamerInfo::OnTripVote_Accept(VoteNS::IFVoteMember* pMember)
	{
		if (GetRole() == NULL)
			return;
		FxRSPacketBuilder opb;
		GetRole()->SendGS(opb.RoleTripVoteAcceptNtf(pMember->GetVoteIdentity()), opb.GetPacketBuf());
	}

	void TeamerInfo::OnTripVote_Refuse(VoteNS::IFVoteMember* pMember)
	{
		if (GetRole() == NULL)
			return;
		FxRSPacketBuilder opb;
		GetRole()->SendGS(opb.RoleTripVoteRefuseNtf(pMember->GetVoteIdentity()), opb.GetPacketBuf());
	}

	void TeamerInfo::OnTripVote_TimeOut(VoteNS::IFVoteMember* pMember)
	{
		if (GetRole() == NULL)
			return;
		FxRSPacketBuilder opb;
		GetRole()->SendGS(opb.RoleTripVoteTimeOutNtf(pMember->GetVoteIdentity()), opb.GetPacketBuf());
	}

	UINT32 TeamerInfo::GetVoteIdentity() const
	{
		return GetRoleID();
	}

	UINT32 TeamerInfo::GetCamp() const
	{
		if (m_pRole)
		{
			return m_pRole->GetCamp();
		}
		return 0;
	}

	time_t TeamerInfo::GetRecordTime() const
	{
		return m_RecordTime;
	}

	void TeamerInfo::RecordTime()
	{
		m_RecordTime = time(NULL);
	}

	void TeamerInfo::OffsetRecordTime(time_t val)
	{
		m_RecordTime += val;
	}

	UINT8 TeamerInfo::GetJob() const
	{
		if (m_pRole)
		{
			return m_pRole->GetCareer();
		}
		return 0;
	}

	UINT8 TeamerInfo::GetLevel() const
	{
		if (m_pRole)
		{
			return (UINT8)m_pRole->GetLevel();
		}
		return 0;
	}

	void TeamerInfo::RefuseAllInvite() const
	{
		UINT32 dwRoleID = GetRoleID();
		RequestRecord::RecordVEC allInvite4Me;
		RequestRecord::SGetAllInviteRecords(dwRoleID, allInvite4Me);


		TeamStateBase* pMineState = GetState();
		if (NULL == pMineState)
		{
			return;
		}

		for (RequestRecord::RecordVEC::iterator iter = allInvite4Me.begin(); 
			iter != allInvite4Me.end(); ++iter)
		{
			RequestRecord* pVal = *iter;
			if (pVal->m_Operator == INVALID_ROLE_ID)
			{
				continue;
			}

			pMineState->InviteRES(pVal->m_Operator, ETeamVerifyRefuse, pVal->m_TeamID, (EOrganizationType)pVal->m_eCurTeamType);
		}
	}

	FxRole_RS* TeamerInfo::GetVoteRole() const
	{
		return GetRole();
	}

	void TeamerInfo::OnInterruptTripVote()
	{
		if (GetRole() == NULL)
			return;
		FxRSPacketBuilder opb;
		GetRole()->SendGS(opb.RoleTripVoteInterrupt(), opb.GetPacketBuf());
	}

    void TeamerInfo::RefuseAll() const
    {
        RefuseAllInvite();
        RsTeam * pSelfTeam = TeamUtils::SGetTeam(GetTeamerInfo());
        if (pSelfTeam)
        {
            pSelfTeam->mRecordMgr.ClearJoinRecords();
            pSelfTeam->mRecordMgr.ClearRecommendRecords();
        }
    }

}
