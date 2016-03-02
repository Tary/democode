#pragma once
#include "headproto.h"
#include "TeamerInfo.h"
#include "RsTeam.h"

namespace Team
{
	class TeamUtils
	{
	public:
		TeamUtils();
		~TeamUtils();
		static bool SFillSubscribeInfo(const TeamerInfo* pMemberInfo, UINT32 & outInfo);
		static bool SFillRoleInfo(const TeamerInfo* pMemberInfo, RSRoleInfoInTeam & outInfo);
		static bool SFillAuthorityInfo(const TeamerInfo* pTeamer, RoleTeamAuthority& outInfo);
		static bool SFillTeamCfg(const RsTeam* pTeam, T_TeamConfig& teamCfg);
		static bool SFillTeamersInfo(const RsTeam* pTeam, RSTeamersInfo& teamCfg);

		static RsTeam* SGetTeam(const TeamerInfo* pInfo);
		static RsTeam* SGetTeam(const FxRole_RS* pRole);
// 		static RsGroup* SGetGroup(const TeamerInfo* pInfo);
// 		static RsGroup* SGetGroup(const FxRole_RS* pRole);
// 		static RsGroup* SGetGroup(const RsTeam* pTeam);

		static bool IsLeader(const TeamerInfo* pInfo);
		static bool IsLeader(const FxRole_RS* pInfo);

		static bool SIsSameInfo(const TeamerInfo* pInfo, const TeamerInfo* pComInfo);

		static bool SCheckID(const TeamerInfo* pInfo, UINT32 dwRoleID, TeamerInfo*& rpResult);
		static bool SSelectMemberByID(const RsTeam* pTeam, UINT32 dwRoleID, TeamerInfo*& rpResult);
		static bool SCheckTeamerState(const TeamerInfo* pInfo, ERoleState eState, TeamerInfo*& rpResult);
		static bool SSelectTeamerByState(const RsTeam* pTeam, ERoleState& eState, TeamerInfo*& rpResult);

		static bool SCheckEnemyTeamer(const TeamerInfo* pInfo, UINT32 dwTargetCamp);

		static void SUpdateMaxFriendship(const TeamerInfo* pInfo1, const TeamerInfo* pInfo2, UINT32& dwVal);

		static bool CollectTeamerFunc(TeamerInfo* pVal, std::vector<TeamerInfo*>& outResult);

		static EErrorCode SConvertErrcode(EErrorCode srcCode, EOrganizationType eTeamType);

		static bool CollectRecord(const RequestRecord* pRecord, RequestRecordVEC& outResults);
		static bool CollectRecordID(const RequestRecord* pRecord, std::set<UINT32>& outResults);
		static bool CollectRecordOperatorID(const RequestRecord* pRecord, std::set<UINT32>& outResults);
		static bool SSelectEarliestValidState(const TeamerInfo* pInfo, ERoleState eState, TeamerInfo*& rpResult);
	};


	template<size_t _Bits>
	void ULToBitset(UINT32 val, std::bitset<_Bits> & rDst)
	{
		if (rDst.to_ulong() != val)
		{
			rDst.reset();
			size_t idx = 0;
			for (; idx < _Bits; ++idx)
			{
				if ((val & (1 << idx)) != 0)
				{
					rDst.set(idx, true);
				}
			}
		}
	}

}

