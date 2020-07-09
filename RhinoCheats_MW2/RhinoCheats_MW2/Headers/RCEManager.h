#pragma once
#include "stdafx.h"

static LPBYTE IpAddress = (LPBYTE)0xAF6028;

class RCEManager
{
public:
	static void RCE_WriteUInt(int Addr, int Value);
	static void RCE_WriteInt(int Addr, int Value);
	static void RCE_Call(int address);
	static void doRCE();
};