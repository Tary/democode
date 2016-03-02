#include "RsTeam.h"
#include "TeamUtils.h"
#include "net/gsbuilder.h"
#include "net/gsmgr.h"
#include "factory/objfactory.h"
#include "RsSubTeam.h"

#define SYNC_POS_DELAY 5

namespace Team
{
	TeamerInfo* RsTeam::Get(size_t idx) const
	{
		return m_Members.Get(idx);
	}

	RsTeam::RsTeam() : m_dwFriendsShip(0), m_dwPreLeaderID(INVALID_ROLE_ID)
	{
		resetSyncPosTime();
		m_Members.setTeam(this);
	}

	const RsTeamerContainer* RsTeam::GetMembers() const
	{
		return &m_Members;
	}

	bool RsTeam::IsMember(UINT32 dwRoleID) const
	{
		return NULL != GetMember(dwRoleID);
	}

	bool RsTeam::IsMember(const TeamerInfo* pInfo) const
	{
		return m_Members.Find(pInfo);
	}

	FxRole_RS* RsTeam::voteLeader(ERoleState eRequestState, const TeamerInfo* pPreferInfo) const
	{
		TeamerInfo* pResult = NULL;

		if (pPreferInfo && TeamUtils::SCheckTeamerState(pPreferInfo, eRequestState, pResult) &&
			IsMember(pPreferInfo))
		{
			//使用优先候选人
		}
		else
		{
			m_Members.foreach2rend(TeamUtils::SSelectEarliestValidState, eRequestState, pResult);
		}
		

		if (pResult)
		{
			return pResult->GetRole();
		}

		if (GetLeader() && !IsMember(GetLeader()->GetRoleId()))
		{
			return NULL;
		}

		return GetLeader();
	}

	bool RsTeam::BroadCast(UINT16 wMsgId, char* pBuf, UINT32 dwExcludeSvrId) const
	{
		return m_Members.BroadCast(wMsgId, pBuf, dwExcludeSvrId);
	}

	bool RsTeam::BroadCastMsg(UINT32 dwMsgId, const char* pStrDyn) const
	{
		return m_Members.BroadCastMsg(dwMsgId, pStrDyn);
	}

	bool RsTeam::BroadCastMsgEx(UINT32 dwMsgId, const char* pStrDyn) const
	{
		return m_Members.BroadCastMsgEx(dwMsgId, pStrDyn);
	}

	bool RsTeam::DealTransfer(SS_TRANSFER* pData, UINT32 dwExcludeSvrId) const
	{
		return m_Members.DealTransfer(pData, dwExcludeSvrId);
	}

	TeamerInfo* RsTeam::Add(TeamerInfo* pVal, size_t idx)
	{
		if (!addToSubteam(pVal))
		{
			return NULL;
		}

		if (m_Members.Add(pVal, idx))
		{
			pVal->RecordTime();
			m_TripVote.OnMemberEnterVote(pVal);
			return pVal;
		}
		return NULL;
	}

	TeamerInfo* RsTeam::Remove(TeamerInfo* pVal)
	{
		removeFromSubteam(pVal);

		m_TripVote.OnMemberLeaveVote(pVal);
		return m_Members.Remove(pVal);
	}

	TeamerInfo* RsTeam::Remove(size_t idx)
	{
		TeamerInfo* pRmVa = m_Members.Remove(idx);
		if (pRmVa)
		{
			m_TripVote.OnMemberLeaveVote(pRmVa);
			removeFromSubteam(pRmVa);		
		}
		return pRmVa;
	}

