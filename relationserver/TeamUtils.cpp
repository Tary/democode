#include "TeamUtils.h"
#include "obj/role_rs.h"
#include "config/config.h"

namespace Team
{
	TeamUtils::TeamUtils()
	{
	}
	
	
	TeamUtils::~TeamUtils()
	{
	}


	bool TeamUtils::SFillRoleInfo(const TeamerInfo* pMemberInfo, RSRoleInfoInTeam & outInfo)
	{
		if (NULL != pMemberInfo)
		{
			FxRole_RS* pRole = pMemberInfo->GetRole();
			outInfo.stRoleInfo.stRoleId.dwRoleId = pMemberInfo->GetRoleID();
			outInfo.stRoleInfo.stRoleId.dwObjId = pMemberInfo->GetRoleOBJID();
			outInfo.stRoleInfo.stRoleProp.wLevel = pRole->GetLevel();
			outInfo.stRoleInfo.stRoleProp.dwCareer = pRole->GetCareer();
			outInfo.stRoleInfo.stRoleProp.dwCamp = pRole->GetCamp();
			outInfo.stRoleInfo.stRoleProp.byRace = pRole->GetRace();
			outInfo.stRoleInfo.stRoleProp.stUsingEquipInfo = pRole->GetUsingEquipInfo();
			outInfo.stRoleInfo.stRoleProp.wPortrait = pRole->GetPortrait();
			outInfo.stRoleInfo.stRoleProp.byFashionVisible = pRole->GetFashionVisible();
			outInfo.stRoleInfo.stRoleProp.byHuanqiVisible = pRole->GetHuanqiVisible();
			outInfo.stRoleInfo.stRoleProp.wSceneBaseID = pRole->GetSceneBaseID();
			outInfo.stRoleInfo.stRoleProp.byOnlineState = (pRole->GetRoleState() == RoleState_Logout) ? 1 : 0;
			outInfo.stRoleInfo.stRoleProp.byState = pRole->GetRoleState();
			outInfo.stRoleInfo.stRoleProp.stPos = pRole->GetPos();
			outInfo.stRoleInfo.stRoleProp.dwBattleVal = pRole->GetFightScore();
			SafeArrStrCpy(outInfo.stRoleInfo.stRoleProp.szRoleName, pMemberInfo->GetRoleName());
			outInfo.stRoleInfo.byChannelId = pRole->GetChannelId();
			outInfo.stRoleInfo.byGroupIdx = (UINT8)pMemberInfo->GetSubTeamID();
			outInfo.stRoleInfo.byIndex = pMemberInfo->GetIndexOfSubTeam();
			outInfo.dwSubscribes = pMemberInfo->GetSubscribe();
			outInfo.dwSvrId = pRole->GetSvrId();
		}
		else
		{
			memcpy(&outInfo, 0, sizeof(RSRoleInfoInTeam));
			outInfo.stRoleInfo.stRoleId.dwRoleId = INVALID_ROLE_ID;
			outInfo.stRoleInfo.stRoleId.dwObjId = INVALID_OBJ_ID;
			outInfo.stRoleInfo.stRoleProp.byFashionVisible = false;
			outInfo.stRoleInfo.stRoleProp.byHuanqiVisible = false;
			outInfo.stRoleInfo.stRoleProp.byState = RoleState_Logout;
			outInfo.stRoleInfo.byChannelId = INVALID_CHANNEL_ID;
			outInfo.dwSvrId = INVALID_SERVER_ID;
			outInfo.stRoleInfo.stRoleProp.wSceneBaseID = INVALID_BASE_SCENE_ID;
		}
		return false;
	}

	bool TeamUtils::SFillAuthorityInfo(const TeamerInfo* pTeamer, RoleTeamAuthority& outInfo)
	{
		if (pTeamer)
		{
			outInfo.stRoleId.dwObjId = pTeamer->GetRoleOBJID();
			outInfo.stRoleId.dwRoleId = pTeamer->GetRoleID();
			outInfo.dwAuthorityFlags = pTeamer->GetAllAuthority();
		}
		else
		{
			outInfo.stRoleId.dwObjId = INVALID_OBJ_ID;
			outInfo.stRoleId.dwRoleId = INVALID_ROLE_ID;
			outInfo.dwAuthorityFlags = 0;
		}

		return false;
	}

