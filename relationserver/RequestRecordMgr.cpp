#include "RequestRecordMgr.h"
#include "TeamUtils.h"

namespace Team
{
	EErrorCode RequestRecordMgr::CanInvite(UINT32 dwTargetRoleID) const
	{
		RequestRecord* pRecord = m_InviteRecordsBag.GetRecord(dwTargetRoleID, false);
		if (pRecord && !pRecord->IsExpire())
		{
			return TeamEC_TargetHasBeenInvited;
		}

		if (m_InviteRecordsBag.FreeCount() <= 0)
		{
			return TeamEC_InviteTooMany;
		}

		return TeamEC_OK;
	}

	EErrorCode RequestRecordMgr::CanJoin(UINT32 dwTargetRoleID) const
	{
		RequestRecord* pRecord = m_JoinRecords.GetRecord(dwTargetRoleID);
		if (pRecord && !pRecord->IsExpire())
		{
			return TeamEC_TargetHasSendJoin;
		}

		if (m_JoinRecords.FreeCount() <= 0)
		{
			return TeamEC_JoinTooMany;
		}

		return TeamEC_OK;
	}

	EErrorCode RequestRecordMgr::CanRecommend(UINT32 dwTargetRoleID) const
	{
		RequestRecord* pRecord = m_InviteRecordsBag.GetRecord(dwTargetRoleID, false);
		if (pRecord && !pRecord->IsExpire())
		{
			return TeamEC_TargetHasBeenInvited4Recommend;
		}

		RequestRecord* pRecommendRecord = m_RecommendRecords.GetRecord(dwTargetRoleID);
		if (pRecommendRecord && !pRecommendRecord->IsExpire())
		{
			return TeamEC_TargetHasBeenRecommended;
		}

		if (m_RecommendRecords.FreeCount() <= 0)
		{
			return TeamEC_RecommendTooMany;
		}

		return TeamEC_OK;
	}

	RequestRecord* RequestRecordMgr::AddInviteRecord(UINT32 dwTargetRoleID, UINT32 dwOperator)
	{
		EErrorCode err = CanInvite(dwTargetRoleID);
		if (TeamEC_OK != err)
		{
			return NULL;
		}

		return m_InviteRecordsBag.AddRecord(dwTargetRoleID, dwOperator);
	}

	size_t RequestRecordMgr::DelInviteRecord(UINT32 dwTargetRoleID)
	{
		return m_InviteRecordsBag.DelRecord(dwTargetRoleID, INVALID_ROLE_ID);
	}

	RequestRecord* RequestRecordMgr::GetInviteRecord(UINT32 dwTargetRoleID) const
	{
		return m_InviteRecordsBag.GetRecord(dwTargetRoleID);
	}

	RequestRecord* RequestRecordMgr::AddJoinRecord(UINT32 dwTargetRoleID, UINT32 dwOperator)
	{
		EErrorCode err = CanJoin(dwTargetRoleID);
		if (TeamEC_OK != err)
		{
			return NULL;
		}

		return m_JoinRecords.AddRecord(dwTargetRoleID, dwOperator);
	}

	size_t RequestRecordMgr::DelJoinRecord(UINT32 dwTargetRoleID, UINT32 dwOperatorID)
	{
		return m_JoinRecords.DelRecord(dwTargetRoleID, dwOperatorID);
	}

	RequestRecord* RequestRecordMgr::GetJoinRecord(UINT32 dwTargetRoleID) const
	{
		return m_JoinRecords.GetRecord(dwTargetRoleID);
	}

	RequestRecord* RequestRecordMgr::AddRecommendRecord(UINT32 dwTargetRoleID, UINT32 dwOperator)
	{
		EErrorCode err = CanInvite(dwTargetRoleID);
		if (TeamEC_OK != err)
		{
			return NULL;
		}

		return m_RecommendRecords.AddRecord(dwTargetRoleID, dwOperator);
	}

	size_t RequestRecordMgr::DelRecommendRecord(UINT32 dwTargetRoleID, UINT32 dwOperatorID)
	{
		return m_RecommendRecords.DelRecord(dwTargetRoleID, dwOperatorID);
	}

	RequestRecord* RequestRecordMgr::GetRecommendRecord(UINT32 dwTargetRoleID) const
	{
		return m_RecommendRecords.GetRecord(dwTargetRoleID);
	}

	void RequestRecordMgr::ClearRecommendRecords()
	{
		m_RecommendRecords.foreach1(RequestRecordBag::DestroyRecord, INVALID_ROLE_ID);
		m_RecommendRecords.UnSafeClear();
	}


	RequestRecordMgr::RequestRecordMgr()
		: m_RecommendRecords(MAX_RECOMMEND_RECORD)
		, m_JoinRecords(MAX_JOIN_RECORD)
	{

	}

	void RequestRecordMgr::ClearDropedRecord(UINT32 dwTargetRoleID)
	{
		m_InviteRecordsBag.ClearDropedRecord(dwTargetRoleID);
	}

	void RequestRecordMgr::DropInviteRecord(EDropReason eReason)
	{
		m_InviteRecordsBag.DropAllRecord(eReason);
	}

	void RequestRecordMgr::GetAllJoinTargetID(std::set<UINT32>& outResult) const
	{
		m_JoinRecords.foreach1r(TeamUtils::CollectRecordID, outResult);
	}

	void RequestRecordMgr::ClearJoinRecords()
	{
		m_JoinRecords.foreach1(RequestRecordBag::DestroyRecord, INVALID_ROLE_ID);
		m_JoinRecords.UnSafeClear();
	}

	void RequestRecordMgr::GetAllIntroducerID(std::set<UINT32>& outResult) const
	{
		m_RecommendRecords.foreach1r(TeamUtils::CollectRecordOperatorID, outResult);
	}
}
