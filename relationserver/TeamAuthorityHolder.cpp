#include "TeamAuthorityHolder.h"
namespace Team
{
	bool TeamAuthorityHolder::AddAuthority(ETeamAuthority eAuthority)
	{
		if (CheckAuthority(eAuthority))
		{
			return false;
		}

		m_AuthorityFlag.set(eAuthority, true);
		return true;
	}

	bool TeamAuthorityHolder::RemoveAuthority(ETeamAuthority eAuthority)
	{
		if (CheckAuthority(eAuthority))
		{
			m_AuthorityFlag.set(eAuthority, false);
			return true;
		}
		
		return false;
	}

	bool TeamAuthorityHolder::CheckAuthority(ETeamAuthority eAuthority) const
	{
		return m_AuthorityFlag.test(eAuthority);
	}

	UINT32 TeamAuthorityHolder::GetAllAuthority() const
	{
		return (UINT32)m_AuthorityFlag.to_ulong();
	}

	void TeamAuthorityHolder::reset()
	{
		m_AuthorityFlag.reset();
	}

}
