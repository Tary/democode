#pragma once
#include "ETeamDef.h"
#include "TSimpleContainer.h"
namespace Team
{

	class RsTeam;

	//过期原因
	enum EDropReason
	{
		EDropReason_None,
		EDropReason_TurnGroup,
		EDropReason_TimeOut,
	};

	class RequestRecord
	{

	private:
		time_t m_CreateTime;
	public:
		UINT32 m_TargetRoleID;
		UINT32 m_Operator;
		EDropReason m_eDropReason;
		EOrganizationType m_eCurTeamType;
		UINT32 m_TeamID;
	public:
		RequestRecord();
		~RequestRecord();

		bool IsExpire() const;

		long GetTimeToExpirec() const;

		typedef std::vector<RequestRecord*> RecordVEC;
		static void SGetAllInviteRecords(UINT32 dwTargetRoleID, RecordVEC& routResult);
		static void SRemoveAutoRefuseRecord(const RequestRecord* pVal);
		static bool SAddAutoRefuseRecord(RequestRecord* pVal, RsTeam* pTeam);

		typedef std::multimap<UINT32, RequestRecord*> TargetRecordMULMAP;
		static TargetRecordMULMAP s_AutoRefuseInviteRecords;
	};

	typedef std::vector<const RequestRecord*> RequestRecordVEC;

	class RequestRecordBag : public TSimpleContainer < RequestRecord >
	{
	public:
		RequestRecordBag(size_t nMax);
		~RequestRecordBag();
		bool IsExist(UINT32 dwTarget) const;

		virtual RequestRecord* GetRecord(UINT32 dwTarget) const;
		virtual RequestRecord* AddRecord(UINT32 dwTarget, UINT32 dwOperator);
		virtual size_t DelRecord(UINT32 dwTarget, UINT32 dwOperator = INVALID_ROLE_ID);
		virtual void DelRecord(RequestRecord* pVal);

		static bool SCheckByID(const RequestRecord* pVal, UINT32& dwTarget, RequestRecord*& outpRs);

		static bool DestroyRecord(const RequestRecord* pVal, UINT32 dwTarget);
		static bool DestroyRecord3(const RequestRecord* pVal, UINT32 dwTarget, UINT32 dwOperatorFilter, RequestRecordVEC& rDels);

	protected:
		virtual void onExpireDrop(RequestRecord* pRecord);
	};	
}