	RsTeam* TeamUtils::SGetTeam(const FxRole_RS* pRole)
	{
		if (NULL == pRole)
		{
			return NULL;
		}

		return SGetTeam(pRole->GetTeamerInfo());
	}


	RsTeam* TeamUtils::SGetTeam(const TeamerInfo* pInfo)
	{
		if (NULL == pInfo)
		{
			return NULL;
		}

		RsTeamerContainer* pTeamGang = pInfo->GetContainer();
		if (NULL == pTeamGang)
		{
			return NULL;
		}

		return pTeamGang->GetTeam();
	}

	bool TeamUtils::SFillTeamCfg(const RsTeam* pTeam, T_TeamConfig& teamCfg)
	{
		memset(&teamCfg, 0, sizeof(T_TeamConfig));
		if (pTeam)
		{
			teamCfg.byAssignMethod = pTeam->GetAssignMethod();
			teamCfg.byAssignLevel = pTeam->GetAssignLevel();

			FxRole_RS* pLeader = pTeam->GetLeader();
			teamCfg.dwTeamLeaderId = (NULL == pLeader) ? INVALID_ROLE_ID : pLeader->GetRoleId();
			teamCfg.dwMaxFriendShip = pTeam->getFriendShip();
			teamCfg.byTeamState = (UINT8)pTeam->GetTeamType();
			teamCfg.byFlags = (UINT8)pTeam->ConfigFlagToUL();
			SafeArrStrCpy(teamCfg.szTeamDesc, pTeam->GetDesc().c_str());
			teamCfg.dwTeamId = pTeam->GetObjId();
		}
		else
		{
			teamCfg.byAssignMethod = TeamAssign_Free;
			teamCfg.byAssignLevel = ItemQuality_Random;
			teamCfg.dwTeamLeaderId = INVALID_ROLE_ID;
			teamCfg.byTeamState = EOrganizationInvalid;
			teamCfg.dwTeamId = INVALID_TEAM_ID;
			teamCfg.byFlags = 0xff;
			teamCfg.dwMaxFriendShip = 0;
		}
		return true;
	}

// 	RsGroup* TeamUtils::SGetGroup(const FxRole_RS* pRole)
// 	{
// 		return SGetGroup(SGetTeam(pRole));
// 	}
// 
// 	RsGroup* TeamUtils::SGetGroup(const RsTeam* pTeam)
// 	{
// 		if (NULL == pTeam || NULL == pTeam->GetGroupGang())
// 		{
// 			return NULL;
// 		}
// 
// 		return pTeam->GetGroupGang()->GetGroup();
// 	}
// 
// 	RsGroup* TeamUtils::SGetGroup(const TeamerInfo* pInfo)
// 	{
// 		return SGetGroup(SGetTeam(pInfo));
// 	}

	bool TeamUtils::IsLeader(const FxRole_RS* pRole)
	{
		if (NULL == pRole || NULL == pRole->GetTeamerInfo())
		{
			return false;
		}

		return IsLeader(pRole->GetTeamerInfo());
	}

	bool TeamUtils::IsLeader(const TeamerInfo* pInfo)
	{
		if (NULL == pInfo || pInfo ->getStateType() == ERoleTeamStateSingle || NULL == pInfo->GetContainer())
		{
			return false;
		}

		RsTeam* pTeam = pInfo->GetContainer()->GetTeam();
		if (NULL == pTeam || NULL == pTeam->GetLeader())
		{
			return false;
		}

		return pTeam->GetLeader()->GetTeamerInfo() == pInfo;
	}

	bool TeamUtils::SCheckID(const TeamerInfo* pInfo, UINT32 dwRoleID, TeamerInfo*& rpResult)
	{
		rpResult = NULL;
		if (NULL != pInfo && pInfo->GetRoleID() == dwRoleID)
		{
			rpResult = const_cast<TeamerInfo*>(pInfo);
		}
		return rpResult != NULL;
	}

