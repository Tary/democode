#pragma once
#include "IFTeamEventHandler.h"
#include "IFRoleProxy.h"
#include "TeamAuthorityHolder.h"
namespace Team
{
	class RsTeam;
	class TeamStateBase : public TeamAuthorityHolder, public IFTeamEventHandler
	{
	protected:
		IFRoleProxy* m_pRoleProxy;
	public:
		TeamStateBase(IFRoleProxy* pProxy);

		virtual bool InviteREQByName(const std::string& strName);

		virtual bool RecommendREQByName(const std::string& strTargetName);

		virtual bool OnEnter(ERoleTeamState ePreState) = 0;
	protected:
		RsTeam* CheckOrganization(UINT32 dwOrgID, EOrganizationType eOrgType, EErrorCode& eOutErrcode, TeamStateBase*& pOutLeaderState) const;


	};
}
