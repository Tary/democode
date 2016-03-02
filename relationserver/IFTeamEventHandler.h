#pragma once
#include "ETeamDef.h"
namespace Team
{
	class IFRoleProxy;
	//************************************
	// Class:    IFTeamEventHandler
	// FullName:  Team::IFTeamEventHandler
	// Access:    public
	// 	bool XXXX(...);
	// 	bool HandlerXXXX(..., EErrorCode& eSrcErr);
	//  Handler系列主要是处理检测逻辑处理消息发送均在调用者处理
	// 	return: 操作成败
	// 	eSrcErrb : 别人相关的错误, 需要发给出发调用的角色
	// 			   在事件主要的处理过程XXXX中调用相关角色的HandlerXXXX处理和该角色相关的操作,
	// 			   关于调用者的错误通过eSrcErr返回
	// 		   Exampl :
	// 	bool XXX(...)
	// 	{
	// 		EErrorCode eSelfErr = TeamEC_OK;
	// 		do
	// 		{
	// 			...其他判定
	// 
	// 			if (!HandlerXXXX(..., eSelfErr))
	// 			{
	// 				break;
	// 			}
	// 
	// 			...
	// 		} while (0);
	// 		//send eSelfErr
	// 	}
	//************************************
	class IFTeamEventHandler
	{
	public:
		/**队长分配*/
		virtual bool AssignItemREQ(UINT32 dwDstRoleID, UINT32 dwItemID) = 0;
		/**修改关注小队(拖出来的小队)*/
		virtual bool ModifySubscribe(ESubTeamID eSubIdx, bool bActive) = 0;
		/**修改队伍配置 如禁止申请 禁止推荐*/
		virtual bool ModifyConfigs(EConfigFlagType eFlagIdx, bool bActive) = 0;
		/**人员安排, 小队间移动 */
		virtual bool Arrange(UINT32 dwRoleID, ESubTeamID eSubIdx, UINT8 posInSub) = 0;

		/**处理被更换小队*/
		virtual bool HandlerArrange(TeamerInfo* pExecutor, EErrorCode& eSrcErr) = 0;

		/**转让队长/团长*/
		virtual bool AttornREQ(UINT32 dwCandidateRoleID) = 0;

		/**处理转让队长/团长(被转让角度)*/
		virtual bool HandlerAttornREQ(TeamerInfo* pExecutor, EErrorCode& eSrcErr) = 0;

		/**改变分配*/
		virtual bool ChangeAssignSchemaREQ(ETeamAssignMethod type, EItemQuality level) = 0;

		/**修改团队/队伍描述*/
		virtual bool ChangeDescREQ(const char* pDesc) = 0;

		/**创建队伍*/
		virtual bool CreateTeamREQ() = 0;

		/**发起推荐 */
		virtual bool RecommendREQ(UINT32 dwTargetRoleId) = 0;

		/**发起推荐 */
		virtual bool RecommendREQByName(const std::string& strTargetName) = 0;

		/**被推荐 */
		virtual bool HandlerRecommendREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pInviter, EErrorCode& eSrcErr) = 0;

		/**队长客户端处理了推荐 */
		virtual bool RecommendRES(UINT32 dwTargetRoleID, const char* pName, ETeamVerify eAgree) = 0;

		/**发起邀请 */
		virtual bool InviteREQ(UINT32 dwRoleID) = 0;

		/**发起邀请 */
		virtual bool InviteREQByName(const std::string& strName) = 0;

		/**被邀请者角度处理被邀请, 组织信息用来检查是否有效 */
		virtual bool HandlerInviteREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pInviter, UINT32 dwOrgID, EOrganizationType eOrgType, EErrorCode& eSrcErr) = 0;

		/**处理邀请的回复(被邀请者角度), 组织信息用来检查是否有效*/
		virtual bool InviteRES(UINT32 dwInviterRoleId, ETeamVerify eAgree, UINT32 dwOrgID, EOrganizationType eOrgType) = 0;

		/**处理邀请回复(邀请者角度), 组织信息用来检查是否有效队伍信息用来检查是否有效 */
		virtual bool HandlerInviteRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pInvited, UINT32 dwOrgID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr) = 0;

		/**加入申请	*/
		virtual bool JoinREQ(UINT32 dwRoleID) = 0;

		/**处理加入申请(被申请者角度)*/
		virtual bool HandlerJoinREQ(IFRoleProxy* pMsgReceiver, TeamerInfo* pJoiner, EErrorCode& eSrcErr) = 0;

		/**加入申请回复(被申请者角度), 组织信息用来检查是否有效 */
		virtual bool JoinRES(UINT32 dwRoleID, ETeamVerify eAgree, UINT32 dwOrgID, EOrganizationType eOrgType) = 0;

		/**处理申请回复(申请者角度), 组织信息用来检查是否有效*/
		virtual bool HandlerJoinRES(IFRoleProxy* pMsgReceiver, TeamerInfo* pTarget, UINT32 dwOrgID, EOrganizationType eOrgType, ETeamVerify eAgree, EErrorCode& eSrcErr) = 0;

		/**剔出队伍*/
		virtual bool KickOut(UINT32 dwRoleID) = 0;

		/**踢出队伍(被踢出角度处理)*/
		virtual bool HandlerKickOut(TeamerInfo* pExecutor, EErrorCode& eSrcErr) = 0;

		/**离开队伍 */
		virtual bool LeaveREQ() = 0;

		virtual bool TurnToGroupREQ() = 0;		
	};
}