#pragma once
#include "RequestRecord.h"
namespace Team
{
	class InviteRequestRecordBag : public RequestRecordBag
	{
	public:
		InviteRequestRecordBag();

		void DropAllRecord(EDropReason eReason);

		void ClearDropedRecord(UINT32 dwTargetRoleID);

		virtual RequestRecord* GetRecord(UINT32 dwTarget, bool bIncludeDroped = true) const;

		virtual size_t DelRecord(UINT32 dwTarget, UINT32 dwOperator = INVALID_ROLE_ID);

	protected:
		virtual void onExpireDrop(RequestRecord* pRecord);

		

	private:
		RequestRecordBag m_DropedRecords;
	};
}


