#include "TeamStateBase.h"
#include "factory/objfactory.h"
#include "TeamUtils.h"
namespace Team
{
	TeamStateBase::TeamStateBase(IFRoleProxy* pProxy)
		: m_pRoleProxy(pProxy)
	{
		
	}

	bool TeamStateBase::InviteREQByName(const std::string& strName)
	{
		UINT32 dwRoleID = FxObjFactory::Instance()->FindRoleID(strName);
		return InviteREQ(dwRoleID);
	}

	RsTeam* TeamStateBase::CheckOrganization(UINT32 dwOrgID, EOrganizationType eOrgType, EErrorCode& eOutErrcode, TeamStateBase*& pOutLeaderState) const
	{
		eOutErrcode = TeamEC_OK;
		pOutLeaderState = NULL;
		if (INVALID_TEAM_ID == dwOrgID)
		{
			eOutErrcode = T_NEW_TEAM_END;
			return NULL;
		}
		
		RsTeam* pTeam = FxObjFactory::Instance()->T_FindTeam(dwOrgID);
		do 
		{
			if (NULL == pTeam)
			{
				eOutErrcode = TeamUtils::SConvertErrcode(TeamEC_TargetTeamDismiss, eOrgType);
				break;
			}

			if (pTeam->GetTeamType() != eOrgType)
			{//暂时不检查
// 				eOutErrcode = TeamUtils::SConvertErrcode(TeamEC_TargetTeamDismiss, eOrgType);
// 				break;
			}

			if (eOutErrcode == TeamEC_OK && pTeam)
			{
				FxRole_RS* pLeader = pTeam->GetLeader();
				if (NULL == pLeader)
				{
					pLeader = pTeam->GetAgentLeader();
				}

				if (NULL == pLeader || NULL == pLeader->GetTeamerInfo())
				{
					eOutErrcode = T_NEW_TEAM_END;//
					break;
				}

				TeamerInfo* pLeaderTeamInfo = pLeader->GetTeamerInfo();
				if (pLeaderTeamInfo->getStateType() == ERoleTeamStateInTeam && eOrgType == EOrganizationGroup)
				{
					eOutErrcode = T_NEW_TEAM_END;//团队的队长是组队状态
					break;
				}

				if (pLeaderTeamInfo->getStateType() == ERoleTeamStateInGroup && eOrgType == EOrganizationTeam)
				{//eErrcode = TeamEC_TargetTeamDismiss;//小队已经属于团队
				}



				pOutLeaderState = pLeaderTeamInfo->GetState();
				if (NULL == pOutLeaderState)
				{
					eOutErrcode = T_NEW_TEAM_END;
					break;
				}
			}			
		} while (false);

		return pTeam;
	}

	bool TeamStateBase::RecommendREQByName(const std::string& strTargetName)
	{
		UINT32 dwRoleID = FxObjFactory::Instance()->FindRoleID(strTargetName);
		return RecommendREQ(dwRoleID);
	}

}
