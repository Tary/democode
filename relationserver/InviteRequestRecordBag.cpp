#include "InviteRequestRecordBag.h"
#include "TeamUtils.h"
namespace Team
{
	void InviteRequestRecordBag::onExpireDrop(RequestRecord* pRecord)
	{
		if (pRecord && pRecord->IsExpire())
		{
			pRecord->m_eDropReason = EDropReason_TimeOut;
			m_DropedRecords.Add(pRecord);
			TSimpleContainer<RequestRecord>::UnSafeRemove(pRecord);
		}
	}

	void InviteRequestRecordBag::DropAllRecord(EDropReason eReason)
	{
		RequestRecordVEC outResults;
		foreach1r(TeamUtils::CollectRecord, outResults);

		for (RequestRecordVEC::iterator iter = outResults.begin(); iter != outResults.end(); ++iter)
		{
			RequestRecord* pRecord = const_cast<RequestRecord*>(*iter);
			pRecord->m_eDropReason = eReason;
			m_DropedRecords.Add(pRecord);
		}

		UnSafeClear();
	}

	InviteRequestRecordBag::InviteRequestRecordBag()
		: RequestRecordBag(MAX_INVITE_RECORD)
		, m_DropedRecords(MAXINT32)
	{

	}

	RequestRecord* InviteRequestRecordBag::GetRecord(UINT32 dwTarget, bool bIncludeDroped) const
	{
		RequestRecord* pRecord = RequestRecordBag::GetRecord(dwTarget);
		if (NULL == pRecord && bIncludeDroped)
		{
			pRecord = m_DropedRecords.GetRecord(dwTarget);
		}
		return pRecord;
	}

	size_t InviteRequestRecordBag::DelRecord(UINT32 dwTarget, UINT32 dwOperator /*= INVALID_ROLE_ID*/)
	{//只能在其中的一个
		m_DropedRecords.DelRecord(dwTarget, dwOperator);
		return RequestRecordBag::DelRecord(dwTarget, dwOperator);
	}

	void InviteRequestRecordBag::ClearDropedRecord(UINT32 dwTargetRoleID)
	{
		m_DropedRecords.DelRecord(dwTargetRoleID, INVALID_ROLE_ID);
	}

}
