#pragma once
#include "TeamStateBase.h"
namespace Team
{
	class TeamState : public TeamStateBase
	{
	public:
		TeamState(IFRoleProxy* pProxy);

		virtual bool Arrange(UINT32 dwRoleID, ESubTeamID eSubIdx, UINT8 posInSub);

		virtual bool HandlerArrange(TeamerInfo* pExecutor, EErrorCode& eSrcErr);

		virtual bool AttornREQ(UINT32 dwCandidateRoleID);

		virtual bool HandlerAttornREQ(TeamerInfo* pExecutor, EErrorCode& eSrcErr);

		virtual bool ChangeAssignSchemaREQ(ETeamAssignMethod type, EItemQuality level);

		virtual bool ChangeDescREQ(const char* pDesc);

		virtual bool CreateTeamREQ();

		virtual bool InviteREQ(UINT32 dwRoleID);

		virtual bool HandlerInviteREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pInviter, UINT32 dwOrgID, EOrganizationType eOrgType, EErrorCode& eSrcErr);

		virtual bool InviteRES(UINT32 dwInviterRoleId, ETeamVerify eAgree, UINT32 dwTeamID, EOrganizationType eOrgType);

		virtual bool HandlerInviteRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pInvited, UINT32 dwTeamID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr);

		virtual bool JoinREQ(UINT32 dwRoleID);

		virtual bool HandlerJoinREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pJoiner, EErrorCode& eSrcErr);

		virtual bool JoinRES(UINT32 dwRoleID, ETeamVerify eAgree, UINT32 dwTeamID, EOrganizationType eTeamType);

		virtual bool HandlerJoinRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pTarget, UINT32 dwOrgID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr);

		virtual bool KickOut(UINT32 dwRoleID);

		virtual bool HandlerKickOut(TeamerInfo* pExecutor, EErrorCode& eSrcErr);

		virtual bool LeaveREQ();

		virtual bool TurnToGroupREQ();

		virtual bool OnEnter(ERoleTeamState ePreState);

		virtual bool RecommendREQ(UINT32 dwTargetRoleId);

		virtual bool ModifyConfigs(EConfigFlagType eFlagType, bool bActive);

		virtual bool ModifySubscribe(ESubTeamID eSubIdx, bool bActive);

		virtual bool AssignItemREQ(UINT32 dwDstRoleID, UINT32 dwItemID);

		virtual bool RecommendRES(UINT32 dwTargetRoleID, const char* pName, ETeamVerify eAgree);


	protected:
		EErrorCode convertErr(EErrorCode eErr) const;
		virtual EErrorCode convertErrRef(EErrorCode& eErr) const;

		virtual bool checkArrangeSubTeamID(ESubTeamID eSubIdx) const;

		virtual bool HandlerRecommendREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pInviter, EErrorCode& eSrcErr);

	};
}
