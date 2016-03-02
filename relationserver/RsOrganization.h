#pragma once
#include "base/object.h"
#include "ETeamDef.h"
#include "RequestRecord.h"
#include "RequestRecordMgr.h"


namespace Team
{
	class RsOrganization : public FxObj
	{
	public:
		RsOrganization();
		~RsOrganization();

		//销毁自身, 调用后应置空
		virtual void Dismiss() = 0;

		EOrganizationType GetTeamType() const;

		bool CheckLogicFlag(ELogicFlag eType) const;
		void SetLogicFlag(ELogicFlag eType, bool bActive);

		bool CheckConfigFlag(EConfigFlagType eType) const;
		void SetConfigFlag(EConfigFlagType eType, bool bActive);
		UINT32 ConfigFlagToUL() const;
		bool ModifyConfigFlag(EConfigFlagType eFlagType, bool bActive);

		FxRole_RS* setLeader(FxRole_RS* val);
		FxRole_RS* GetLeader() const;
		FxRole_RS* GetAgentLeader() const;

		void UpdateLeaderAuthority();

		virtual bool AssignNewLeader(const TeamerInfo* pPreferInfo = NULL);

		virtual bool IsMember(UINT32 dwRoleID) const = 0;

		bool setAssignMethod(ETeamAssignMethod eVal);
		ETeamAssignMethod GetAssignMethod() const;

		bool setAssignLevel(EItemQuality eVal);
		EItemQuality GetAssignLevel() const;

		void setDesc(const std::string& strVal);
		const std::string& GetDesc() const;

		RequestRecordMgr mRecordMgr;

	protected:
		virtual FxRole_RS* voteLeader(ERoleState eRequestState, const TeamerInfo* pPreferInfo = NULL) const = 0;
		EOrganizationType setTeamType(EOrganizationType eType);		
	private:
		FxRole_RS* m_pLeader;
		ETeamAssignMethod m_eAssignMethod;
		EItemQuality m_eAssignLevel;
		std::string m_strDesc;
		std::bitset<EConfigFlagCount> m_ConfigFlags;
		std::bitset<ELogicFlagCount> m_LogicFlags;
		EOrganizationType m_eType;
	};
	
}