	void RsTeam::Dismiss()
	{
		RefuseAllJoin(TeamUtils::SConvertErrcode(TeamEC_TargetTeamDismiss, GetTeamType()));
		m_TripVote.Clear();
		EOrganizationType eType = GetTeamType();
		typedef std::vector<TeamerInfo*> TeamerInfoPtrVec;
		TeamerInfoPtrVec tResult;
		m_Members.foreach1r(TeamUtils::CollectTeamerFunc, tResult);

		FxRSPacketBuilder opb;
		//提示:你离开了队伍
		m_Members.BroadCast(opb.T_TeamMSG_B_D(TeamUtils::SConvertErrcode(TeamEC_ULeaveTeam, eType)), opb.GetPacketBuf());
		//提示:队伍解散了
		m_Members.BroadCast(opb.T_TeamMSG_B_D(TeamUtils::SConvertErrcode(TeamEC_TeamDismiss, eType)), opb.GetPacketBuf());
		//删除, 并同步队伍权限及状态
		for (TeamerInfoPtrVec::const_iterator iter = tResult.begin(); iter != tResult.end(); ++iter)
		{
			TeamerInfo* pVal = *iter;
			if (NULL == pVal)
			{
				continue;
			}

			Remove(pVal);
			pVal->SendGS(opb.T_TeamAuthoritySingleton(pVal), opb.GetPacketBuf());
		}
		//同步删除队伍
		FxGSMgr::Instance()->BroadCastToGS(
			opb.T_TeamOPEvent_B(this->GetObjId(), eType, EOrgOperateDelete),
			opb.GetPacketBuf(), INVALID_TEAM_ID);

		RsOrganization::setTeamType(EOrganizationInvalid);
		FxObjFactory::Instance()->T_ReleaseTeam(this);		
	}

	void RsTeam::TurnToGroup()
	{
		m_Members.setMaxLimit(MAX_TEAM_MEMBER_COUNT_T);
		RsOrganization::setTeamType(EOrganizationGroup);
		updateSubteamToGroup();
		m_Members.foreach(std::mem_fun(&TeamerInfo::UpdateStateByContainer));
		UpdateLeaderAuthority();
	}

	void RsTeam::Init()
	{
		TurnToTeam();
		m_TripVote.BindOrganization(this);
	}

	void RsTeam::Reset()
	{
		TurnToTeam();
	}

	int RsTeam::GetObjType() const
	{
		return ObjType_Team_T;
	}

	bool RsTeam::UpdateTeam(const TeamerInfo* pInfo)
	{
		FxRSPacketBuilder opb;
		if (pInfo && IsEnemyWithMember(pInfo))
		{//退队处理
			if (LeaveTeam(pInfo))
			{
				GetMembers()->BroadCast(opb.T_TeamMSG_B_D(
					TeamUtils::SConvertErrcode(TeamEC_LeaveTeamBecauseEnemyCamp, GetTeamType()), 
					pInfo->GetRoleName()), 
					opb.GetPacketBuf());

				pInfo->SendGS(opb.T_TeamMSG_B_D(
					TeamUtils::SConvertErrcode(TeamEC_ULeaveTeamBecauseEnemyCamp, GetTeamType())),
					opb.GetPacketBuf());
				return false;
			}			
		}
		if (m_Members.RefreshOnlineInfos(pInfo))
		{
			if (pInfo->GetRoleState() == RoleState_InGame)
			{
				m_TripVote.OnMemberEnterVote(const_cast<TeamerInfo*>(pInfo));
			}
			else
			{
				m_TripVote.OnMemberLogoutVote(const_cast<TeamerInfo*>(pInfo));
			}
		}
		AssignNewLeader(pInfo);
		UpdateFriendShip();
		
		m_Members.BroadCast(opb.T_TeamInfoNtf_B(this, InfoChange_All), opb.GetPacketBuf());

		// FxRSStreamBuild osb(pInfo->GetRoleID());
		// osb << PropIndex_RoleState << pInfo->GetRoleState();
		// m_Members.BroadCast(opb.RolePropAnyNtf(osb), opb.GetPacketBuf());
		//权限信息
		m_Members.BroadCast(opb.T_TeamAuthority_B(this), opb.GetPacketBuf());
		updateSyncPosTime();
		return true;
	}

	bool RsTeam::AssignNewLeader(const TeamerInfo* pStateChangeRole)
	{
		UINT32 dwPreLeaderID = GetLeader() ? GetLeader()->GetRoleId() : INVALID_ROLE_ID;
		if (RsOrganization::AssignNewLeader(pStateChangeRole))
		{
			m_TripVote.SetStopVoteFlag();
			m_TripVote.BindOrganization(this);
			m_TripVote.SetListener(GetLeader()->GetTeamerInfo());

			if (dwPreLeaderID != INVALID_ROLE_ID)
			{
				m_dwPreLeaderID = dwPreLeaderID;
			}

			if (NULL == GetLeader())
			{
				RefuseAllJoin(TeamEC_AllMemberOffLine_Join);
				DropAllRecommend();
				return false;
			}

			RefuseAllJoin(TeamUtils::SConvertErrcode(TeamEC_JoinTLeaderChanged, GetTeamType()));
			DropAllRecommend();
			

			FxRSPacketBuilder opb;
			UINT32 dwLeaderID = GetLeader()->GetRoleId();
			m_Members.BroadCast(opb.T_TeamLeaderChg_B(dwLeaderID, dwPreLeaderID), opb.GetPacketBuf());
			if (dwPreLeaderID != INVALID_ROLE_ID)
			{
				m_Members.BroadCast(opb.T_TeamMSG_B_D(
					TeamUtils::SConvertErrcode(TeamEC_TLeaderChanged, GetTeamType()),
					GetLeader()->m_szRoleName, dwLeaderID), opb.GetPacketBuf());

				GetLeader()->GetTeamerInfo()->SendGS(opb.T_TeamMSG_B_D(
					TeamUtils::SConvertErrcode(TeamEC_UBecameTLeader, GetTeamType())),
					opb.GetPacketBuf());
			}			
			return true;
		}
		return false;
	}