	bool TeamUtils::SCheckTeamerState(const TeamerInfo* pInfo, ERoleState eState, TeamerInfo*& rpResult)
	{
		rpResult = NULL;

		if (NULL != pInfo && pInfo->GetRoleState() == eState)
		{
			rpResult = const_cast<TeamerInfo*>(pInfo);
			return true;
		}

		return false;
	}

	bool TeamUtils::SSelectEarliestValidState(const TeamerInfo* pInfo, ERoleState eState, TeamerInfo*& rpResult)
	{
		if (NULL != pInfo && pInfo->GetRoleState() == eState)
		{
			TeamerInfo* pCurResult = const_cast<TeamerInfo*>(pInfo);
			if (NULL == rpResult || pCurResult->GetRecordTime() < rpResult->GetRecordTime())
			{
				rpResult = pCurResult;
			}
			return true;
		}

		return false;
	}

	bool TeamUtils::SSelectMemberByID(const RsTeam* pTeam, UINT32 dwRoleID, TeamerInfo*& rpResult)
	{
		rpResult = NULL;
		if (NULL == pTeam)
		{
			return false;
		}

		pTeam->GetMembers()->foreach2rend(TeamUtils::SCheckID, dwRoleID, rpResult);
		return NULL != rpResult;
	}

	bool TeamUtils::SSelectTeamerByState(const RsTeam* pTeam, ERoleState& eState, TeamerInfo*& rpResult)
	{
		rpResult = NULL;
		if (NULL == pTeam)
		{
			return false;
		}

		pTeam->GetMembers()->foreach2rend(TeamUtils::SCheckTeamerState, eState, rpResult);
		return NULL != rpResult;
	}

	bool TeamUtils::CollectTeamerFunc(TeamerInfo* pVal, std::vector<TeamerInfo*>& outResult)
	{
		if (pVal)
		{
			outResult.push_back(pVal);
		}

		return false;
	}

	bool TeamUtils::SFillTeamersInfo(const RsTeam* pTeam, RSTeamersInfo& teamCfg)
	{
		memset(&teamCfg, 0, sizeof(RSTeamersInfo));
		if (pTeam)
		{
			teamCfg.byTeamMemberCount = (UINT8)pTeam->GetMembers()->foreach_ra(SFillRoleInfo,
				teamCfg.astTeamMembers, (size_t)MAX_TEAM_MEMBER_COUNT_T);
		}

		return true;
	}

	bool TeamUtils::SFillSubscribeInfo(const TeamerInfo* pMemberInfo, UINT32& outInfo)
	{
		if (NULL != pMemberInfo)
		{
			outInfo = pMemberInfo->GetSubscribe();
		}
		else
		{
			outInfo = 0;
		}
		
		return false;
	}

