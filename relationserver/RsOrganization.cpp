#include "RsOrganization.h"
#include "TeamerInfo.h"

namespace Team
{
	RsOrganization::RsOrganization()
		: m_pLeader(NULL)
		, m_eAssignMethod(TeamAssign_Random)
		, m_eAssignLevel(ItemQuality_Green)
		, m_eType(EOrganizationInvalid)
	{
	}


	RsOrganization::~RsOrganization()
	{
	}

	bool RsOrganization::CheckLogicFlag(ELogicFlag eType) const
	{
		return m_LogicFlags.test(eType);
	}

	void RsOrganization::SetLogicFlag(ELogicFlag eType, bool bActive)
	{
		m_LogicFlags.set(eType, bActive);
	}

	bool RsOrganization::CheckConfigFlag(EConfigFlagType eType) const
	{
		return m_ConfigFlags.test(eType);
	}

	void RsOrganization::SetConfigFlag(EConfigFlagType eType, bool bActive)
	{
		m_ConfigFlags.set(eType, bActive);
	}

	FxRole_RS* RsOrganization::setLeader(FxRole_RS* val)
	{
		if (val && !IsMember(val->GetRoleId()))
		{
			val = NULL;
		}

		if (m_pLeader && m_pLeader->GetTeamerInfo() && IsMember(m_pLeader->GetRoleId()))
		{//进入别的队伍别人负责处理
			TeamStateBase* pLeaderState = m_pLeader->GetTeamerInfo()->GetState();
			if (pLeaderState)
			{//重置
				pLeaderState->OnEnter((ERoleTeamState)GetTeamType());
			}
		}

		m_pLeader = val;

		UpdateLeaderAuthority();
		return m_pLeader;
	}

	FxRole_RS* RsOrganization::GetLeader() const
	{
		return m_pLeader;
	}

	bool RsOrganization::setAssignMethod(ETeamAssignMethod eVal)
	{
		if (m_eAssignMethod == eVal)
		{	return false;
		}
		m_eAssignMethod = eVal;

		return true;
	}

	ETeamAssignMethod RsOrganization::GetAssignMethod() const
	{
		return m_eAssignMethod;
	}

	bool RsOrganization::setAssignLevel(EItemQuality eVal)
	{
		if (m_eAssignLevel == eVal)
		{	return false;
		}
		m_eAssignLevel = eVal;

		return true;
	}

	EItemQuality RsOrganization::GetAssignLevel() const
	{
		return m_eAssignLevel;
	}

	void RsOrganization::setDesc(const std::string& strVal)
	{
		m_strDesc = strVal;
	}

	const std::string& RsOrganization::GetDesc() const
	{
		return m_strDesc;
	}

	bool RsOrganization::AssignNewLeader(const TeamerInfo* pPreferInfo)
	{
		if (NULL == GetLeader() || !IsMember(GetLeader()->GetRoleId())|| GetLeader()->GetRoleState() == RoleState_Logout || CheckLogicFlag(ELogicFlag_Attorn))
		{
			FxRole_RS* pLeader = pLeader = voteLeader(RoleState_InGame, pPreferInfo);
			setLeader(pLeader);
			return NULL != m_pLeader;
		}

		return false;
	}

	Team::EOrganizationType RsOrganization::GetTeamType() const
	{
		return m_eType;
	}

	Team::EOrganizationType RsOrganization::setTeamType(EOrganizationType eType)
	{
		m_eType = eType;
		return eType;
	}

	UINT32 RsOrganization::ConfigFlagToUL() const
	{
		return m_ConfigFlags.to_ulong();
	}

	bool RsOrganization::ModifyConfigFlag(EConfigFlagType eFlagIdx, bool bActive)
	{
		if (eFlagIdx >= EConfigFlagCount || eFlagIdx  < 0 || m_ConfigFlags.test(eFlagIdx) == bActive)
		{
			return false;
		}

		m_ConfigFlags.set(eFlagIdx, bActive);
		return true;
	}

	void RsOrganization::UpdateLeaderAuthority()
	{
		if (m_pLeader && m_pLeader->GetTeamerInfo())
		{
			TeamStateBase* pLeaderState = m_pLeader->GetTeamerInfo()->GetState();
			if (pLeaderState)
			{
				pLeaderState->AddAuthority(ETeamAuthorityArrange);
				pLeaderState->AddAuthority(ETeamAuthorityAssign);
				pLeaderState->AddAuthority(ETeamAuthorityAttorn);
				pLeaderState->AddAuthority(ETeamAuthorityInvite);
				pLeaderState->AddAuthority(ETeamAuthorityKick);
				pLeaderState->AddAuthority(ETeamAuthirutyTurnToGroup);
				pLeaderState->AddAuthority(ETeamAuthorityMdAssignSchema);
				pLeaderState->AddAuthority(ETeamAuthorityMdTeamDesc);
				pLeaderState->AddAuthority(ETeamAuthorityMdConfig);
			}
		}
	}

	FxRole_RS* RsOrganization::GetAgentLeader() const
	{
		if (m_pLeader)
		{
			return m_pLeader;
		}

		return voteLeader(RoleState_Logout);
	}

}