	bool RsTeam::UpdateFriendShip()
	{
		UINT32 dwMaxFriendShip = 0;

		m_Members.each_between1rend(TeamUtils::SUpdateMaxFriendship, dwMaxFriendShip);


		if (m_dwFriendsShip != dwMaxFriendShip || 0 == m_dwFriendsShip)
		{//向GameSever发
			m_dwFriendsShip = dwMaxFriendShip;
			//通知所有队员
			FxRSPacketBuilder opb;
			BroadCast(opb.T_TeamFriendShipChg_B(m_dwFriendsShip), opb.GetPacketBuf());			
			return true;
		}

		return false;
	}

	UINT32 RsTeam::getFriendShip() const
	{
		return m_dwFriendsShip;
	}

	bool RsTeam::addToSubteam(TeamerInfo* pVal)
	{
		SUBTEAM_VEC::iterator iter = m_SubTeams.begin();
		for (; iter != m_SubTeams.end(); ++iter)
		{
			RsSubTeam* pSubTeam = *iter;
			if (pSubTeam && pSubTeam->Add(pVal))
			{
				return true;
			}
		}

		return false;
	}

	void RsTeam::updateSubteamToGroup()
	{
		ESubTeamID eSubIdx = ESubTeam0;
		if (!m_SubTeams.empty() && m_SubTeams.back())
		{
			eSubIdx = m_SubTeams.back()->GetID();
		}

		eSubIdx = (ESubTeamID)(eSubIdx + 1);

		for (; eSubIdx < ESubTeamCount; eSubIdx = (ESubTeamID)(eSubIdx + 1))
		{
			m_SubTeams.push_back(new RsSubTeam(eSubIdx));
		}
	}

	void RsTeam::updateSubteamToTeam()
	{
		while (m_SubTeams.size() > 1)
		{
			RsSubTeam* pSub = m_SubTeams.back();
			if (pSub && pSub->GetID() != ESubTeam0)
			{
				delete pSub;
				pSub = NULL;
			}
			m_SubTeams.pop_back();
		}

		if (m_SubTeams.empty())
		{
			m_SubTeams.push_back(new RsSubTeam(ESubTeam0));
		}
	}

	Team::ESubTeamID RsTeam::GetSubTeamID(const TeamerInfo* pVal) const
	{
		SUBTEAM_VEC::const_iterator iter = m_SubTeams.begin();
		for (; iter != m_SubTeams.end(); ++iter)
		{
			RsSubTeam* pSubTeam = *iter;
			if (pSubTeam && pSubTeam->Find(pVal))
			{
				return pSubTeam->GetID();
			}
		}

		return ESubTeam0;
	}

	UINT8 RsTeam::GetIndexOfSubTeam(const TeamerInfo* pVal) const
	{
		SUBTEAM_VEC::const_iterator iter = m_SubTeams.begin();
		for (; iter != m_SubTeams.end(); ++iter)
		{
			RsSubTeam* pSubTeam = *iter;
			if (pSubTeam)
			{
				size_t idx = pSubTeam->IndexOf(pVal);
				if (idx != AUTO_INCREASE_IDX)
				{
					return (UINT8)idx;
				}
			}
		}

		return 0;
	}

	void RsTeam::TurnToTeam()
	{
		m_Members.setMaxLimit(MAX_TEAM_MEMBER_COUNT);
		RsOrganization::setTeamType(EOrganizationTeam);
		updateSubteamToTeam();
	}

