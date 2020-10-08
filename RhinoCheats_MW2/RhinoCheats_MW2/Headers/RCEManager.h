#pragma once
#include "stdafx.h"

class RCEManager
{
public:
	static void RCE_WriteUInt(int Addr, int Value);
	static void RCE_WriteInt(int Addr, int Value);
	static void RCE_Call(int address);
};