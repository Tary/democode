#include "StateSingle.h"
#include "factory/objfactory.h"
#include "TeamerInfo.h"
#include "net/gsbuilder.h"
#include "net/gsmgr.h"
#include "TeamUtils.h"
namespace Team
{

	bool SingleState::Arrange(UINT32 dwRoleID, ESubTeamID eSubIdx, UINT8 posInSub)
	{
		return commonNotInTeam();
	}

	bool SingleState::HandlerArrange(TeamerInfo* pExecutor, EErrorCode& eSrcErr)
	{
		eSrcErr = TeamEC_TargetNInTeam;
		return false;
	}

	bool SingleState::AttornREQ(UINT32 dwCandidateRoleID)
	{
		return commonNotInTeam();
	}

	bool SingleState::HandlerAttornREQ(TeamerInfo* pExecutor, EErrorCode& eSrcErr)
	{
		eSrcErr = TeamEC_TargetNInTeam;
		return false;
	}

	bool SingleState::ChangeAssignSchemaREQ(ETeamAssignMethod type, EItemQuality level)
	{
		return commonNotInTeam();
	}

	bool SingleState::ChangeDescREQ(const char* pDesc)
	{
		return commonNotInTeam();
	}

	bool SingleState::CreateTeamREQ()
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			TeamerInfo* pSelfTeamInfo = m_pRoleProxy->GetTeamerInfo();
			if (NULL == pSelfTeamInfo)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}


			RsTeam* pTeam = FxObjFactory::Instance()->T_CreateTeam();
			if (NULL == pTeam)
			{
				eErrcode = TeamEC_CreateTeamFailed;
				break;
			}

			pTeam->Add(pSelfTeamInfo);
			//同步创建队伍
			FxGSMgr::Instance()->BroadCastToGS(opb.T_TeamOPEvent_B(pTeam->GetObjId(), EOrganizationTeam, EOrgOperateCreate), opb.GetPacketBuf(), INVALID_TEAM_ID);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(TeamEC_CreateTeamSuccess), opb.GetPacketBuf());
			pTeam->UpdateTeam(pSelfTeamInfo);

			m_pRoleProxy->SendGS(
				opb.T_TeamAssignChangeNTF_B_D(pTeam->GetAssignMethod(), pTeam->GetAssignLevel(), false),
				opb.GetPacketBuf());

		} while (false);

		if (TeamEC_OK != eErrcode)
		{
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}
		return TeamEC_OK == eErrcode;
	}


	bool SingleState::InviteREQ(UINT32 dwRoleID)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		FxRole_RS* pInvited = (INVALID_ROLE_ID == dwRoleID) ? NULL : FxObjFactory::Instance()->FindRole(dwRoleID);
		do 
		{
			if (NULL == pInvited)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (pInvited->GetRoleState() == RoleState_Logout)
			{
				eErrcode = TeamEC_TargetOffLine;
				break;
			}

			TeamerInfo* pSelfInfo = m_pRoleProxy->GetTeamerInfo();
			if (NULL == pSelfInfo)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			eErrcode = pSelfInfo->CanInvite(dwRoleID);
			if (eErrcode != TeamEC_OK)
			{
				break;
			}
			
			TeamerInfo* pTargetInfo = pInvited->GetTeamerInfo();
			if (NULL == pTargetInfo)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			if (TeamUtils::SCheckEnemyTeamer(pSelfInfo, pTargetInfo->GetCamp()))
			{
				eErrcode = SYSTEM_ERROR_NOT_IN_SAME_CAMP;
				break;
			}

			TeamStateBase* pTargetState = pTargetInfo->GetState();
			if (NULL == pTargetState)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}
			if (!pTargetState->HandlerInviteREQ(pTargetInfo, m_pRoleProxy->GetTeamerInfo(), INVALID_TEAM_ID, EOrganizationTeam, eErrcode))
			{
				break;
			}

			//添加记录
			RequestRecord* pRecord = pSelfInfo->AddInviteRecord(dwRoleID);
			if (NULL == pRecord)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			RequestRecord::SAddAutoRefuseRecord(pRecord, NULL);

			pTargetInfo->SendGS(opb.T_TeamInviteNtf(pSelfInfo), opb.GetPacketBuf());
		} while (false);

		m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode, pInvited->m_szRoleName), opb.GetPacketBuf());

		return eErrcode == TeamEC_OK;
	}

	bool SingleState::HandlerInviteREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pInviter, UINT32 dwOrgID, EOrganizationType eOrgType, EErrorCode& eSrcErr)
	{
		if (m_pRoleProxy->GetRoleState() != RoleState_InGame)
		{
			eSrcErr = TeamEC_TargetOffLine;
		}
		else
		{
			eSrcErr = TeamEC_OK;
		}		
		return eSrcErr == TeamEC_OK;
	}

	/**
	* 指定了队伍ID则 由指定ID的队伍管理员角度处理, 提示发给邀请者
	* 当队伍属于团队时由团队管理员角度处理, 提示仍发给邀请者
	*/
	bool SingleState::InviteRES(UINT32 dwInviterRoleId, ETeamVerify eAgree, UINT32 dwOrgID, EOrganizationType eOrgType)
	{
		EErrorCode eErrcode = TeamEC_OK;
		FxRole_RS* pInviter = (INVALID_ROLE_ID == dwInviterRoleId) ? NULL : FxObjFactory::Instance()->FindRole(dwInviterRoleId);
		do 
		{
			TeamerInfo* pInviterTeamInfo = (NULL == pInviter) ? NULL : pInviter->GetTeamerInfo();
 			if (INVALID_TEAM_ID != dwOrgID)
 			{
				TeamStateBase* pLeaderState = NULL;
				RsTeam* pTeam = CheckOrganization(dwOrgID, eOrgType, eErrcode, pLeaderState);

				do
				{
					if (NULL == pLeaderState || TeamEC_OK != eErrcode)
					{
						break;
					}

					if (NULL == pTeam || (pInviterTeamInfo && !pTeam->IsMember(pInviterTeamInfo->GetRoleID())))
					{//离开队伍则放弃在队伍时的邀请回复
						pInviterTeamInfo = NULL;
					}

					if (!pLeaderState->HandlerInviteRES(pInviterTeamInfo, m_pRoleProxy->GetTeamerInfo(), dwOrgID, eOrgType, eAgree, eErrcode))
					{
						break;
					}
				} while (false);			

				break;//END OF INVITED WITH ORGANIZATION
				//////////////////////////////////////////////////////////////////////////
			}

			if (NULL == pInviter)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (NULL == pInviterTeamInfo)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			TeamStateBase* pInviterState = pInviterTeamInfo->GetState();
			if (NULL == pInviter)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (!pInviterState->HandlerInviteRES(pInviterTeamInfo, m_pRoleProxy->GetTeamerInfo(), dwOrgID, eOrgType, eAgree, eErrcode))
			{
				break;
			}
		} while (false);


		FxRSPacketBuilder opb;

		const char* lpstrName = NULL;
		if (pInviter)
		{
			lpstrName = pInviter->m_szRoleName;
		}
		else
		{
			lpstrName = "";
		}

		if (eAgree == ETeamVerifyRefuse)
		{
			
			m_pRoleProxy->SendGS(opb.T_TeamInviteResAck(TeamEC_OK, eAgree, dwInviterRoleId, lpstrName), opb.GetPacketBuf());
		}
		else
		{
			m_pRoleProxy->SendGS(opb.T_TeamInviteResAck(eErrcode, eAgree, dwInviterRoleId, lpstrName), opb.GetPacketBuf());			
		}

		return TeamEC_OK == eErrcode;
	}

	bool SingleState::HandlerInviteRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pInvited, UINT32 dwOrgID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			if (NULL == pInvited)
			{
				eSrcErr = T_NEW_TEAM_END;
				eErrcode = T_NEW_TEAM_END;
				break;
			}
			TeamerInfo* pSelfTeamInfo = m_pRoleProxy->GetTeamerInfo();
			if (NULL == pSelfTeamInfo)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			TeamerInfo* pInvitedTeamInfo = pInvited->GetTeamerInfo();
			if (NULL == pInvitedTeamInfo)
			{
				eSrcErr = T_NEW_TEAM_END;
				break;
			}

			if (pSelfTeamInfo->DelInviteRecord(pInvited->GetRoleID()) == 0 || (dwOrgID != INVALID_TEAM_ID))
			{// dwOrgID != INVALID_TEAM_ID 不应该出现在这里
				eSrcErr = TeamEC_AnInviteTimeout;//找不到记录
				//eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (eSrcErr != TeamEC_OK)
			{//其他错误
				eErrcode = eSrcErr;
				break;
			}

			if (eAgree == ETeamVerifyRefuse)
			{
				eErrcode = TeamEC_InviteRefuse;
				break;
			}

			if (TeamUtils::SCheckEnemyTeamer(pSelfTeamInfo, pInvitedTeamInfo->GetCamp()))
			{
				eSrcErr = TeamEC_TheTeamExistEnemyCamp;//找不到记录
				eErrcode = TeamEC_EnemyCampWithMember_InviteFailed;
				break;
			}

			RsTeam* pTeam = FxObjFactory::Instance()->T_CreateTeam();
			if (NULL == pTeam)
			{
				eErrcode = eSrcErr = TeamEC_CreateTeamFailed;
				break;
			}

			const RsTeamerContainer* pContainer = pTeam->GetMembers();
			//同步创建队伍
			FxGSMgr::Instance()->BroadCastToGS(opb.T_TeamOPEvent_B(pTeam->GetObjId(), EOrganizationTeam, EOrgOperateCreate), opb.GetPacketBuf(), INVALID_TEAM_ID);
			//Add member and refresh team
			pTeam->Add(pSelfTeamInfo);
			pSelfTeamInfo->OffsetRecordTime(-1);
			pTeam->Add(pInvitedTeamInfo);
			//xxx加入了队伍
			pSelfTeamInfo->SendGS(opb.T_TeamMSG_B_D(TeamEC_CreateTeamSuccess), opb.GetPacketBuf());
			//xxx加入了队伍
			pSelfTeamInfo->SendGS(opb.T_TeamMSG_B_D(TeamEC_NewMemberOfTeam, pInvitedTeamInfo->GetRoleName()), opb.GetPacketBuf());
			
			pInvitedTeamInfo->SendGS(opb.T_TeamMSG_B_D(TeamUtils::SConvertErrcode(TeamEC_HasJoinTeam, pTeam->GetTeamType()), m_pRoleProxy->GetRoleName()),
				opb.GetPacketBuf());

			pTeam->UpdateTeam(pSelfTeamInfo);

			pTeam->BroadCast(opb.T_TeamAssignChangeNTF_B_D(pTeam->GetAssignMethod(), pTeam->GetAssignLevel(), false),
								opb.GetPacketBuf());
		} while (false);

		if (pMsgReceiver)
		{
			pMsgReceiver->SendGS(opb.T_TeamInviteAck(eErrcode, pInvited->GetRole()), opb.GetPacketBuf());
		}		
		return TeamEC_OK == eErrcode;
	}

	bool SingleState::JoinREQ(UINT32 dwRoleID)
	{
		EErrorCode eErrcode = TeamEC_OK;
		FxRole_RS* pTarget = (INVALID_ROLE_ID == dwRoleID) ? NULL : FxObjFactory::Instance()->FindRole(dwRoleID);
		const char* pTargetName = NULL;
		do
		{
			if (NULL == pTarget)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (pTarget->GetRoleState() == RoleState_Logout)
			{
				eErrcode = TeamEC_TargetOffLine;
				break;
			}

			pTargetName = pTarget->m_szRoleName;

			TeamerInfo* pTargetInfo = pTarget->GetTeamerInfo();
			if (NULL == pTargetInfo)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			RsTeam* pTargetTeam = TeamUtils::SGetTeam(pTargetInfo);
			if (pTargetTeam)
			{//转为所在队伍队长
				pTarget = pTargetTeam->GetLeader();
				pTargetInfo = (NULL == pTarget) ? pTargetInfo : pTarget->GetTeamerInfo();
			}

			TeamStateBase* pTargetState = pTargetInfo->GetState();
			if (NULL == pTargetState)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}
			if (!pTargetState->HandlerJoinREQ(pTargetInfo, m_pRoleProxy->GetTeamerInfo(), eErrcode))
			{
				break;
			}
		} while (false);

		FxRSPacketBuilder opb;
		if (eErrcode == TeamEC_OK)
		{
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(TeamEC_JoinSended, pTarget->m_szRoleName), opb.GetPacketBuf());
		}
		else
		{
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode, pTargetName), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool SingleState::HandlerJoinREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pJoiner, EErrorCode& eSrcErr)
	{
		eSrcErr = TeamEC_TargetNInTeam;
		return false;
	}

	bool SingleState::JoinRES(UINT32 dwRoleID, ETeamVerify eAgree, UINT32 dwOrgID, EOrganizationType eOrgType)
	{
		if (eAgree == ETeamVerifyAgree)
		{
			return commonNotInTeam();
		}
		
		return false;
	}

	bool SingleState::HandlerJoinRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pTarget, UINT32 dwOrgID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr)
	{
		if (m_pRoleProxy->GetRoleState() != RoleState_InGame)
		{
			eSrcErr = TeamEC_TargetOffLine;
		}

		return TeamEC_OK == eSrcErr;
	}

	bool SingleState::KickOut(UINT32 dwRoleID)
	{
		return commonNotInTeam();
	}

	bool SingleState::HandlerKickOut(TeamerInfo* pExecutor, EErrorCode& eSrcErr)
	{
		eSrcErr = TeamEC_TargetNInTeam;
		return false;
	}

	bool SingleState::LeaveREQ()
	{
		return commonNotInTeam();
	}

	bool SingleState::TurnToGroupREQ()
	{
		return commonNotInTeam();
	}

	SingleState::SingleState(IFRoleProxy* pProxy)
		: TeamStateBase(pProxy)
	{

	}

	bool SingleState::commonNotInTeam()
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_UNInTeam;
		do
		{
			TeamerInfo* pSelfInfo = m_pRoleProxy->GetTeamerInfo();
			if (NULL == pSelfInfo)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			pSelfInfo->SendGS(opb.T_TeamAuthoritySingleton(pSelfInfo), opb.GetPacketBuf());
		} while (false);


		if (TeamEC_OK != eErrcode)
		{
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}
		return false;
	}

	bool SingleState::OnEnter(ERoleTeamState ePreState)
	{
		TeamStateBase::m_pRoleProxy->ClearSubscribe();

		TeamAuthorityHolder::reset();
		TeamAuthorityHolder::AddAuthority(ETeamAuthorityJoin);
		return true;
	}

	bool SingleState::RecommendREQ(UINT32 dwTargetRoleId)
	{
		return commonNotInTeam();
	}

	bool SingleState::ModifyConfigs(EConfigFlagType eFlagIdx, bool bActive)
	{
		return commonNotInTeam();
	}

	bool SingleState::ModifySubscribe(ESubTeamID eSubIdx, bool bActive)
	{
		return commonNotInTeam();
	}

	bool SingleState::AssignItemREQ(UINT32 dwDstRoleID, UINT32 dwItemID)
	{
		return commonNotInTeam();
	}

	bool SingleState::RecommendRES(UINT32 dwTargetRoleID, const char* pName, ETeamVerify eAgree)
	{
		return commonNotInTeam();
	}

	bool SingleState::HandlerRecommendREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pInviter, EErrorCode& eSrcErr)
	{
		return true;
	}

}