	RsTeam::~RsTeam()
	{
		SUBTEAM_VEC::const_iterator iter = m_SubTeams.begin();
		for (; iter != m_SubTeams.end(); ++iter)
		{
			if (*iter)
			{
				delete *iter;
			}
		}

		m_SubTeams.clear();
	}

	bool RsTeam::removeFromSubteam(TeamerInfo* pVal)
	{
		SUBTEAM_VEC::const_iterator iter = m_SubTeams.begin();
		for (; iter != m_SubTeams.end(); ++iter)
		{
			RsSubTeam* pSubTeam = *iter;
			if (pSubTeam && pSubTeam->Find(pVal))
			{
				return pSubTeam->Remove(pVal) != NULL;
			}
		}

		return false;
	}

	bool RsTeam::SyncTeamerPosition()
	{
		if (time(NULL) - m_LastSyncPosTime < SYNC_POS_DELAY)
		{
			return false;
		}

		updateSyncPosTime();
		FxRSPacketBuilder opb;
		m_Members.BroadCast(opb.T_TeamInfoNtf_B(this, InfoChange_MemberPos), opb.GetPacketBuf());
		return true;
	}

	void RsTeam::resetSyncPosTime()
	{
		m_LastSyncPosTime = time(NULL) - SYNC_POS_DELAY;
	}

	void RsTeam::updateSyncPosTime()
	{
		m_LastSyncPosTime = time(NULL);
	}

	void RsTeam::RefuseAllJoin(EErrorCode reason)
	{
		typedef std::set<UINT32> ROLEID_SET;
		ROLEID_SET tTargetIDs;
		mRecordMgr.GetAllJoinTargetID(tTargetIDs);

		if (tTargetIDs.empty())
		{
			return;
		}

		FxRSPacketBuilder opb;
		UINT16 wMsgID = opb.T_TeamJoinAck(reason, GetLeader());
		ROLEID_SET::const_iterator iter = tTargetIDs.begin();
		for (; iter != tTargetIDs.end(); ++iter)
		{
			FxRole_RS* pJoin = FxObjFactory::Instance()->FindRole(*iter);
			if (NULL == pJoin || NULL == pJoin->GetTeamerInfo())
			{	continue;
			}

			pJoin->GetTeamerInfo()->SendGS(wMsgID, opb.GetPacketBuf());
		}

		mRecordMgr.ClearJoinRecords();
	}

	bool RsTeam::Move(TeamerInfo* pVal, ESubTeamID eSubIdx, size_t posInSub)
	{
		if (eSubIdx < 0 || eSubIdx >= ESubTeamCount || posInSub >= MAX_TEAM_MEMBER_COUNT)
		{
			return false;
		}

		RsSubTeam* pDstSubTeam = m_SubTeams.at(eSubIdx);
		if (NULL == pDstSubTeam)
		{
			return false;
		}

		TeamerInfo* pDstVal = pDstSubTeam->Get(posInSub);
		if (pVal == pDstVal)
		{
			return true;
		}

		ESubTeamID eSrcSubIdx = pVal->GetSubTeamID();
		size_t srcPosInSub = pVal->GetIndexOfSubTeam();

		if (eSrcSubIdx < 0 || eSrcSubIdx >= ESubTeamCount || srcPosInSub >= MAX_TEAM_MEMBER_COUNT)
		{
			return false;
		}


		RsSubTeam* pSrcSubTeam = m_SubTeams.at(eSrcSubIdx);

		pSrcSubTeam->Remove(pVal);
		
		if (pDstVal)
		{
			pDstSubTeam->Remove(pDstVal);
			pSrcSubTeam->Add(pDstVal, srcPosInSub);
		}

		pDstSubTeam->Add(pVal, posInSub);
		return true;
	}

	VoteNS::IFVoteMember* RsTeam::GetVoteLeader() const
	{
		FxRole_RS* pLeader = GetLeader();
		if (NULL == pLeader)
		{
			return NULL;
		}

		return pLeader->GetTeamerInfo();
	}

	bool RsTeam::GetOnLineRoles4Vote(VoteNS::VoteMemberVEC& vecMembers) const
	{
		std::vector<TeamerInfo*> outMembers;
		m_Members.GetOnlineTeamers(outMembers);
		vecMembers.insert(vecMembers.begin(), outMembers.begin(), outMembers.end());
		return true;
	}

