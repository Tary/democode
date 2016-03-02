#pragma once
#include "stdafx.h"
#include "types.h"
#include <string>
#include <vector>
#include <map>
#include "obj/role_rs.h"
#include "macros.h"
#include "types.h"
#include "errorcode.h"

#define INVALID_INDEX size_t(-1) 

#define MAX_JOIN_RECORD size_t(40)
#define MAX_RECOMMEND_RECORD size_t(40)
#define MAX_INVITE_RECORD size_t(80)

namespace Team
{
	enum ELogicFlag
	{
		ELogicFlag_Attorn,
		ELogicFlagCount,
	};
}