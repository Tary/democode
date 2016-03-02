#pragma once
#include "IFStateEvent.h"
#include "ETeamDef.h"

namespace Team
{
	/**
	 * 角色的代理, 发消息,获取角色信息等
	 */
	class IFRoleProxy : IFStateEvent
	{


	public:
		virtual FxRole_RS* GetRole() const = 0;

		virtual UINT32 GetRoleID() const = 0;

		virtual UINT32 GetRoleOBJID() const = 0;
		
		virtual char* GetRoleName() const = 0;

		virtual ERoleState GetRoleState() const = 0;

		virtual TeamerInfo* GetTeamerInfo() const = 0;

		virtual UINT32 GetCamp() const = 0;

		virtual UINT8 GetJob() const = 0;
		virtual UINT8 GetLevel() const = 0;

		virtual bool SendGS(UINT16 wMsgId, const char* pBuff) const = 0;

		virtual bool SendDB(UINT16 wMsgId, const char* pBuff) const = 0;

		virtual bool SendRS(UINT16 wMsgId, const char* pBuff) const = 0;

		virtual bool SendClient(UINT16 wMsgId, const char* pBuff) const = 0;

		virtual void SendMsg(UINT32 dwMsgId, const char* strDyn = NULL) const = 0;
		virtual void SendMsgEx(UINT32 dwMsgId, const char* strDyn = NULL) const = 0;

		virtual void SetSubscribe(ESubscribeIdx eIdx, bool bActive) = 0;
		virtual void ClearSubscribe() = 0;
		virtual UINT32 GetSubscribe() const = 0;

		virtual UINT32 GetAllAuthority() const = 0;
	};
}
