#pragma once
#include "ETeamDef.h"
class FxRole_RS;
namespace Team
{

	/**
	 * 处理成功完成的事件,主要是通知或改变状态
	 */
	class IFStateEvent
	{



	public:
		/**
		 * 转为团队
		 */
		virtual void OnTurnedToGroup(FxRole_RS* pTeamLeader, FxRole_RS* pGroupLeader) = 0;

		/**
		 * 离开团队/队伍
		 */
		virtual void OnLeaveOrgnization() = 0;

		/**
		 * 成功申请进队伍
		 */
		virtual void OnJoinTeam(FxRole_RS* pGroupLeader, UINT32 dwOrgID) = 0;

		/**
		 * 成功申请进团队
		 */
		virtual void OnJoinGroup(FxRole_RS* pGroupLeader, UINT32 dwOrgID) = 0;

		/**
		 * 通过被邀请加入了队伍
		 */
		virtual void OnInvitedInTeam(FxRole_RS* pGroupLeader, UINT32 dwOrgID) = 0;

		/**
		 * 通过被邀请加入了团队
		 */
		virtual void OnInvitedInGroup(FxRole_RS* pGroupLeader, UINT32 dwOrgID) = 0;
	};
}
