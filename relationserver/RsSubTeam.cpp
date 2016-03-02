#include "RsSubTeam.h"
namespace Team
{
	RsSubTeam::RsSubTeam(ESubTeamID eId) : m_eID(eId)
	{
		setMaxLimit(MAX_TEAM_MEMBER_COUNT);
	}

	ESubTeamID RsSubTeam::GetID() const
	{
		return m_eID;
	}

	TeamerInfo* RsSubTeam::onAdd(TeamerInfo* pVal)
	{
		return pVal;
	}

	TeamerInfo* RsSubTeam::onRemove(TeamerInfo* pVal)
	{
		return pVal;
	}

}
