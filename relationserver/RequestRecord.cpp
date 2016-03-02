#include "RequestRecord.h"
#include "TeamUtils.h"

namespace Team
{
	RequestRecord::RequestRecord()
		: m_CreateTime(time(NULL))
		, m_TargetRoleID(INVALID_ROLE_ID)
		, m_Operator(INVALID_ROLE_ID)
		, m_eDropReason(EDropReason_None)
		, m_eCurTeamType(EOrganizationInvalid)
		, m_TeamID(INVALID_TEAM_ID)
	{
	}
	
	
	RequestRecord::~RequestRecord()
	{
		SRemoveAutoRefuseRecord(this);
	}

	bool RequestRecord::IsExpire() const
	{
		return time(NULL) - m_CreateTime > TEAM_REQUEST_EXPIRE_SECOND;
	}

	long RequestRecord::GetTimeToExpirec() const
	{
		long lThroughTime = (long)(time(NULL) - m_CreateTime);
		return max(0, TEAM_REQUEST_EXPIRE_SECOND - lThroughTime);
	}

	void RequestRecord::SRemoveAutoRefuseRecord(const RequestRecord* pVal)
	{
		if (NULL == pVal || INVALID_ROLE_ID == pVal->m_TargetRoleID)
		{
			return;
		}

		TargetRecordMULMAP::iterator beginIter = s_AutoRefuseInviteRecords.lower_bound(pVal->m_TargetRoleID);
		TargetRecordMULMAP::iterator endIter = s_AutoRefuseInviteRecords.upper_bound(pVal->m_TargetRoleID);

		while (beginIter != endIter)
		{
			if (beginIter->second == pVal)
			{
				s_AutoRefuseInviteRecords.erase(beginIter);
				break;
			}
			++beginIter;
		}
	}

	void RequestRecord::SGetAllInviteRecords(UINT32 dwTargetRoleID, RecordVEC& routResult)
	{
		if (INVALID_ROLE_ID == dwTargetRoleID)
		{
			return;
		}

		TargetRecordMULMAP::iterator beginIter = s_AutoRefuseInviteRecords.lower_bound(dwTargetRoleID);
		TargetRecordMULMAP::iterator endIter = s_AutoRefuseInviteRecords.upper_bound(dwTargetRoleID);

		while (beginIter != endIter)
		{
			RequestRecord* pVal = beginIter->second;
			if (pVal)
			{
				routResult.push_back(pVal);
			}
			++beginIter;
		}
	}

	bool RequestRecord::SAddAutoRefuseRecord(RequestRecord* pRecord, RsTeam* pTeam)
	{
		if (NULL == pRecord || INVALID_ROLE_ID == pRecord->m_TargetRoleID)
		{
			return false;
		}

		if (NULL == pTeam)
		{
			pRecord->m_eCurTeamType = EOrganizationInvalid;
			pRecord->m_TeamID = INVALID_TEAM_ID;
		}
		else
		{
			pRecord->m_eCurTeamType = pTeam->GetTeamType();
			pRecord->m_TeamID = pTeam->GetObjId();
		}
		
		s_AutoRefuseInviteRecords.insert(TargetRecordMULMAP::value_type(pRecord->m_TargetRoleID, pRecord));

		return true;
	}

	RequestRecord::TargetRecordMULMAP RequestRecord::s_AutoRefuseInviteRecords;

	RequestRecordBag::RequestRecordBag(size_t nMax)
	{
		setMaxLimit(nMax);
	}

	RequestRecord* RequestRecordBag::GetRecord(UINT32 dwTarget) const
	{
		RequestRecord* pExist = NULL;
		foreach2rend(SCheckByID, dwTarget, pExist);

		return pExist;
	}

	bool RequestRecordBag::DestroyRecord3(const RequestRecord* pVal, UINT32 dwTarget, UINT32 dwOperatorFilter, RequestRecordVEC& rDels)
	{
		if (pVal && (INVALID_ROLE_ID == dwTarget || pVal->m_TargetRoleID == dwTarget))
		{
			if (INVALID_ROLE_ID != dwOperatorFilter && pVal->m_Operator != dwOperatorFilter)
			{
				return false;
			}

			rDels.push_back(pVal);
			delete pVal;

			if (INVALID_ROLE_ID != dwTarget)
			{
				return true;
			}
		}
		return false;
	}

	size_t RequestRecordBag::DelRecord(UINT32 dwTarget, UINT32 dwOperator)
	{
		RequestRecordVEC resultDels;
		foreach3rend(DestroyRecord3, dwTarget, dwOperator, resultDels);

		for (RequestRecordVEC::iterator iter = resultDels.begin(); iter != resultDels.end(); ++iter)
		{
			Remove(const_cast<RequestRecord*>(*iter));
		}
		return resultDels.size();
	}

	void RequestRecordBag::DelRecord(RequestRecord* pVal)
	{
		if (pVal && Remove(pVal))
		{
			delete pVal;
		}
	}

	RequestRecordBag::~RequestRecordBag()
	{
		foreach1(DestroyRecord, INVALID_ROLE_ID);
		UnSafeClear();
	}

	bool RequestRecordBag::DestroyRecord(const RequestRecord* pVal, UINT32 dwTarget)
	{
		if (pVal && (INVALID_ROLE_ID == dwTarget || pVal->m_TargetRoleID == dwTarget))
		{
			delete pVal;

			if (INVALID_ROLE_ID != dwTarget)
			{
				return true;
			}
		}

		return false;
	}

	RequestRecord* RequestRecordBag::AddRecord(UINT32 dwTarget, UINT32 dwOperator)
	{
		RequestRecord* pRecord = GetRecord(dwTarget);
		if (pRecord && pRecord->IsExpire())
		{
			onExpireDrop(pRecord);
		}

		pRecord = new RequestRecord();
		if (NULL == Add(pRecord))
		{
			delete pRecord;
			return NULL;
		}

		pRecord->m_Operator = dwOperator;
		pRecord->m_TargetRoleID = dwTarget;
		return pRecord;
	}

	bool RequestRecordBag::SCheckByID(const RequestRecord* pVal, UINT32& dwTarget, RequestRecord*& outpRs)
	{
		if (NULL == pVal)
		{
			outpRs = NULL;
			return false;
		}

		if (pVal->m_TargetRoleID == dwTarget)
		{
			outpRs = const_cast<RequestRecord*>(pVal);
			return true;
		}

		return false;
	}

	bool RequestRecordBag::IsExist(UINT32 dwTarget) const
	{
		return NULL != GetRecord(dwTarget);
	}

	void RequestRecordBag::onExpireDrop(RequestRecord* pRecord)
	{
		DelRecord(pRecord);
	}

}
