#pragma once

#define ANGLE_DOWN 70
#define ANGLE_UP -70

void AntiAim(usercmd_t* pCmd);

#define HOOKCALL __cdecl

int HOOKCALL hPredictPlayerState(int a1);
typedef int(HOOKCALL* tPredictPlayerState)(int a1);
extern tPredictPlayerState oPredictPlayerState;

int HOOKCALL hWritePacket(int a1);
typedef int(HOOKCALL* tWritePacket)(int a1);
extern tWritePacket oWritePacket;