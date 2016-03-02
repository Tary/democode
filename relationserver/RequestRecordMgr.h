#pragma once
#include "RequestRecord.h"
#include "InviteRequestRecordBag.h"
namespace Team
{	
	class RequestRecordMgr
	{
	public:
		RequestRecordMgr();

		EErrorCode CanInvite(UINT32 dwTargetRoleID) const;
		RequestRecord* GetInviteRecord(UINT32 dwTargetRoleID) const;
		RequestRecord* AddInviteRecord(UINT32 dwTarget, UINT32 dwOperator);
		size_t DelInviteRecord(UINT32 dwTargetRoleID);
		void DropInviteRecord(EDropReason eReason);

		EErrorCode CanJoin(UINT32 dwTargetRoleID) const;
		RequestRecord* GetJoinRecord(UINT32 dwTargetRoleID) const;
		RequestRecord* AddJoinRecord(UINT32 dwTargetRoleID, UINT32 dwOperator);
		void GetAllJoinTargetID(std::set<UINT32>& outResult) const;
		size_t DelJoinRecord(UINT32 dwTargetRoleID, UINT32 dwOperatorID = INVALID_ROLE_ID);
		void ClearJoinRecords();

		EErrorCode CanRecommend(UINT32 dwTargetRoleID) const;
		RequestRecord* GetRecommendRecord(UINT32 dwTargetRoleID) const;
		RequestRecord* AddRecommendRecord(UINT32 dwTargetRoleID, UINT32 dwOperator);
		size_t DelRecommendRecord(UINT32 dwTargetRoleID, UINT32 dwOperatorID = INVALID_ROLE_ID);
		void ClearRecommendRecords();
		void GetAllIntroducerID(std::set<UINT32>& outResult) const;


		void ClearDropedRecord(UINT32 dwTargetRoleID);

	protected:
		RequestRecordBag m_JoinRecords;
		InviteRequestRecordBag m_InviteRecordsBag;
		RequestRecordBag m_RecommendRecords;
	};


	
}