	EErrorCode TeamUtils::SConvertErrcode(EErrorCode srcCode, EOrganizationType eTeamType)
	{
		if (eTeamType == EOrganizationTeam)
		{
			return srcCode;
		}

		switch (srcCode)
		{
		case TeamEC_TargetNInTeam:
			srcCode = TeamEC_TargetNInGroup;
			break;
		case TeamEC_TargetHasInTeam:
			srcCode = TeamEC_TargetHasInGroup;
			break;
		case TeamEC_TargetTeamDismiss:
			srcCode = TeamEC_TargetGroupDismiss;
			break;
		case TeamEC_TeamMemberBeenKicked:
			srcCode = TeamEC_GroupMemberBeenKicked;
			break;			
		case TeamEC_TargetInOtherTeam:
			srcCode = TeamEC_TargetInOtherGroup;
			break;
		case TeamEC_TargetNTLeaderAnyM:
			srcCode = TeamEC_TargetNGLeaderAnyM;
			break;
		case TeamEC_TLeaderChanged:
			srcCode = TeamEC_GLeaderChanged;
			break;
		case TeamEC_UBecameTLeader:
			srcCode = TeamEC_UBecameGLeader;
			break;
		case TeamEC_InTeam:
			srcCode = TeamEC_InGroup;
			break;
		case TeamEC_JoinTLeaderChanged:
			srcCode = TeamEC_JoinGLeaderChanged;
			break;
		case TeamEC_ULeaveTeamBecauseEnemyCamp:
			srcCode = TeamEC_ULeaveGroupBecauseEnemyCamp;
			break;
		case TeamEC_LeaveTeamBecauseEnemyCamp:
			srcCode = TeamEC_LeaveGroupBecauseEnemyCamp;
			break;
		case TeamEC_AuthorityNeedTLeader:
			srcCode = TeamEC_AuthorityNeedGLeader;
			break;
		case TeamEC_NTLeader:
			srcCode = TeamEC_NGLeader;
			break;
		case TeamEC_TeamDismiss:
			srcCode = TeamEC_GroupDismiss;
			break;
		case TeamEC_HasJoinTeam:
			srcCode = TeamEC_HasJoinGroup;
			break;
		case TeamEC_NewMemberOfTeam:
			srcCode = TeamEC_NewMemberOfGroup;
			break;
		case TeamEC_MemberLeaveTeam:
			srcCode = TeamEC_MemberLeaveGroup;
			break;
		case TeamEC_ULeaveTeam:
			srcCode = TeamEC_ULeaveGroup;
			break;
		case TeamEC_RecommendTLeaderChanged:
			srcCode = TeamEC_RecommendGLeaderChanged;
			break;
		case TeamEC_UBeenKickedFromTeam:
			srcCode = TeamEC_UBeenKickedFromGroup;
			break;
		case TeamEC_UNInTeam:
			srcCode = TeamEC_UNInGroup;
			break;

		case TeamEC_WaitLeaderAssign:
			srcCode = TeamEC_WaitGLeaderAssign;
			break;
		case TeamEC_RecommendTLeaderRefused:
			srcCode = TeamEC_RecommendGLeaderRefused;
			break;
		case TeamEC_RecommendTLeaderAccepted:
			srcCode = TeamEC_RecommendGLeaderAccepted;
			break;
		default:
			break;
		}
		return srcCode;
	}

	void TeamUtils::SUpdateMaxFriendship(const TeamerInfo* pInfo1, const TeamerInfo* pInfo2, UINT32& dwVal)
	{
		if (NULL == pInfo1 || NULL == pInfo2)
		{
			return;
		}

		FxRole_RS* poRole1 = pInfo1->GetRole();
		FxRole_RS* poRoleNext = pInfo2->GetRole();

		if (NULL == poRole1 || NULL == pInfo2)
		{
			return;
		}

		if ((poRole1->GetRoleState() == RoleState_InGame) && (pInfo2->GetRoleState() == RoleState_InGame))
		{//在线才算进去
			UINT32 dwFreindShip = poRole1->GetFriends().GetFriendShip(pInfo2->GetRoleID());
			if (dwFreindShip > dwVal)
			{
				dwVal = dwFreindShip;
			}
		}
	}

	bool TeamUtils::SIsSameInfo(const TeamerInfo* pInfo, const TeamerInfo* pComInfo)
	{
		return pInfo == pComInfo;
	}

	bool TeamUtils::CollectRecord(const RequestRecord* pRecord, RequestRecordVEC& outResults)
	{
		if (pRecord)
		{
			outResults.push_back(pRecord);
		}

		return false;
	}

	bool TeamUtils::CollectRecordID(const RequestRecord* pRecord, std::set<UINT32>& outResults)
	{
		if (pRecord && INVALID_ROLE_ID != pRecord->m_TargetRoleID)
		{
			outResults.insert(pRecord->m_TargetRoleID);
		}

		return false;
	}

	bool TeamUtils::CollectRecordOperatorID(const RequestRecord* pRecord, std::set<UINT32>& outResults)
	{
		if (pRecord && INVALID_ROLE_ID != pRecord->m_Operator)
		{
			outResults.insert(pRecord->m_Operator);
		}

		return false;
	}

	bool TeamUtils::SCheckEnemyTeamer(const TeamerInfo* pInfo, UINT32 dwTargetCamp)
	{
		if (GroupRelation_Enemy == FxConfig::Instance()->GetGroupRelationship(pInfo->GetCamp(), dwTargetCamp))
		{
			return true;
		}

		return false;
	}

}