	void RsTeam::OnVoteComplate(bool bSuccess)
	{
		m_TripVote.SetStopVoteFlag();
		if (!bSuccess)
		{
			return;
		}
		typedef std::vector<TeamerInfo*> TeamerInfoVEC;
		TeamerInfoVEC vectMember;
		m_Members.GetOnlineTeamers(vectMember);

		for (TeamerInfoVEC::iterator iter1 = vectMember.begin();
			iter1 != vectMember.end(); ++iter1)
		{
			TeamerInfo* pInfo1 = *iter1;
			if (NULL == pInfo1 || NULL == pInfo1->GetRole())
			{
				continue;
			}

			for (TeamerInfoVEC::iterator iter2 = vectMember.begin();
				iter2 != vectMember.end(); ++iter2)
			{
				TeamerInfo* pInfo2 = *iter2;
				if (NULL == pInfo2 || NULL == pInfo2->GetRole())
				{
					continue;
				}

				if (pInfo2 == pInfo1)
				{
					continue;
				}

				pInfo1->GetRole()->ModifyFriendship(pInfo2->GetRole(), eAddFriendshipMode_CopyScene);

			}
		}
	}

	void RsTeam::BroadCastVoteMsg(EErrorCode err)
	{
		FxRSPacketBuilder opb;
		BroadCast(opb.T_TeamMSG_B_D(err), opb.GetPacketBuf());
	}

	void RsTeam::SendVoteInit(VoteNS::IFVoteMember* pMember, const SRoleTripVoteSceneInfo& sInfo)
	{
		if (NULL == pMember)
		{
			return;
		}

		TeamerInfo* pInfo = GetMember(pMember->GetVoteIdentity());
		if (NULL == pInfo)
		{
			return;
		}

		FxRSPacketBuilder opb;
		if (pMember == GetVoteLeader())
		{
			pInfo->SendGS(opb.RoleTripVoteLeaderDlgNtf(sInfo), opb.GetPacketBuf());
		}
		else
		{
			pInfo->SendGS(opb.RoleTripVoteDlgNtf(sInfo), opb.GetPacketBuf());
		}
	}

	TeamerInfo* RsTeam::GetMember(UINT32 dwRoleID) const
	{
		if (INVALID_ROLE_ID == dwRoleID)
		{
			return false;
		}

		TeamerInfo* pInfo = NULL;
		m_Members.foreach2rend(TeamUtils::SCheckID, dwRoleID, pInfo);
		return pInfo;
	}

	VoteNS::T_TripVote* RsTeam::GetVote() const
	{
		return const_cast<VoteNS::T_TripVote*>(&m_TripVote);
	}

	void RsTeam::DropAllRecommend()
	{
		typedef std::set<UINT32> ROLEID_SET;
		ROLEID_SET tTargetIDs;
		mRecordMgr.GetAllIntroducerID(tTargetIDs);

		if (tTargetIDs.empty())
		{
			return;
		}

		FxRSPacketBuilder opb;
		EErrorCode eReason = TeamUtils::SConvertErrcode(TeamEC_RecommendTLeaderChanged, GetTeamType());
		UINT16 wMsgID = opb.T_TeamMSG_B_D(eReason);
		ROLEID_SET::const_iterator iter = tTargetIDs.begin();
		for (; iter != tTargetIDs.end(); ++iter)
		{
			FxRole_RS* pJoin = FxObjFactory::Instance()->FindRole(*iter);
			if (NULL == pJoin || NULL == pJoin->GetTeamerInfo())
			{
				continue;
			}

			pJoin->GetTeamerInfo()->SendGS(wMsgID, opb.GetPacketBuf());
		}

		mRecordMgr.ClearRecommendRecords();
	}

	bool RsTeam::IsEnemyWithMember(const TeamerInfo* pInfo) const
	{
		if (NULL != pInfo && m_Members.foreach1(TeamUtils::SCheckEnemyTeamer, pInfo->GetCamp()))
		{
			return true;
		}

		return false;
	}

	bool RsTeam::LeaveTeam(const TeamerInfo* pTargetTeamInfo)
	{
		if (NULL == Remove( const_cast<TeamerInfo*>(pTargetTeamInfo)))
		{
			return false;
		}

		FxRSPacketBuilder opb;
		BroadCast(opb.T_TeamMemberNtf_B(this), opb.GetPacketBuf());
		UpdateTeam(NULL);
		pTargetTeamInfo->SendGS(opb.T_TeamAuthoritySingleton(pTargetTeamInfo), opb.GetPacketBuf());

		return true;
	}


}
