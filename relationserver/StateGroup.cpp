#include "StateGroup.h"
#include "net/gsbuilder.h"
#include "factory/objfactory.h"
#include "TeamUtils.h"
#include "net/gsmgr.h"
namespace Team
{

// 	bool GroupState::Arrange(UINT32 dwRoleID, ESubTeamID eSubIdx, UINT8 posInSub)
// 	{
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			if (dwRoleID == INVALID_ROLE_ID)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			if (eSubIdx < 0 || eSubIdx >= ESubTeamCount || posInSub >= MAX_TEAM_MEMBER_COUNT)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			RsTeam* pTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
// 			if (NULL == pTeam)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityArrange))
// 			{
// 				eErrcode = TeamEC_AuthorityNeedTLeader;
// 				break;
// 			}
// 
// 			FxRole_RS* pTarget = FxObjFactory::Instance()->FindRole(dwRoleID);
// 			if (NULL == pTarget)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			if (!pTeam->IsMember(dwRoleID))
// 			{
// 				eErrcode = TeamEC_TargetNTeamer;
// 				break;
// 			}
// 
// 			TeamerInfo* pTargetTeamInfo = (NULL == pTarget) ? NULL : pTarget->GetTeamerInfo();
// 			if (NULL == pTargetTeamInfo || NULL == pTargetTeamInfo->GetState())
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			TeamStateBase* pTargetState = pTargetTeamInfo->GetState();
// 			convertErrRef(eErrcode);
// 			if (!pTargetState->HandlerAttornREQ(m_pRoleProxy->GetTeamerInfo(), eErrcode))
// 			{
// 				break;
// 			}
// 
// 			pTeam->Move(pTargetTeamInfo, eSubIdx, posInSub);
// 			pTeam->UpdateTeam(NULL);
// 		} while (false);
// 
// 		if (TeamEC_OK != eErrcode)
// 		{
// 			FxRSPacketBuilder opb;
// 			convertErrRef(eErrcode);
// 			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
//
// 	bool GroupState::HandlerArrange(TeamerInfo* pExecutor, EErrorCode& eSrcErr)
// 	{
// 		return true;
// 	}
//
// 	bool GroupState::AttornREQ(UINT32 dwCandidateRoleID)
// 	{
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			if (dwCandidateRoleID == INVALID_ROLE_ID)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			if (m_pRoleProxy->GetRoleID() == dwCandidateRoleID)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//客户端BUG
// 				break;
// 			}
// 
// 			RsTeam* pTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
// 			if (NULL == pTeam)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityAttorn))
// 			{
// 				eErrcode = TeamEC_AuthorityNeedTLeader;
// 				break;
// 			}
// 
// 			FxRole_RS* pTarget = FxObjFactory::Instance()->FindRole(dwCandidateRoleID);
// 			if (NULL == pTarget || pTarget->GetRoleState() == RoleState_Logout)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			if (!pTeam->IsMember(dwCandidateRoleID))
// 			{
// 				eErrcode = TeamEC_TargetNTeamer;
// 				break;
// 			}
// 
// 			TeamerInfo* pTargetTeamInfo = (NULL == pTarget) ? NULL : pTarget->GetTeamerInfo();
// 			if (NULL != pTargetTeamInfo)
// 			{
// 				TeamStateBase* pTargetState = pTargetTeamInfo->GetState();
// 				if (NULL == pTargetState)
// 				{
// 					eErrcode = T_NEW_TEAM_END;
// 					break;
// 				}
// 
// 				convertErrRef(eErrcode);
// 				if (!pTargetState->HandlerAttornREQ(m_pRoleProxy->GetTeamerInfo(), eErrcode))
// 				{
// 					break;
// 				}
// 
// 				pTeam->SetLogicFlag(ELogicFlag_Attorn, true);
// 				pTeam->UpdateTeam(pTargetTeamInfo);
// 				pTeam->SetLogicFlag(ELogicFlag_Attorn, false);
// 			}
// 		} while (false);
// 
// 		if (TeamEC_OK != eErrcode)
// 		{
// 			FxRSPacketBuilder opb;
// 			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
//
// 	bool GroupState::HandlerAttornREQ(TeamerInfo* pExecutor, EErrorCode& eSrcErr)
// 	{
// 		RsTeam* pTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
// 		if (NULL == pTeam)
// 		{
// 			eSrcErr = T_NEW_TEAM_END;
// 		}
// 
// 		return eSrcErr == TeamEC_OK;
// 	}
//
// 	bool GroupState::ChangeAssignSchemaREQ(ETeamAssignMethod type, EItemQuality level)
// 	{
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			RsTeam* pGroup = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
// 			if (NULL == pGroup)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityMdAssignSchema))
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_AuthorityNeedTLeader, pGroup->GetOrgType());
// 				break;
// 			}
// 
// 			bool bRsLevel = pGroup->setAssignLevel(level);
// 			bool bRsType = pGroup->setAssignMethod(type);
// 			if (bRsLevel || bRsType)
// 			{
// 				pGroup->UpdateTeam(m_pRoleProxy->GetTeamerInfo());
// 			}			
// 		} while (false);
// 
// 		if (TeamEC_OK != eErrcode)
// 		{
// 			FxRSPacketBuilder opb;
// 			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
//
// 	bool GroupState::ChangeDescREQ(const char* pDesc)
// 	{
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			RsTeam* pGroup = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
// 			if (NULL == pGroup)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityMdTeamDesc))
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_AuthorityNeedTLeader, pGroup->GetOrgType());
// 				break;
// 			}
// 
// 			if (pDesc == NULL)
// 			{
// 				pDesc = "";
// 			}
// 
// 			pGroup->setDesc(pDesc);
// 			pGroup->UpdateTeam(m_pRoleProxy->GetTeamerInfo());
// 		} while (false);
// 
// 		if (TeamEC_OK != eErrcode)
// 		{
// 			FxRSPacketBuilder opb;
// 			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
//
// 	bool GroupState::CreateTeamREQ()
// 	{
// 		FxRSPacketBuilder opb;
// 		TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(TeamEC_InGroup), opb.GetPacketBuf());
// 		return false;
// 	}
//
// 	bool GroupState::InviteREQ(UINT32 dwRoleID)
// 	{
// 		FxRSPacketBuilder opb;
// 		EErrorCode eErrcode = TeamEC_OK;
// 		FxRole_RS* pInvited = (INVALID_ROLE_ID == dwRoleID) ? NULL : FxObjFactory::Instance()->FindRole(dwRoleID);
// 		const char* pInvitedName = NULL;
// 		do
// 		{
// 			if (NULL == pInvited)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			pInvitedName = pInvited->m_szRoleName;
// 
// 			TeamerInfo* pSelfInfo = TeamStateBase::m_pRoleProxy->GetTeamerInfo();
// 			if (NULL == pSelfInfo)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			TeamerInfo* pInvitedInfo = pInvited->GetTeamerInfo();
// 			if (NULL == pInvitedInfo)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			RsTeam* pGroup = TeamUtils::SGetTeam(pSelfInfo);
// 			if (NULL == pGroup)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			if (pGroup->IsMember(dwRoleID))
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_TargetHasInTeam, pGroup->GetOrgType());
// 				break;
// 			}
// 
// 			if (pGroup->IsEnemyWithMember(pInvitedInfo))
// 			{
// 				eErrcode = TeamEC_EnemyCampWithMember_Invite;
// 				break;
// 			}
// 
// 			if (!TeamUtils::IsLeader(pSelfInfo))
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_NTLeader, pGroup->GetOrgType());
// 				break;
// 			}
// 
// 			if (pGroup->GetMembers()->FreeCount() == 0)
// 			{
// 				eErrcode = TeamEC_MemberLimit4Invite;
// 				break;
// 			}
// 			eErrcode = pSelfInfo->CanInvite(dwRoleID);
// 			if (eErrcode != TeamEC_OK)
// 			{
// 				break;
// 			}
// 			eErrcode = pGroup->mRecordMgr.CanInvite(dwRoleID);
// 			if (eErrcode != TeamEC_OK)
// 			{
// 				break;
// 			}
// 
// 			TeamStateBase* pTargetState = pInvitedInfo->GetState();
// 			if (NULL == pTargetState)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			if (!pTargetState->HandlerInviteREQ(pInvitedInfo, TeamStateBase::m_pRoleProxy->GetTeamerInfo(), 
// 				INVALID_TEAM_ID, EOrganizationGroup, eErrcode))
// 			{
// 				break;
// 			}
// 
// 			//添加记录
// 			if (NULL == pGroup->mRecordMgr.AddInviteRecord(dwRoleID, TeamStateBase::m_pRoleProxy->GetRoleID()))
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 			pInvitedInfo->SendGS(opb.T_TeamInviteNtf(pSelfInfo), opb.GetPacketBuf());
// 		} while (false);
// 
// 		
// 		if (eErrcode == TeamEC_OK)
// 		{
// 			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(TeamEC_InviteSended, pInvited->m_szRoleName), opb.GetPacketBuf());
// 		}
// 		else
// 		{
// 			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode, pInvitedName), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
// 
// 	bool GroupState::HandlerInviteREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pInviter, UINT32 dwOrgID, EOrganizationType eOrgType, EErrorCode& eSrcErr)
// 	{
// 		eSrcErr = TeamEC_TargetInOtherTeam;
// 		return false;
// 	}
//
// 	bool GroupState::InviteRES(UINT32 dwInviterRoleId, ETeamVerify eAgree, UINT32 dwOrgID, EOrganizationType eOrgType)
// 	{
// 		EErrorCode eErrcode = TeamEC_InGroup;
// 		EErrorCode eInvitedErr = TeamEC_TargetHasInGroup;
// 		FxRole_RS* pInviter = (INVALID_ROLE_ID == dwInviterRoleId) ? NULL : FxObjFactory::Instance()->FindRole(dwInviterRoleId);
// 		do
// 		{
// 			TeamerInfo* pInviterTeamInfo = (NULL == pInviter) ? NULL : pInviter->GetTeamerInfo();
// 			if (INVALID_TEAM_ID != dwOrgID)
// 			{
// 				eErrcode = TeamEC_OK;
// 				TeamStateBase* pLeaderState = NULL;
// 				RsTeam* pTeam = TeamStateBase::CheckOrganization(dwOrgID, eOrgType, eErrcode, pLeaderState);
// 				if (pInviterTeamInfo && pTeam && !pTeam->IsMember(pInviterTeamInfo->GetRoleID()))
// 				{//离开队伍则放弃在队伍时的邀请回复
// 					pInviterTeamInfo = NULL;
// 				}
// 
// 				eErrcode = TeamEC_InGroup;
// 				if (NULL == pLeaderState)
// 				{
// 					break;
// 				}
// 
// 				if (!pLeaderState->HandlerInviteRES(pInviterTeamInfo, TeamStateBase::m_pRoleProxy->GetTeamerInfo(), dwOrgID, eOrgType, eAgree, eInvitedErr))
// 				{
// 					break;
// 				}
// 
// 				break;//END OF INVITED WITH ORGANIZATION
// 				//////////////////////////////////////////////////////////////////////////
// 			}
// 
// 			if (NULL == pInviter)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			if (NULL == pInviter->GetTeamerInfo())
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			TeamStateBase* pInviterState = pInviter->GetTeamerInfo()->GetState();
// 			if (NULL == pInviter)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 			if (!pInviterState->HandlerInviteRES(pInviterTeamInfo, TeamStateBase::m_pRoleProxy->GetTeamerInfo(), 
// 				dwOrgID, eOrgType, eAgree, eInvitedErr))
// 			{
// 				break;
// 			}
// 		} while (false);
// 
// 		const char* lpstrName = NULL;
// 		if (pInviter)
// 		{
// 			lpstrName = pInviter->m_szRoleName;
// 		}
// 		else
// 		{
// 			lpstrName = "";
// 		}
// 
// 		FxRSPacketBuilder opb;
// 		if (eAgree == ETeamVerifyRefuse)
// 		{
// 
// 			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamInviteResAck(TeamEC_OK, eAgree, dwInviterRoleId, lpstrName), opb.GetPacketBuf());
// 		}
// 		else
// 		{
// 			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamInviteResAck(eErrcode, eAgree, dwInviterRoleId, lpstrName), opb.GetPacketBuf());
// 		}
// 
// 		return TeamEC_OK == eErrcode;
// 	}
// 
// 	bool GroupState::HandlerInviteRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pInvited, UINT32 dwTeamID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr)
// 	{
// 		//如果dwOrgID == INVALID_TEAM_ID时可能不是队长,都则一定是队长
// 		FxRSPacketBuilder opb;
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			if (NULL == pInvited)
// 			{
// 				eSrcErr = T_NEW_TEAM_END;
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 			TeamerInfo* pSelfTeamInfo = m_pRoleProxy->GetTeamerInfo();
// 			if (NULL == pSelfTeamInfo)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			TeamerInfo* pTargetTeamInfo = pInvited->GetTeamerInfo();
// 			if (NULL == pTargetTeamInfo)
// 			{
// 				eSrcErr = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			RsTeam * pSelfGroup = TeamUtils::SGetTeam(pSelfTeamInfo);
// 			if (NULL == pSelfGroup || pSelfGroup->GetOrgType() != EOrganizationGroup)
// 			{
// 				eSrcErr = T_NEW_TEAM_END;//没有队伍的情况下竟然处于队伍状态
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			//记录一定要删除
// 			if (dwTeamID == INVALID_TEAM_ID)
// 			{//路人时邀请的在自己身上
// 				if (pSelfTeamInfo->DelInviteRecord(pInvited->GetRoleID()) == 0)
// 				{//仅路人时邀请的不要求记录
// 					eSrcErr = T_NEW_TEAM_END;//找不到记录
// 					eErrcode = T_NEW_TEAM_END;
// 					break;
// 				}
// 			}
// 			else 
// 			{
// 				RequestRecord* pRecord = pSelfGroup->mRecordMgr.GetInviteRecord(pInvited->GetRoleID());
// 				EDropReason eDropReason = (NULL == pRecord) ? EDropReason_None : pRecord->m_eDropReason;
// 				pSelfGroup->mRecordMgr.DelRecommendRecord(pInvited->GetRoleID());
// 				pSelfGroup->mRecordMgr.DelInviteRecord(pInvited->GetRoleID());
// 
// 				if (eDropReason != EDropReason_None)
// 				{
// 					switch (eDropReason)
// 					{
// 					case EDropReason_TurnGroup:
// 						eSrcErr = TeamEC_TargetTeamDismiss;//找不到记录
// 						break;
// 					case EDropReason_TimeOut:
// 						eSrcErr = TeamEC_AnInviteTimeout;
// 						eErrcode = TeamEC_InviteTimeout;//找不到记录
// 						break;
// 					}
// 					break;
// 				}
// 
// 				if (NULL == pRecord)
// 				{
// 					eSrcErr = T_NEW_TEAM_END;//找不到记录
// 					eErrcode = T_NEW_TEAM_END;
// 					break;
// 				}
// 
// 				pRecord = NULL;
// 			}
// 			//对方自己判断不合格,到这就结束了
// 			if (eSrcErr != TeamEC_OK)
// 			{//其他错误
// 				eErrcode = eSrcErr;
// 				break;
// 			}
// 
// 			if (eAgree == ETeamVerifyRefuse)
// 			{
// 				eErrcode = TeamEC_InviteRefuse;
// 				break;
// 			}
// 
// 			if (INVALID_TEAM_ID == dwTeamID && !TeamUtils::IsLeader(pSelfTeamInfo))
// 			{//指定了队伍ID的应该发给队长处理而不是邀请人
// 				eSrcErr = TeamUtils::SConvertErrcode(TeamEC_TargetInOtherTeam, pSelfGroup->GetOrgType());
// 				break;
// 			}
// 
// 			if (pSelfGroup->IsEnemyWithMember(pTargetTeamInfo))
// 			{
// 				eErrcode = TeamEC_EnemyCampWithMember_InviteFailed;
// 				eSrcErr = TeamEC_TheTeamExistEnemyCamp;
// 				break;
// 			}
// 
// 			const RsTeamerContainer* pContainer = pSelfGroup->GetMembers();
// 			if (pContainer->FreeCount() == 0 || pSelfGroup->Add(pTargetTeamInfo) == NULL)
// 			{
// 				eSrcErr = TeamEC_TargetMemberLimit;
// 				eErrcode = TeamEC_MemberLimit4InviteFailed;
// 				break;
// 			}
// 			
// 			if (pMsgReceiver)
// 			{
// 				pMsgReceiver->SendGS(opb.T_TeamInviteAck(TeamEC_InviteAccept, pInvited->GetRole()), opb.GetPacketBuf());
// 			}
// 
// 			//xxx加入了队伍
// 			pContainer->BroadCast(opb.T_TeamMSG_B_D(TeamEC_NewMemberOfGroup, pTargetTeamInfo->GetRoleName(), pTargetTeamInfo->GetRoleID()), 
// 				opb.GetPacketBuf());
// 			pSelfGroup->UpdateTeam(pTargetTeamInfo);
// 
// 			pTargetTeamInfo->SendGS(opb.T_TeamMSG_B_D(TeamUtils::SConvertErrcode(TeamEC_HasJoinTeam, eOrgType), m_pRoleProxy->GetRoleName()),
// 				opb.GetPacketBuf());
// 		} while (false);
// 
// 		if (pMsgReceiver && TeamEC_OK != eErrcode)
// 		{
// 			pMsgReceiver->SendGS(opb.T_TeamInviteAck(eErrcode, pInvited->GetRole()), opb.GetPacketBuf());
// 		}
// 		return TeamEC_OK == eErrcode && TeamEC_OK == eSrcErr;
// 	}
// 
// 	bool GroupState::JoinREQ(UINT32 dwRoleID)
// 	{
// 		FxRSPacketBuilder opb;
// 		TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(TeamEC_InGroup), opb.GetPacketBuf());
// 		return false;
// 	}
// 
// 	bool GroupState::HandlerJoinREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pJoiner, EErrorCode& eSrcErr)
// 	{
// 		EErrorCode eErrcode = TeamEC_OK;
// 		if (m_pRoleProxy->GetRoleState() != RoleState_InGame)
// 		{
// 			eSrcErr = TeamEC_TargetOffLine;
// 		}
// 		else
// 		{
// 			eSrcErr = TeamEC_OK;
// 			do
// 			{
// 				if (NULL == pJoiner)
// 				{
// 					eSrcErr = T_NEW_TEAM_END;
// 					eErrcode = T_NEW_TEAM_END;
// 					break;
// 				}
// 				TeamerInfo* pSelfTeamInfo = m_pRoleProxy->GetTeamerInfo();
// 				if (NULL == pSelfTeamInfo)
// 				{
// 					eErrcode = T_NEW_TEAM_END;
// 					break;
// 				}
// 
// 				RsTeam * pSelfGroup = TeamUtils::SGetTeam(pSelfTeamInfo);
// 				if (NULL == pSelfGroup || !TeamUtils::IsLeader(pSelfTeamInfo))
// 				{
// 					eSrcErr = T_NEW_TEAM_END;//没有队伍的情况下竟然处于队伍状态
// 					eErrcode = T_NEW_TEAM_END;
// 					break;
// 				}
// 
// 				if (pSelfGroup->IsEnemyWithMember(pJoiner->GetTeamerInfo()))
// 				{
// 					eSrcErr = TeamEC_TheTeamExistEnemyCamp;
// 					break;
// 				}
// 
// 				if (pSelfGroup->CheckConfigFlag(EConfigFlag_ForbiddenJoin))
// 				{
// 					eSrcErr = TeamEC_AuthorityJoinForbidden;
// 					break;
// 				}
// 
// 				const RsTeamerContainer* pContainer = pSelfGroup->GetMembers();
// 
// 				if (pContainer->FreeCount() == 0)
// 				{
// 					eSrcErr = TeamEC_TargetMemberLimit;
// 					break;
// 				}
// 
// 				eSrcErr = pSelfGroup->mRecordMgr.CanJoin(pJoiner->GetRoleID());
// 				if (eSrcErr != TeamEC_OK)
// 				{
// 					break;
// 				}
// 
// 				//添加记录
// 				if (NULL == pSelfGroup->mRecordMgr.AddJoinRecord(pJoiner->GetRoleID(), m_pRoleProxy->GetRoleID()))
// 				{
// 					eErrcode = T_NEW_TEAM_END;//临时用调试
// 					break;
// 				}
// 			} while (false);
// 
// 
// 			if (pMsgReceiver && eSrcErr == TeamEC_OK && eErrcode == TeamEC_OK)
// 			{
// 				FxRSPacketBuilder opb;
// 				pMsgReceiver->SendGS(opb.T_TeamJoinNtf(pJoiner, m_pRoleProxy->GetTeamerInfo()), opb.GetPacketBuf());
// 			}
// 		}
// 		return eSrcErr == TeamEC_OK && eErrcode == TeamEC_OK;
// 	}
// 
// 	bool GroupState::JoinRES(UINT32 dwRoleID, ETeamVerify eAgree, UINT32 dwTeamID, EOrganizationType eTeamType)
// 	{
// 		FxRSPacketBuilder opb;
// 		EErrorCode eErrcode = TeamEC_OK;
// 		EErrorCode eErrcode4Joiner = TeamEC_OK;
// 		FxRole_RS* pJoiner = (INVALID_ROLE_ID == dwRoleID) ? NULL : FxObjFactory::Instance()->FindRole(dwRoleID);
// 		do
// 		{
// 			TeamerInfo* pJoinerTeamInfo = (NULL == pJoiner) ? NULL : pJoiner->GetTeamerInfo();
// 			if (INVALID_TEAM_ID == dwTeamID)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			RsTeam* pTeamWhenJoin = FxObjFactory::Instance()->T_FindTeam(dwTeamID);
// 			RsTeam* pCurrentTeam = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
// 			if (NULL == pCurrentTeam)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			if (pCurrentTeam->GetLeader() != m_pRoleProxy->GetRole())
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_NTLeader, pCurrentTeam->GetOrgType());
// 				//申请方在队长变更时提示
// 				break;
// 			}//优先检查是不是队长, 防止变更队长后, 原申请记录已删除, 申请者重新申请应该在新队长处理时删除
// 
// 			pCurrentTeam->mRecordMgr.DelJoinRecord(dwRoleID, m_pRoleProxy->GetRoleID());
// 
// 			if (pCurrentTeam->IsMember(pJoinerTeamInfo))
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_TargetHasInTeam, pCurrentTeam->GetOrgType());
// 				break;
// 			}
// 
// 			if (pCurrentTeam != pTeamWhenJoin/* || pCurrentTeam->GetOrgType() != eTeamType*/)
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_UInOtherTeam, pCurrentTeam->GetOrgType());
// 				eErrcode4Joiner = TeamUtils::SConvertErrcode(TeamEC_TargetInOtherTeam, pCurrentTeam->GetOrgType());
// 				pJoinerTeamInfo->SendGS(opb.T_TeamJoinAck(eErrcode4Joiner, m_pRoleProxy->GetRole()), opb.GetPacketBuf());
// 				break;
// 			}
// 
// 			if (NULL == pJoinerTeamInfo)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			if (eAgree == ETeamVerifyRefuse)
// 			{
// 				pJoinerTeamInfo->SendGS(opb.T_TeamJoinAck(TeamEC_JoinRefuse, m_pRoleProxy->GetRole()), opb.GetPacketBuf());
// 				break;
// 			}
// 
// 			TeamStateBase* pJoinerState = pJoinerTeamInfo->GetState();
// 			if (NULL == pJoinerState)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			const RsTeamerContainer* pContainer = pCurrentTeam->GetMembers();
// 
// 			
// 			if (pContainer->FreeCount() == 0)
// 			{
// 				eErrcode4Joiner = TeamEC_TargetMemberLimit;
// 				eErrcode = TeamEC_MemberLimit4JoinTarget;
// 				pJoinerTeamInfo->SendGS(opb.T_TeamJoinAck(eErrcode4Joiner, m_pRoleProxy->GetRole()), opb.GetPacketBuf());
// 				break;
// 			}
// 
// 			if (!pJoinerState->HandlerJoinRES(pJoinerTeamInfo, m_pRoleProxy->GetTeamerInfo(), dwTeamID, eTeamType, eAgree, eErrcode4Joiner))
// 			{
// 				break;
// 			}
// 
// 			if (pCurrentTeam->Add(pJoinerTeamInfo) == NULL)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			pJoinerTeamInfo->SendGS(opb.T_TeamJoinAck(TeamEC_JoinAccept, m_pRoleProxy->GetRole()), opb.GetPacketBuf());
// 
// 			//xxx加入了队伍
// 			pContainer->BroadCast(opb.T_TeamMSG_B_D(TeamEC_NewMemberOfTeam, pJoinerTeamInfo->GetRoleName(), pJoinerTeamInfo->GetRoleID()),
// 				opb.GetPacketBuf());
// 			pCurrentTeam->UpdateTeam(pJoinerTeamInfo);
// 			pJoinerTeamInfo->SendGS(opb.T_TeamMSG_B_D(TeamUtils::SConvertErrcode(TeamEC_HasJoinTeam, eTeamType), m_pRoleProxy->GetRoleName()),
// 				opb.GetPacketBuf());
// 			break;//END OF INVITED WITH ORGANIZATION
// 			//////////////////////////////////////////////////////////////////////////
// 		} while (false);
// 
// 		const char* lpstrName = NULL;
// 		if (pJoiner)
// 		{
// 			lpstrName = pJoiner->m_szRoleName;
// 		}
// 		else
// 		{
// 			lpstrName = "";
// 		}
// 
// 
// 		if (eAgree == ETeamVerifyRefuse)
// 		{
// 			m_pRoleProxy->SendGS(opb.T_TeamJoinResAck(TeamEC_OK, eAgree, dwRoleID, lpstrName), opb.GetPacketBuf());
// 		}
// 		else
// 		{
// 			m_pRoleProxy->SendGS(opb.T_TeamJoinResAck(eErrcode, eAgree, dwRoleID, lpstrName), opb.GetPacketBuf());
// 		}
// 
// 		return TeamEC_OK == eErrcode;
// 	}
// 
// 	bool GroupState::HandlerJoinRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pTarget, UINT32 dwOrgID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr)
// 	{
// 		eSrcErr = TeamEC_TargetHasInGroup;
// 		return false;
// 	}
// 
// 	bool GroupState::KickOut(UINT32 dwRoleID)
// 	{
// 		FxRSPacketBuilder opb;
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			if (dwRoleID == INVALID_ROLE_ID)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			if (m_pRoleProxy->GetRoleID() == dwRoleID)
// 			{
// 				eErrcode = TeamEC_AuthorityNEnough;
// 				break;
// 			}
// 
// 			RsTeam* pGroup = TeamUtils::SGetTeam(m_pRoleProxy->GetTeamerInfo());
// 			if (NULL == pGroup)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			if (!m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityKick))
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_AuthorityNeedTLeader, pGroup->GetOrgType());
// 				break;
// 			}
// 
// 			if (!pGroup->IsMember(dwRoleID))
// 			{
// 				eErrcode = TeamEC_TargetNTeamer;
// 				break;
// 			}
// 
// 			FxRole_RS* pTarget = FxObjFactory::Instance()->FindRole(dwRoleID);
// 			if (NULL == pTarget)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			TeamerInfo* pTargetTeamInfo = pTarget->GetTeamerInfo();
// 			if (NULL != pTargetTeamInfo)
// 			{
// 				TeamStateBase* pTargetState = pTargetTeamInfo->GetState();
// 				if (NULL == pTargetState)
// 				{
// 					eErrcode = T_NEW_TEAM_END;
// 					break;
// 				}
// 
// 				pTargetState->HandlerKickOut(m_pRoleProxy->GetTeamerInfo(), eErrcode);
// 
// 				pGroup->LeaveTeam(pTargetTeamInfo);
// 				pTargetTeamInfo->SendGS(opb.T_TeamMSG_B_D(TeamEC_UBeenKickedFromGroup), opb.GetPacketBuf());
// 				pGroup->BroadCast(opb.T_TeamMSG_B_D(TeamEC_GroupMemberBeenKicked, pTargetTeamInfo->GetRoleName()), opb.GetPacketBuf());
// 			}
// 		} while (false);
// 
// 		if (TeamEC_OK != eErrcode)
// 		{
// 			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
// 
// 	bool GroupState::HandlerKickOut(TeamerInfo* pExecutor, EErrorCode& eSrcErr)
// 	{
// 		return true;
// 	}
//
// 	bool GroupState::LeaveREQ()
// 	{
// 		FxRSPacketBuilder opb;
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			TeamerInfo* pSelfInfo = TeamStateBase::m_pRoleProxy->GetTeamerInfo();
// 			if (NULL == pSelfInfo)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			RsTeam* pGroup = TeamUtils::SGetTeam(pSelfInfo);
// 			if (NULL == pGroup)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 			const RsTeamerContainer* pContainer = pGroup->GetMembers();
// 			size_t memberCount = pContainer->Count();
// 
// 			if (memberCount == 1)
// 			{//解散
// 				pGroup->Dismiss();
// 				pGroup = NULL;
// 			}
// 			else
// 			{
// 				if (!pGroup->LeaveTeam(pSelfInfo))
// 				{
// 					eErrcode = T_NEW_TEAM_END;//临时用调试
// 					break;
// 				}
// 				pGroup->GetMembers()->BroadCast(opb.T_TeamMSG_B_D(TeamEC_MemberLeaveTeam, pSelfInfo->GetRoleName()), opb.GetPacketBuf());
// 				pSelfInfo->SendGS(opb.T_TeamMSG_B_D(TeamUtils::SConvertErrcode(TeamEC_ULeaveTeam, pGroup->GetOrgType())),
// 					opb.GetPacketBuf());
// 			}
// 
// 			
// 		} while (false);
// 
// 		if (eErrcode != TeamEC_OK)
// 		{
// 			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}		
// 		return eErrcode == TeamEC_OK;
// 	}

	bool GroupState::TurnToGroupREQ()
	{
		FxRSPacketBuilder opb;
		TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(TeamEC_InGroup), opb.GetPacketBuf());
		return false;
	}

	GroupState::GroupState(IFRoleProxy* pProxy)
		: TeamState(pProxy)
	{

	}

	bool GroupState::OnEnter(ERoleTeamState ePreState)
	{
		TeamerInfo* pInfo = TeamStateBase::m_pRoleProxy->GetTeamerInfo();
		if (NULL == pInfo)
		{
			return false;
		}

		switch (ePreState)
		{
		case ERoleTeamStateSingle://重置
		{
			TeamStateBase::m_pRoleProxy->ClearSubscribe();
			ESubTeamID eSubTeamIdx = pInfo->GetSubTeamID();
			TeamStateBase::m_pRoleProxy->SetSubscribe(ESubscribeIdx(ESubscribeFlag_Buff + eSubTeamIdx), true);
			TeamStateBase::m_pRoleProxy->SetSubscribe(ESubscribeIdx(ESubscribeFlag_SubTeamChat + eSubTeamIdx), true);
			for (UINT8 bySubTeamIdx = 0; bySubTeamIdx < ESubTeamCount; ++bySubTeamIdx)
			{
				TeamStateBase::m_pRoleProxy->SetSubscribe(ESubscribeIdx(ESubscribeFlag_Common + bySubTeamIdx), true);
			}
		}break;
		case ERoleTeamStateInTeam://复制
		{
			for (UINT8 bySubTeamIdx = 0; bySubTeamIdx < ESubTeamCount; ++bySubTeamIdx)
			{
				TeamStateBase::m_pRoleProxy->SetSubscribe(ESubscribeIdx(ESubscribeFlag_Common + bySubTeamIdx), true);
			}
		}break;
		default://不处理
			break;
		}
		

		TeamAuthorityHolder::reset();
		TeamAuthorityHolder::AddAuthority(ETeamAuthorityLeave);
		return true;
	}

