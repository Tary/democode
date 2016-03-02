#include "StateTeam.h"
#include "factory/objfactory.h"
#include "TeamerInfo.h"
#include "net/gsbuilder.h"
#include "TeamUtils.h"
#include "net/gsmgr.h"
namespace Team
{


	bool TeamState::Arrange(UINT32 dwRoleID, ESubTeamID eSubIdx, UINT8 posInSub)
	{//仅仅允许小队内部调整
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			if (dwRoleID == INVALID_ROLE_ID)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (!checkArrangeSubTeamID(eSubIdx) || posInSub >= MAX_TEAM_MEMBER_COUNT)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			RsTeam* pTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityArrange))
			{
				eErrcode = TeamEC_AuthorityNeedTLeader;
				break;
			}

			FxRole_RS* pTarget = FxObjFactory::Instance()->FindRole(dwRoleID);
			if (NULL == pTarget)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (!pTeam->IsMember(dwRoleID))
			{
				eErrcode = TeamEC_TargetNTeamer;
				break;
			}

			TeamerInfo* pTargetTeamInfo = (NULL == pTarget) ? NULL : pTarget->GetTeamerInfo();
			if (NULL == pTargetTeamInfo || NULL == pTargetTeamInfo->GetState())
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			TeamStateBase* pTargetState = pTargetTeamInfo->GetState();
			convertErrRef(eErrcode);
			if (!pTargetState->HandlerAttornREQ(m_pRoleProxy->GetTeamerInfo(), eErrcode))
			{
				break;
			}

			pTeam->Move(pTargetTeamInfo, eSubIdx, posInSub);
			pTeam->UpdateTeam(NULL);
		} while (false);

		if (TeamEC_OK != eErrcode)
		{
			FxRSPacketBuilder opb;
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::HandlerArrange(TeamerInfo* pExecutor, EErrorCode& eSrcErr)
	{
		//你移动到了....
		return true;
	}

	bool TeamState::AttornREQ(UINT32 dwCandidateRoleID)
	{
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			if (dwCandidateRoleID == INVALID_ROLE_ID)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (m_pRoleProxy->GetRoleID() == dwCandidateRoleID)
			{
				eErrcode = T_NEW_TEAM_END;//客户端BUG
				break;
			}

			RsTeam* pTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityAttorn))
			{
				eErrcode = TeamEC_AuthorityNeedTLeader;
				break;
			}

			FxRole_RS* pTarget = FxObjFactory::Instance()->FindRole(dwCandidateRoleID);
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

			if (!pTeam->IsMember(dwCandidateRoleID))
			{
				eErrcode = TeamEC_TargetNTeamer;
				break;
			}

			TeamerInfo* pTargetTeamInfo = (NULL == pTarget) ? NULL : pTarget->GetTeamerInfo();
			if (NULL != pTargetTeamInfo)
			{
				TeamStateBase* pTargetState = pTargetTeamInfo->GetState();
				if (NULL == pTargetState)
				{	eErrcode = T_NEW_TEAM_END;
					break;
				}

				convertErrRef(eErrcode);
				if (!pTargetState->HandlerArrange(m_pRoleProxy->GetTeamerInfo(), eErrcode))
				{	break;
				}
				pTeam->SetLogicFlag(ELogicFlag_Attorn, true);
				pTeam->UpdateTeam(pTargetTeamInfo);
				pTeam->SetLogicFlag(ELogicFlag_Attorn, false);
			}
		} while (false);

		if (TeamEC_OK != eErrcode)
		{
			FxRSPacketBuilder opb;
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::HandlerAttornREQ(TeamerInfo* pExecutor, EErrorCode& eSrcErr)
	{
		return true;
	}

	bool TeamState::ChangeAssignSchemaREQ(ETeamAssignMethod type, EItemQuality level)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			RsTeam* pTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityMdAssignSchema))
			{
				eErrcode = TeamEC_AuthorityNeedTLeader;
				break;
			}

			bool bRsLevel = pTeam->setAssignLevel(level);
			bool bRsType = pTeam->setAssignMethod(type);
			if (bRsLevel || bRsType)
			{
				//pTeam->UpdateTeam(m_pRoleProxy->GetTeamerInfo());
				pTeam->BroadCast(opb.T_TeamAssignChangeNTF_B_D(type, level, true), opb.GetPacketBuf());
			}
		} while (false);

		if (TeamEC_OK != eErrcode)
		{
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::ChangeDescREQ(const char* pDesc)
	{
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			RsTeam* pTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityMdTeamDesc))
			{
				eErrcode = TeamEC_AuthorityNeedTLeader;
				break;
			}

			if (pDesc == NULL)
			{
				pDesc = "";
			}

			pTeam->setDesc(pDesc);
			pTeam->UpdateTeam(m_pRoleProxy->GetTeamerInfo());
		} while (false);

		if (TeamEC_OK != eErrcode)
		{
			FxRSPacketBuilder opb;
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::CreateTeamREQ()
	{
		FxRSPacketBuilder opb;
		m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(convertErr(TeamEC_InTeam)), opb.GetPacketBuf());
		return false;
	}

	bool TeamState::InviteREQ(UINT32 dwRoleID)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		FxRole_RS* pInvited = (INVALID_ROLE_ID == dwRoleID) ? NULL : FxObjFactory::Instance()->FindRole(dwRoleID);
		const char* pInvitedName = NULL;
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

			pInvitedName = pInvited->m_szRoleName;

			TeamerInfo* pSelfInfo = m_pRoleProxy->GetTeamerInfo();
			if (NULL == pSelfInfo)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			TeamerInfo* pInvitedInfo = pInvited->GetTeamerInfo();
			if (NULL == pInvitedInfo)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			RsTeam* pTeam = TeamUtils::SGetTeam(pSelfInfo);
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (pTeam->IsMember(dwRoleID))
			{
				eErrcode = TeamEC_TargetHasInTeam;
				break;
			}

			if (pTeam->IsEnemyWithMember(pInvitedInfo))
			{
				eErrcode = TeamEC_EnemyCampWithMember_Invite;
				break;
			}

			if (!TeamUtils::IsLeader(pSelfInfo))
			{
				eErrcode = TeamEC_NTLeader;
				break;
			}

			if (pTeam->GetMembers()->FreeCount() == 0)
			{
				eErrcode = TeamEC_MemberLimit4Invite;
				break;
			}
			eErrcode = pSelfInfo->CanInvite(dwRoleID);
			if (eErrcode != TeamEC_OK)
			{
				break;
			}
			eErrcode = pTeam->mRecordMgr.CanInvite(dwRoleID);
			if (eErrcode != TeamEC_OK)
			{
				break;
			}			


			TeamStateBase* pTargetState = pInvitedInfo->GetState();
			if (NULL == pTargetState)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			convertErrRef(eErrcode);
			if (!pTargetState->HandlerInviteREQ(pInvitedInfo, m_pRoleProxy->GetTeamerInfo(), 
				pTeam->GetObjId(), pTeam->GetTeamType(), eErrcode))
			{
				break;
			}

			//添加记录
			RequestRecord* pRecord = pTeam->mRecordMgr.AddInviteRecord(dwRoleID, m_pRoleProxy->GetRoleID());
			if (NULL == pRecord)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			RequestRecord::SAddAutoRefuseRecord(pRecord, pTeam);
			pInvitedInfo->SendGS(opb.T_TeamInviteNtf(pSelfInfo), opb.GetPacketBuf());
		} while (false);

		
		if (eErrcode == TeamEC_OK)
		{
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(TeamEC_InviteSended, pInvited->m_szRoleName), opb.GetPacketBuf());
		}
		else
		{
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode, pInvitedName), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::HandlerInviteREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pInviter, UINT32 dwOrgID, EOrganizationType eOrgType, EErrorCode& eSrcErr)
	{
		eSrcErr = convertErr(TeamEC_TargetInOtherTeam);
		return false;
	}

	bool TeamState::InviteRES(UINT32 dwInviterRoleId, ETeamVerify eAgree, UINT32 dwTeamID, EOrganizationType eOrgType)
	{
		EErrorCode eErrcode = TeamEC_InTeam;
		EErrorCode eInviterErr = TeamEC_TargetHasInTeam;
		FxRole_RS* pInviter = (INVALID_ROLE_ID == dwInviterRoleId) ? NULL : FxObjFactory::Instance()->FindRole(dwInviterRoleId);
		do
		{
			TeamerInfo* pInviterTeamInfo = (NULL == pInviter) ? NULL : pInviter->GetTeamerInfo();
			if (INVALID_TEAM_ID != dwTeamID)
			{
				eErrcode = TeamEC_OK;
				TeamStateBase* pLeaderState = NULL;
				RsTeam* pTeam = TeamStateBase::CheckOrganization(dwTeamID, eOrgType, eErrcode, pLeaderState);
				if (pInviterTeamInfo && pTeam && !pTeam->IsMember(pInviterTeamInfo->GetRoleID()))
				{//离开队伍则放弃在队伍时的邀请回复
					pInviterTeamInfo = NULL;
				}

				eErrcode = TeamEC_InTeam;
				if (NULL == pLeaderState)
				{
					break;
				}

				convertErrRef(eInviterErr);
				if (pLeaderState->HandlerInviteRES(pInviterTeamInfo, m_pRoleProxy->GetTeamerInfo(), 
					dwTeamID, eOrgType, eAgree, eInviterErr))
				{
					break;
				}

				break;//END OF INVITED WITH ORGANIZATION
				//////////////////////////////////////////////////////////////////////////
			}

			if (NULL == pInviter)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (NULL == pInviter->GetTeamerInfo())
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			TeamStateBase* pInviterState = pInviter->GetTeamerInfo()->GetState();
			if (NULL == pInviter)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			convertErrRef(eInviterErr);
			if (pInviterState->HandlerInviteRES(pInviterTeamInfo, m_pRoleProxy->GetTeamerInfo(), 
				dwTeamID, eOrgType, eAgree, eInviterErr))
			{
				break;
			}
		} while (false);

		const char* lpstrName = NULL;
		if (pInviter)
		{
			lpstrName = pInviter->m_szRoleName;
		}
		else
		{
			lpstrName = "";
		}

		FxRSPacketBuilder opb;
		if (eAgree == ETeamVerifyRefuse)
		{

			m_pRoleProxy->SendGS(opb.T_TeamInviteResAck(TeamEC_OK, eAgree, dwInviterRoleId, lpstrName), opb.GetPacketBuf());
		}
		else
		{
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamInviteResAck(eErrcode, eAgree, dwInviterRoleId, lpstrName), opb.GetPacketBuf());
		}

		return TeamEC_OK == eErrcode;
	}

	bool TeamState::HandlerInviteRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pInvited, UINT32 dwTeamID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr)
	{//如果dwOrgID == INVALID_TEAM_ID时可能不是队长,都则一定是队长
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

			TeamerInfo* pTargetTeamInfo = pInvited->GetTeamerInfo();
			if (NULL == pTargetTeamInfo)
			{
				eSrcErr = T_NEW_TEAM_END;
				break;
			}

			RsTeam * pSelfTeam = TeamUtils::SGetTeam(pSelfTeamInfo);
			if (NULL == pSelfTeam)
			{
				eSrcErr = T_NEW_TEAM_END;//没有队伍的情况下竟然处于队伍状态
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			//记录一定要删除
			if (dwTeamID == INVALID_TEAM_ID)
			{//路人时邀请的在自己身上
				if (pSelfTeamInfo->DelInviteRecord(pInvited->GetRoleID()) == 0)
				{//仅路人时邀请的不要求记录
					eSrcErr = TeamEC_AnInviteTimeout;//找不到记录
					//eErrcode = T_NEW_TEAM_END;
					break;
				}
			}
			else 
			{
				RequestRecord* pRecord = pSelfTeam->mRecordMgr.GetInviteRecord(pInvited->GetRoleID());
				EDropReason eDropReason = (NULL == pRecord) ? EDropReason_None : pRecord->m_eDropReason;
				pSelfTeam->mRecordMgr.DelRecommendRecord(pInvited->GetRoleID());
				pSelfTeam->mRecordMgr.DelInviteRecord(pInvited->GetRoleID());
				
				if (eDropReason != EDropReason_None)
				{
					switch (eDropReason)
					{
					case EDropReason_TurnGroup:
						eSrcErr = TeamEC_TargetTeamDismiss;//找不到记录
						break;
					case EDropReason_TimeOut:
						eSrcErr = TeamEC_AnInviteTimeout;
						eErrcode = TeamEC_InviteTimeout;//找不到记录
						break;
					}
					break;
				}

				if (NULL == pRecord)
				{
					eSrcErr = T_NEW_TEAM_END;//找不到记录
					eErrcode = T_NEW_TEAM_END;
					break;
				}

				pRecord = NULL;
			}

			if (eAgree == ETeamVerifyRefuse)
			{
				eErrcode = TeamEC_InviteRefuse;
				break;
			}

			if (pSelfTeam->IsMember(pTargetTeamInfo))
			{
				eErrcode = TeamEC_EnemyCampWithMember_InviteFailed;
				break;
			}

			//对方自己判断不合格,到这就结束了
			if (eSrcErr != TeamEC_OK)
			{//其他错误
				eErrcode = eSrcErr;
				break;
			}

			if (INVALID_TEAM_ID == dwTeamID && !TeamUtils::IsLeader(pSelfTeamInfo))
			{//指定了队伍ID的应该发给队长处理而不是邀请人
				eSrcErr = TeamEC_TargetInOtherTeam;
				break;
			}

			if (pSelfTeam->IsMember(pTargetTeamInfo))
			{
				eErrcode = TeamEC_TargetHasInTeam;
				break;
			}

			if (pSelfTeam->IsEnemyWithMember(pTargetTeamInfo))
			{
				eErrcode = TeamEC_EnemyCampWithMember_InviteFailed;
				eSrcErr = TeamEC_TheTeamExistEnemyCamp;
				break;
			}
			
			const RsTeamerContainer* pContainer = pSelfTeam->GetMembers();

			if (pContainer->FreeCount() == 0 || pSelfTeam->Add(pTargetTeamInfo) == NULL)
			{
				eSrcErr = TeamEC_TargetMemberLimit;
				eErrcode = TeamEC_MemberLimit4InviteFailed;
				break;
			}
			if (pMsgReceiver)
			{
				pMsgReceiver->SendGS(opb.T_TeamInviteAck(TeamEC_InviteAccept, pInvited->GetRole()), opb.GetPacketBuf());
			}
			//xxx加入了队伍
			pContainer->BroadCast(opb.T_TeamMSG_B_D(convertErr(TeamEC_NewMemberOfTeam), pTargetTeamInfo->GetRoleName(), pTargetTeamInfo->GetRoleID()),
				opb.GetPacketBuf());
			pSelfTeam->UpdateTeam(pTargetTeamInfo);	

			pTargetTeamInfo->SendGS(
				opb.T_TeamMSG_B_D(convertErr(TeamEC_HasJoinTeam), m_pRoleProxy->GetRoleName()),
				opb.GetPacketBuf());


			pTargetTeamInfo->SendGS(opb.T_TeamAssignChangeNTF_B_D(pSelfTeam->GetAssignMethod(), pSelfTeam->GetAssignLevel(), false),
				opb.GetPacketBuf());

		} while (false);

		if (pMsgReceiver && eErrcode != TeamEC_OK)
		{
			convertErrRef(eErrcode);
			pMsgReceiver->SendGS(opb.T_TeamInviteAck(eErrcode, pInvited->GetRole()), opb.GetPacketBuf());
		}

		convertErrRef(eSrcErr);
		return TeamEC_OK == eErrcode && TeamEC_OK == eSrcErr;
	}

	bool TeamState::JoinREQ(UINT32 dwRoleID)
	{
		FxRSPacketBuilder opb;
		m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(convertErr(TeamEC_InTeam)), opb.GetPacketBuf());
		return false;
	}

	bool TeamState::HandlerJoinREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pJoiner, EErrorCode& eSrcErr)
	{
		EErrorCode eErrcode = TeamEC_OK;
		if (m_pRoleProxy->GetRoleState() != RoleState_InGame)
		{
			eSrcErr = TeamEC_TargetOffLine;
		}
		else
		{
			eSrcErr = TeamEC_OK;
			do
			{
				if (NULL == pJoiner)
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

				RsTeam * pSelfTeam = TeamUtils::SGetTeam(pSelfTeamInfo);
				if (NULL == pSelfTeam || !TeamUtils::IsLeader(pSelfTeamInfo))
				{
					eSrcErr = T_NEW_TEAM_END;//没有队伍的情况下竟然处于队伍状态
					eErrcode = T_NEW_TEAM_END;
					break;
				}

				if (pSelfTeam->IsEnemyWithMember(pJoiner->GetTeamerInfo()))
				{
					eSrcErr = TeamEC_TheTeamExistEnemyCamp;
					break;
				}

				if (pSelfTeam->CheckConfigFlag(EConfigFlag_ForbiddenJoin))
				{
					eSrcErr = TeamEC_AuthorityJoinForbidden;
					break;
				}

				const RsTeamerContainer* pContainer = pSelfTeam->GetMembers();

				if (pContainer->FreeCount() == 0)
				{
					eSrcErr = TeamEC_TargetMemberLimit;
					break;
				}

				eSrcErr = pSelfTeam->mRecordMgr.CanJoin(pJoiner->GetRoleID());
				if (eSrcErr != TeamEC_OK)
				{
					break;
				}

				//添加记录
				if (NULL == pSelfTeam->mRecordMgr.AddJoinRecord(pJoiner->GetRoleID(), m_pRoleProxy->GetRoleID()))
				{
					eErrcode = T_NEW_TEAM_END;//临时用调试
					break;
				}
			} while (false);


			if (pMsgReceiver && eSrcErr == TeamEC_OK && eErrcode == TeamEC_OK)
			{
				FxRSPacketBuilder opb;
				pMsgReceiver->SendGS(opb.T_TeamJoinNtf(pJoiner, m_pRoleProxy->GetTeamerInfo()), opb.GetPacketBuf());
			}
		}

		convertErrRef(eSrcErr);
		return eSrcErr == TeamEC_OK && eErrcode == TeamEC_OK;
	}

	bool TeamState::JoinRES(UINT32 dwRoleID, ETeamVerify eAgree, UINT32 dwTeamID, EOrganizationType eTeamType)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		EErrorCode eErrcode4Joiner = TeamEC_OK;
		FxRole_RS* pJoiner = (INVALID_ROLE_ID == dwRoleID) ? NULL : FxObjFactory::Instance()->FindRole(dwRoleID);
		do
		{
			TeamerInfo* pJoinerTeamInfo = (NULL == pJoiner) ? NULL : pJoiner->GetTeamerInfo();
			if (INVALID_TEAM_ID == dwTeamID)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			RsTeam* pTeamWhenJoin = FxObjFactory::Instance()->T_FindTeam(dwTeamID);
			RsTeam* pCurrentTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
			if (NULL == pCurrentTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (pCurrentTeam->GetLeader() != m_pRoleProxy->GetRole())
			{
				eErrcode = TeamEC_NTLeader;
				//申请方在队长变更时提示
				break;
			}//优先检查是不是队长, 防止变更队长后, 原申请记录已删除, 申请者重新申请应该在新队长处理时删除

			pCurrentTeam->mRecordMgr.DelJoinRecord(dwRoleID, m_pRoleProxy->GetRoleID());

			if (pCurrentTeam->IsMember(pJoinerTeamInfo))
			{
				eErrcode = TeamEC_TargetHasInTeam;
				break;
			}

			if (pCurrentTeam->IsEnemyWithMember(pJoinerTeamInfo))
			{
				eErrcode4Joiner = TeamEC_TheTeamExistEnemyCamp;
				eErrcode = TeamEC_EnemyCampWithMember_AddFailed;
				break;
			}

			if (pCurrentTeam != pTeamWhenJoin/* || pCurrentTeam->GetOrgType() != eTeamType*/)
			{
				eErrcode = TeamEC_UInOtherTeam;
				/*eErrcode4Joiner = TeamEC_TargetInOtherTeam;
				if (NULL != pTeamWhenJoin)
				{
					convertErrRef(eErrcode4Joiner);
					pJoinerTeamInfo->SendGS(opb.T_TeamJoinAck(eErrcode4Joiner, m_pRoleProxy->GetRole()), opb.GetPacketBuf());
				}*/
				break;
			}

			if (NULL == pJoinerTeamInfo)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (eAgree == ETeamVerifyRefuse)
			{
				pJoinerTeamInfo->SendGS(opb.T_TeamJoinAck(TeamEC_JoinRefuse, m_pRoleProxy->GetRole()), opb.GetPacketBuf());
				break;
			}

			TeamStateBase* pJoinerState = pJoinerTeamInfo->GetState();
			if (NULL == pJoinerState)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			const RsTeamerContainer* pContainer = pCurrentTeam->GetMembers();


			if (pContainer->FreeCount() == 0)
			{
				eErrcode4Joiner = TeamEC_TargetMemberLimit;
				eErrcode = TeamEC_MemberLimit4JoinTarget;
				convertErrRef(eErrcode4Joiner);
				pJoinerTeamInfo->SendGS(opb.T_TeamJoinAck(eErrcode4Joiner, m_pRoleProxy->GetRole()), opb.GetPacketBuf());
				break;
			}

			convertErrRef(eErrcode);
			if (!pJoinerState->HandlerJoinRES(pJoinerTeamInfo, m_pRoleProxy->GetTeamerInfo(), 
				dwTeamID, eTeamType, eAgree, eErrcode))
			{
				break;
			}

			if (pCurrentTeam->Add(pJoinerTeamInfo) == NULL)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			pJoinerTeamInfo->SendGS(opb.T_TeamJoinAck(TeamEC_JoinAccept, m_pRoleProxy->GetRole()), opb.GetPacketBuf());

			//xxx加入了队伍
			pContainer->BroadCast(opb.T_TeamMSG_B_D(convertErr(TeamEC_NewMemberOfTeam), pJoinerTeamInfo->GetRoleName(), pJoinerTeamInfo->GetRoleID()),
				opb.GetPacketBuf());
			pCurrentTeam->UpdateTeam(pJoinerTeamInfo);
			pJoinerTeamInfo->SendGS(opb.T_TeamMSG_B_D(convertErr(TeamEC_HasJoinTeam), m_pRoleProxy->GetRoleName()),
				opb.GetPacketBuf());

			pJoinerTeamInfo->SendGS(opb.T_TeamAssignChangeNTF_B_D(pCurrentTeam->GetAssignMethod(), pCurrentTeam->GetAssignLevel(), false),
				opb.GetPacketBuf());
			break;//END OF INVITED WITH ORGANIZATION
			//////////////////////////////////////////////////////////////////////////
		} while (false);

		const char* lpstrName = NULL;
		if (pJoiner)
		{
			lpstrName = pJoiner->m_szRoleName;
		}
		else
		{
			lpstrName = "";
		}


		if (eAgree == ETeamVerifyRefuse)
		{
			m_pRoleProxy->SendGS(opb.T_TeamJoinResAck(TeamEC_OK, eAgree, dwRoleID, lpstrName), opb.GetPacketBuf());
		}
		else
		{
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamJoinResAck(eErrcode, eAgree, dwRoleID, lpstrName), opb.GetPacketBuf());
		}

		return TeamEC_OK == eErrcode;
	}

	bool TeamState::HandlerJoinRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pTarget, UINT32 dwTeamID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr)
	{
		eSrcErr = TeamEC_TargetHasInTeam;
		convertErrRef(eSrcErr);
		return false;
	}

	bool TeamState::KickOut(UINT32 dwRoleID)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			if (dwRoleID == INVALID_ROLE_ID)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (m_pRoleProxy->GetRoleID() == dwRoleID)
			{
				eErrcode = TeamEC_AuthorityNEnough;
				break;
			}

			RsTeam* pTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityKick))
			{
				eErrcode = TeamEC_AuthorityNeedTLeader;
				break;
			}

			if (!pTeam->IsMember(dwRoleID))
			{
				eErrcode = TeamEC_TargetNTeamer;
				break;
			}
			
			FxRole_RS* pTarget = FxObjFactory::Instance()->FindRole(dwRoleID);
			if (NULL == pTarget)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			TeamerInfo* pTargetTeamInfo = pTarget->GetTeamerInfo();
			if (NULL != pTargetTeamInfo)
			{
				TeamStateBase* pTargetState = pTargetTeamInfo->GetState();
				if (NULL == pTargetState)
				{
					eErrcode = T_NEW_TEAM_END;
					break;
				}

				convertErrRef(eErrcode);
				pTargetState->HandlerKickOut(m_pRoleProxy->GetTeamerInfo(), eErrcode);

				pTeam->LeaveTeam(pTargetTeamInfo);

				pTargetTeamInfo->SendGS(opb.T_TeamMSG_B_D(convertErr(TeamEC_UBeenKickedFromTeam)), opb.GetPacketBuf());
				pTeam->BroadCast(opb.T_TeamMSG_B_D(convertErr(TeamEC_TeamMemberBeenKicked), pTargetTeamInfo->GetRoleName()), opb.GetPacketBuf());
			}
		} while (false);

		if (TeamEC_OK != eErrcode)
		{
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::HandlerKickOut(TeamerInfo* pExecutor, EErrorCode& eSrcErr)
	{
		return true;
	}

	bool TeamState::LeaveREQ()
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			TeamerInfo* pSelfInfo = m_pRoleProxy->GetTeamerInfo();
			if (NULL == pSelfInfo)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			RsTeam* pTeam = TeamUtils::SGetTeam(pSelfInfo);
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			if (pTeam->GetMembers()->Count() == 1)
			{//解散
				pTeam->Dismiss();
				pTeam = NULL;
			}
			else
			{				
				if (!pTeam->LeaveTeam(pSelfInfo))
				{
					eErrcode = T_NEW_TEAM_END;//临时用调试
					break;
				}
				pTeam->GetMembers()->BroadCast(opb.T_TeamMSG_B_D(convertErr(TeamEC_MemberLeaveTeam), pSelfInfo->GetRoleName()), opb.GetPacketBuf());
				pSelfInfo->SendGS(opb.T_TeamMSG_B_D(convertErr(TeamEC_ULeaveTeam)),
					opb.GetPacketBuf());
			}
		} while (false);


		if (TeamEC_OK != eErrcode)
		{
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}	

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::TurnToGroupREQ()
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;

		do 
		{	TeamerInfo* pSelfInfo = m_pRoleProxy->GetTeamerInfo();
			if (NULL == pSelfInfo)
			{	eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			RsTeam* pTeam = TeamUtils::SGetTeam(pSelfInfo);
			if (NULL == pTeam)
			{	eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityKick))
			{
				eErrcode = TeamEC_AuthorityNeedTLeader;
				break;
			}

			pTeam->TurnToGroup();
//			pTeam->mRecordMgr.DropInviteRecord(EDropReason_TurnGroup);

			pTeam->BroadCast(opb.T_TeamMSG_B_D(TeamEC_BecameGroup, m_pRoleProxy->GetRoleName()), opb.GetPacketBuf());

			FxGSMgr::Instance()->BroadCastToGS(
				opb.T_TeamOPEvent_B(pTeam->GetObjId(), EOrganizationTeam, EOrgOperateTurnToGroup),
				opb.GetPacketBuf(), pTeam->GetObjId());

			pTeam->UpdateTeam(NULL);			
		} while (false);

		
		if (TeamEC_OK != eErrcode)
		{
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}
		return TeamEC_OK == eErrcode;
	}

	TeamState::TeamState(IFRoleProxy* pProxy)
		: TeamStateBase(pProxy)
	{

	}

	bool TeamState::OnEnter(ERoleTeamState ePreState)
	{
		TeamStateBase::m_pRoleProxy->ClearSubscribe();
		TeamStateBase::m_pRoleProxy->SetSubscribe(ESubscribe_SubTeamChat_G0, true);
		TeamStateBase::m_pRoleProxy->SetSubscribe(ESubscribe_Common_G0, true);
		TeamStateBase::m_pRoleProxy->SetSubscribe(ESubscribe_Buff_G0, true);

		TeamAuthorityHolder::reset();
		TeamAuthorityHolder::AddAuthority(ETeamAuthorityLeave);
		return true;
	}

	bool TeamState::RecommendREQ(UINT32 dwTargetRoleId)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		FxRole_RS* pRecommended = (INVALID_ROLE_ID == dwTargetRoleId) ? NULL : FxObjFactory::Instance()->FindRole(dwTargetRoleId);
		const char* pRecommendedName = NULL;
		do
		{
			if (NULL == pRecommended)
			{
				eErrcode = TeamEC_TargetNFound;
				break;
			}

			if (pRecommended->GetRoleState() == RoleState_Logout)
			{
				eErrcode = TeamEC_TargetOffLine;
				break;
			}

			pRecommendedName = pRecommended->m_szRoleName;

			TeamerInfo* pSelfInfo = m_pRoleProxy->GetTeamerInfo();
			if (NULL == pSelfInfo)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			TeamerInfo* pRecommendedInfo = pRecommended->GetTeamerInfo();
			if (NULL == pRecommendedInfo || NULL == pRecommendedInfo->GetState())
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			TeamStateBase* pTargetState = pRecommendedInfo->GetState();
			if (!pTargetState->HandlerRecommendREQ(pSelfInfo, pSelfInfo, eErrcode))
			{
				break;
			}

			RsTeam* pTeam = TeamUtils::SGetTeam(pSelfInfo);
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			if (pTeam->IsMember(dwTargetRoleId))
			{
				eErrcode = TeamEC_TargetHasInTeam;//临时用调试
				break;
			}

			if (pTeam->IsEnemyWithMember(pRecommendedInfo))
			{
				eErrcode = TeamEC_EnemyCampWithMember_Recommend;
				break;
			}

			if (TeamUtils::IsLeader(pSelfInfo))
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			if (pTeam->CheckConfigFlag(EConfigFlag_ForbiddenRecommend))
			{
				eErrcode = TeamEC_AuthorityInviteForbidden;
				break;
			}

			if (pTeam->GetMembers()->FreeCount() == 0)
			{
				eErrcode = TeamEC_MemberLimit4Recommend;
				break;
			}

			eErrcode = pTeam->mRecordMgr.CanRecommend(dwTargetRoleId);
			if (eErrcode != TeamEC_OK)
			{
				break;
			}

			

			FxRole_RS* pLeader = pTeam->GetLeader();
			if (NULL == pLeader)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			if (pLeader->GetRoleState() == RoleState_Logout)
			{
				eErrcode = TeamEC_TargetOffLine;//也不该出现
				break;
			}

			if (NULL == pLeader->GetTeamerInfo())
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			//添加记录
			if (NULL == pTeam->mRecordMgr.AddRecommendRecord(dwTargetRoleId, m_pRoleProxy->GetRoleID()))
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}
			pLeader->GetTeamerInfo()->SendGS(opb.T_TeamRecommendNtf(pRecommendedInfo, pSelfInfo), opb.GetPacketBuf());
		} while (false);


		if (eErrcode == TeamEC_OK)
		{
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(TeamEC_RecommendSended, pRecommendedName), opb.GetPacketBuf());
		}
		else
		{
			convertErrRef(eErrcode);
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode, pRecommendedName), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::ModifyConfigs(EConfigFlagType eFlagIdx, bool bActive)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			RsTeam* pTeam = TeamUtils::SGetTeam(TeamStateBase::m_pRoleProxy->GetTeamerInfo());
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (!TeamStateBase::m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityMdConfig))
			{
				eErrcode = TeamEC_AuthorityNeedTLeader;
				break;
			}

			if (pTeam->ModifyConfigFlag(eFlagIdx, bActive))
			{
				pTeam->GetMembers()->BroadCast(opb.T_TeamInfoNtf_B(pTeam, InfoChange_Config), opb.GetPacketBuf());
			}
		} while (false);

		convertErrRef(eErrcode);
		if (TeamEC_OK != eErrcode)
		{
			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::ModifySubscribe(ESubTeamID eSubIdx, bool bActive)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			TeamerInfo* pSelfInfo = m_pRoleProxy->GetTeamerInfo();
			if (NULL == pSelfInfo)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			if (pSelfInfo->GetSubTeamID() == eSubIdx)
			{
				break;
			}

			RsTeam* pTeam = TeamUtils::SGetTeam(pSelfInfo);
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;//临时用调试
				break;
			}

			//pSelfInfo->SetSubscribe(ESubscribeIdx(ESubscribeFlag_SubTeamChat + eSubIdx), bActive);
			pSelfInfo->SetSubscribe(ESubscribeIdx(ESubscribeFlag_Buff + eSubIdx), bActive);
			pTeam->BroadCast(opb.T_TeamMemberNtf_B(pTeam), opb.GetPacketBuf());
			FxGSMgr::Instance()->BroadCastToGS(
				opb.T_TeamOPEvent_B(pTeam->GetObjId(), EOrganizationTeam, EOrgOperateExchange4Subscribe, m_pRoleProxy->GetRoleID()),
				opb.GetPacketBuf(), pTeam->GetObjId());
		} while (false);


		convertErrRef(eErrcode);
		if (TeamEC_OK != eErrcode)
		{
			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::AssignItemREQ(UINT32 dwDstRoleID, UINT32 dwItemID)
	{
		FxRSPacketBuilder opb;
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			if (INVALID_ITEM_ID == dwItemID)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}
			RsTeam* pTeam = TeamUtils::SGetTeam(TeamStateBase::m_pRoleProxy->GetTeamerInfo());
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (!TeamStateBase::m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityAssign))
			{
				eErrcode = TeamEC_AuthorityNeedTLeader;
				break;
			}

			if (INVALID_ROLE_ID != dwDstRoleID && !pTeam->IsMember(dwDstRoleID))
			{
				eErrcode = TeamEC_TargetNTeamer;
				break;
			}

			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamLeaderAssignACK_D(dwDstRoleID, dwItemID), opb.GetPacketBuf());
		} while (false);

		convertErrRef(eErrcode);
		if (TeamEC_OK != eErrcode)
		{
			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}

		return eErrcode == TeamEC_OK;
	}

	bool TeamState::RecommendRES(UINT32 dwTargetRoleID, const char* pName, ETeamVerify eAgree)
	{
		UINT32 dwRecommenderRoleID = INVALID_ROLE_ID;
		EErrorCode eErrcode = TeamEC_OK;
		do
		{
			if (NULL == TeamStateBase::m_pRoleProxy || INVALID_ROLE_ID == dwTargetRoleID)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			RsTeam* pTeam = TeamUtils::SGetTeam(TeamStateBase::m_pRoleProxy->GetTeamerInfo());
			if (NULL == pTeam)
			{
				eErrcode = T_NEW_TEAM_END;
				break;
			}

			if (pTeam->GetLeader() != TeamStateBase::m_pRoleProxy->GetRole())
			{
				break;
			}
			RequestRecord* pRecord = pTeam->mRecordMgr.GetRecommendRecord(dwTargetRoleID);
			if (pRecord)
			{
				dwRecommenderRoleID = pRecord->m_Operator;
				pTeam->mRecordMgr.DelRecommendRecord(dwTargetRoleID);

				if (!pTeam->IsMember(dwRecommenderRoleID))
				{
					dwRecommenderRoleID = INVALID_ROLE_ID;
				}
			}			
		} while (false);

		FxRSPacketBuilder opb;
		if (TeamEC_OK != eErrcode)
		{
			convertErrRef(eErrcode);
			
			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
		}


		if (eAgree == ETeamVerifyAgree)
		{
			InviteREQ(dwTargetRoleID);
		}
		
		if (INVALID_ROLE_ID != dwRecommenderRoleID)
		{
			FxRole_RS* pRecommender = FxObjFactory::Instance()->FindRole(dwRecommenderRoleID);
			FxRole_RS* pTarget = FxObjFactory::Instance()->FindRole(dwTargetRoleID);
			if (pRecommender)
			{
				EErrorCode eError4Recommender = TeamEC_OK;
				if (eAgree == ETeamVerifyAgree)
				{
					eError4Recommender = TeamEC_RecommendTLeaderAccepted;
				}
				else
				{
					eError4Recommender = TeamEC_RecommendTLeaderRefused;
				}

				convertErrRef(eError4Recommender);
				pRecommender->SendGS(opb.T_TeamMSG_B_D(eError4Recommender, pName), opb.GetPacketBuf());
			}
		}

		return eErrcode == TeamEC_OK;
	}

	EErrorCode TeamState::convertErrRef(EErrorCode& eErr) const
	{
		return eErr;
	}

	EErrorCode TeamState::convertErr(EErrorCode eErr) const
	{
		return convertErrRef(eErr);
	}

	bool TeamState::checkArrangeSubTeamID(ESubTeamID eSubIdx) const
	{
		return ESubTeam0 == eSubIdx;
	}

	bool TeamState::HandlerRecommendREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pInviter, EErrorCode& eSrcErr)
	{
		eSrcErr = convertErr(TeamEC_TargetHasInTeam);
		return false;
	}

}
