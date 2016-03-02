#pragma once

#include <bitset>
#include "ETeamDef.h"


namespace Team
{
	class TeamAuthorityHolder
	{
	
	private:
		std::bitset<ETeamAuthorityCount> m_AuthorityFlag;
	protected:
		void reset();
	public:
		bool AddAuthority(ETeamAuthority eAuthority);
	
		bool RemoveAuthority(ETeamAuthority eAuthority);
	
		bool CheckAuthority(ETeamAuthority eAuthority) const;
	
		UINT32 GetAllAuthority() const;
	};
}