// 	bool GroupState::RecommendREQ(UINT32 dwTargetRoleId)
// 	{
// 		FxRSPacketBuilder opb;
// 		EErrorCode eErrcode = TeamEC_OK;
// 		FxRole_RS* pRecommended = (INVALID_ROLE_ID == dwTargetRoleId) ? NULL : FxObjFactory::Instance()->FindRole(dwTargetRoleId);
// 		const char* pRecommendedName = NULL;
// 		do
// 		{
// 			if (NULL == pRecommended)
// 			{
// 				eErrcode = TeamEC_TargetNFound;
// 				break;
// 			}
// 
// 			pRecommendedName = pRecommended->m_szRoleName;
// 
// 			TeamerInfo* pSelfInfo = TeamStateBase::m_pRoleProxy->GetTeamerInfo();
// 			if (NULL == pSelfInfo)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			TeamerInfo* pRecommendedInfo = pRecommended->GetTeamerInfo();
// 			if (NULL == pRecommendedInfo)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			RsTeam* pTeam = TeamUtils::SGetTeam(pSelfInfo);
// 			if (NULL == pTeam)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			if (pTeam->IsMember(dwTargetRoleId))
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_TargetHasInTeam, pTeam->GetOrgType());//临时用调试
// 				break;
// 			}
// 
// 			if (pTeam->IsEnemyWithMember(pRecommendedInfo))
// 			{
// 				eErrcode = TeamEC_EnemyCampWithMember_Recommend;
// 				break;
// 			}
// 
// 			if (TeamUtils::IsLeader(pSelfInfo))
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			if (pTeam->CheckConfigFlag(EConfigFlag_ForbiddenRecommend))
// 			{
// 				eErrcode = TeamEC_AuthorityInviteForbidden;
// 				break;
// 			}
// 
// 			if (pTeam->GetMembers()->FreeCount() == 0)
// 			{
// 				eErrcode = TeamEC_MemberLimit4Recommend;
// 				break;
// 			}
// 
// 			eErrcode = pTeam->mRecordMgr.CanRecommend(dwTargetRoleId);
// 			if (eErrcode != TeamEC_OK)
// 			{
// 				break;
// 			}
// 
// 
// 			FxRole_RS* pLeader = pTeam->GetLeader();
// 			if (NULL == pLeader)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			if (pLeader->GetRoleState() == RoleState_Logout)
// 			{
// 				eErrcode = TeamEC_TargetOffLine;//也不该出现
// 				break;
// 			}
// 
// 			if (NULL == pLeader->GetTeamerInfo())
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			//添加记录
// 			if (NULL == pTeam->mRecordMgr.AddRecommendRecord(dwTargetRoleId, TeamStateBase::m_pRoleProxy->GetRoleID()))
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 			pLeader->GetTeamerInfo()->SendGS(opb.T_TeamRecommendNtf(pRecommendedInfo, pSelfInfo), opb.GetPacketBuf());
// 		} while (false);
// 
// 
// 		if (eErrcode == TeamEC_OK)
// 		{
// 			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(TeamEC_RecommendSended, pRecommendedName), opb.GetPacketBuf());
// 		}
// 		else
// 		{
// 			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode, pRecommendedName), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
// 
// 	bool GroupState::ModifyConfigs(EConfigFlagType eFlagIdx, bool bActive)
// 	{
// 		FxRSPacketBuilder opb;
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			RsTeam* pTeam = TeamUtils::SGetTeam(TeamStateBase::m_pRoleProxy->GetTeamerInfo());
// 			if (NULL == pTeam)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			if (!TeamStateBase::m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityMdConfig))
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_AuthorityNeedTLeader, pTeam->GetOrgType());
// 				break;
// 			}
// 
// 			pTeam->ModifyConfigFlag(eFlagIdx, bActive);
// 			pTeam->GetMembers()->BroadCast(opb.T_TeamInfoNtf_B(pTeam, InfoChange_Config), opb.GetPacketBuf());
// 						
// 		} while (false);
// 
// 		if (TeamEC_OK != eErrcode)
// 		{
// 			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
//
// 	bool GroupState::ModifySubscribe(ESubTeamID eSubIdx, bool bActive)
// 	{
// 		FxRSPacketBuilder opb;
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			TeamerInfo* pSelfInfo = m_pRoleProxy->GetTeamerInfo();
// 			if (NULL == pSelfInfo)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			if (pSelfInfo->GetSubTeamID() == eSubIdx)
// 			{
// 				break;
// 			}
// 
// 			RsTeam* pTeam = TeamUtils::SGetTeam(pSelfInfo);
// 			if (NULL == pTeam)
// 			{
// 				eErrcode = T_NEW_TEAM_END;//临时用调试
// 				break;
// 			}
// 
// 			pSelfInfo->SetSubscribe(ESubscribeIdx(ESubscribeFlag_SubTeamChat + eSubIdx), bActive);
// 			pSelfInfo->SetSubscribe(ESubscribeIdx(ESubscribeFlag_Buff + eSubIdx), bActive);
// 			pTeam->BroadCast(opb.T_TeamMemberNtf_B(pTeam), opb.GetPacketBuf());
// 			FxGSMgr::Instance()->BroadCastToGS(
// 				opb.T_TeamOPEvent_B(pTeam->GetObjId(), EOrganizationTeam, EOrgOperateExchange4Subscribe, m_pRoleProxy->GetRoleID()),
// 				opb.GetPacketBuf(), pTeam->GetObjId());
// 		} while (false);
// 
// 
// 		if (TeamEC_OK != eErrcode)
// 		{
// 			m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
//
// 	bool GroupState::AssignItemREQ(UINT32 dwDstRoleID, UINT32 dwItemID)
// 	{
// 		FxRSPacketBuilder opb;
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			if (INVALID_ROLE_ID == dwDstRoleID || INVALID_ITEM_ID == dwItemID)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 			RsTeam* pTeam = TeamUtils::SGetTeam(TeamStateBase::m_pRoleProxy->GetTeamerInfo());
// 			if (NULL == pTeam)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			if (!TeamStateBase::m_pRoleProxy->GetTeamerInfo()->CheckAuthority(ETeamAuthorityAssign))
// 			{
// 				eErrcode = TeamUtils::SConvertErrcode(TeamEC_AuthorityNeedTLeader, pTeam->GetOrgType());
// 				break;
// 			}
// 
// 			if (!pTeam->IsMember(dwDstRoleID))
// 			{
// 				eErrcode = TeamEC_TargetNTeamer;
// 				break;
// 			}
// 
// 			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamLeaderAssignACK_D(dwDstRoleID, dwItemID), opb.GetPacketBuf());
// 		} while (false);
// 
// 		if (TeamEC_OK != eErrcode)
// 		{
// 			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}
// 
// 	bool GroupState::RecommendHandled(UINT32 dwTargetRoleID)
// 	{
// 		EErrorCode eErrcode = TeamEC_OK;
// 		do
// 		{
// 			if (NULL == TeamStateBase::m_pRoleProxy || INVALID_ROLE_ID == dwTargetRoleID)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			RsTeam* pTeam = TeamUtils::SGetTeam(TeamStateBase::m_pRoleProxy->GetTeamerInfo());
// 			if (NULL == pTeam)
// 			{
// 				eErrcode = T_NEW_TEAM_END;
// 				break;
// 			}
// 
// 			if (pTeam->GetLeader() != TeamStateBase::m_pRoleProxy->GetRole())
// 			{
// 				break;
// 			}
// 
// 			pTeam->mRecordMgr.DelRecommendRecord(dwTargetRoleID);
// 		} while (false);
// 
// 		if (TeamEC_OK != eErrcode)
// 		{
// 			FxRSPacketBuilder opb;
// 			TeamStateBase::m_pRoleProxy->SendGS(opb.T_TeamMSG_B_D(eErrcode), opb.GetPacketBuf());
// 		}
// 
// 		return eErrcode == TeamEC_OK;
// 	}

	EErrorCode GroupState::convertErrRef(EErrorCode& eErr) const
	{
		return eErr = TeamUtils::SConvertErrcode(eErr, EOrganizationGroup);
	}

	bool GroupState::checkArrangeSubTeamID(ESubTeamID eSubIdx) const
	{
		return eSubIdx >= 0 && eSubIdx < ESubTeamCount;
	}

}
