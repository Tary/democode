#pragma once
#include "ETeamDef.h"
#include "RsTeamerContainer.h"
namespace Team
{
	class RsSubTeam : public RsTeamerContainer
	{
	public:
		RsSubTeam(ESubTeamID id);

		ESubTeamID GetID() const;

		virtual TeamerInfo* onAdd(TeamerInfo* pVal);

		virtual TeamerInfo* onRemove(TeamerInfo* pVal);

	private:
		ESubTeamID m_eID;
	};
}
