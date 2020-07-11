#include "stdafx.h"
#include "WritePackets.h"

void AntiAim(usercmd_t* pCmd)
{	
	/*if (Settings[aim_enabled].Value.bValue || Settings[aim_key].Value.iValue)
		return;
	if (!Settings[aim_mode].Value.iValue)
		return;*/


	/*if (
		Settings[aim_enabled].Value.bValue &&
		Settings[aim_mode].Value.iValue
		)*/
	{

		if (cg_entities[cg->clientNum].currentState.eFlags & 8)
			return;

		//compiler doesn't like it, if i put it inside 'case 1'
		static float spinAngle = 0.0f;

		switch (Settings[anti_aim].Value.iValue)
		{
		case 1:		

			if (spinAngle > 360.0f)
				spinAngle -= 360.0f;
			pCmd->viewangles[1] = ANGLE2SHORT(spinAngle);
			spinAngle += 40.0f;
			pCmd->viewangles[0] = ANGLE2SHORT(ANGLE_DOWN);
			//pCmd->buttons |= BUTTON_CROUCHED; //up & down effect
			break;

		case 2:			

			if (cg_entities[cg->clientNum].nextState.weaponID != 4 && cg_entities[cg->clientNum].nextState.weaponID != 3)
			{
				if (Aim.isReady[Aim_t::isReadyforFire])
				{
					pCmd->viewangles[1] += ANGLE2SHORT(Aim.vAimAngles[1] - 180);
					pCmd->viewangles[0] += ANGLE2SHORT(Aim.vAimAngles[0] - 5.7f);
				}
				else
				{
					pCmd->viewangles[1] = ANGLE2SHORT(pViewMatrix->viewAngleX - 180);
					pCmd->viewangles[0] = ANGLE2SHORT(pViewMatrix->viewAngleY - 5.7f);
				}
			}
		}
	}	
}

void InverseTroller(usercmd_t *pCmd)
{
	if (/*Settings[aim_enabled].Value.bValue && 
		Settings[aim_mode].Value.iValue &&*/ 		
		Settings[inverse_troller].Value.bValue) 	
	{
		static float frot = 360.0f;
		if (frot < 0.0f)
			frot += 360.0f;
		pCmd->viewangles[2] = ANGLE2SHORT(frot);
		frot -= 40.0f;			   		
	}
	else
	{
		pCmd->viewangles[2] = 0.0f;		
	}
}

char ClampMove(char value)
{
	if (value < -128)
		return -128;

	if (value > 127)
		return 127;

	return value;
}

void MovementFix(usercmd_t* usercmd, float yaw, float oldyaw, float forward, float right)
{
	float flDelta = DegreesToRadians(yaw - oldyaw);

	usercmd->forwardmove = ClampMove((char)(cosf(flDelta) * forward - sinf(flDelta) * right));
	usercmd->rightmove = ClampMove((char)(sinf(flDelta) * forward + cosf(flDelta) * right));
}

void DoCurCmd(usercmd_t *curCmd, int seed)
{		
	if (Settings[auto_shoot].Value.bValue && Aim.isReady[Aim_t::isReadyforFire])
		curCmd->buttons |= BUTTON_FIRE;

	Nospread.ApplyNoSpread(curCmd, seed);

	if (Settings[silent_aim].Value.bValue && Aim.isReady[Aim_t::isReadyforFire] && !Settings[third_person].Value.bValue)
	{
		//-= doesn't work
		float flOldYaw = SHORT2ANGLE(curCmd->viewangles[1]);

		curCmd->viewangles[1] += ANGLE2SHORT(Aim.vAimAngles[1]);
		curCmd->viewangles[0] += ANGLE2SHORT(Aim.vAimAngles[0]);

		MovementFix(curCmd, SHORT2ANGLE(curCmd->viewangles[1]), flOldYaw, curCmd->forwardmove, curCmd->rightmove);
	}
}

void DoNextCmd(usercmd_t *nextCmd)
{		
	AntiAim(nextCmd);
	InverseTroller(nextCmd);
}



/************************************************************************/
/* WritePacket                                                          */
/************************************************************************/

void WritePacket()
{
	if (cg)
	{
		usercmd_t* curCmd = pinput->GetUserCmd(pinput->currentCmdNum);

		if (Aim.isVehicle &&
			cg_entities[cg->clientNum].valid && (cg_entities[cg->clientNum].IsAlive & 1))
		{
			if (Settings[silent_aim].Value.bValue && Aim.isReady[Aim_t::isReadyforFire] && !Settings[third_person].Value.bValue)
			{
				//-= doesn't work
				float flOldYaw = SHORT2ANGLE(curCmd->viewangles[1]);

				curCmd->viewangles[1] += ANGLE2SHORT(Aim.vAimAngles[1]);
				curCmd->viewangles[0] += ANGLE2SHORT(Aim.vAimAngles[0]);

				MovementFix(curCmd, SHORT2ANGLE(curCmd->viewangles[1]), flOldYaw, curCmd->forwardmove, curCmd->rightmove);
			}

			if (Settings[auto_shoot].Value.bValue && Aim.isReady[Aim_t::isReadyforFire])
				curCmd->buttons |= BUTTON_FIRE;

			Aim.Autoshoot();
		}

		AntiAim(curCmd);
		InverseTroller(curCmd);
	}
}



/************************************************************************/
/* PredictPlayerState                                                   */
/************************************************************************/

void PredictPlayerState()
{
	if (cg)
	{
		static int backupAngles[3];

		usercmd_t* oldCmd = pinput->GetUserCmd(pinput->currentCmdNum - 1);
		usercmd_t* curCmd = pinput->GetUserCmd(pinput->currentCmdNum);
		usercmd_t* newCmd = pinput->GetUserCmd(pinput->currentCmdNum + 1);

		*newCmd = *curCmd;
		++pinput->currentCmdNum;

		VectorCopy(backupAngles, oldCmd->viewangles);
		VectorCopy(curCmd->viewangles, backupAngles);

		++oldCmd->servertime;
		--curCmd->servertime;

		if (!Aim.isVehicle &&
			cg_entities[cg->clientNum].valid && (cg_entities[cg->clientNum].IsAlive & 1))
		{
			if (Settings[silent_aim].Value.bValue && Aim.isReady[Aim_t::isReadyforFire] && !Settings[third_person].Value.bValue)
			{
				//-= doesn't work
				float flOldYaw = SHORT2ANGLE(oldCmd->viewangles[1]);

				oldCmd->viewangles[1] += ANGLE2SHORT(Aim.vAimAngles[1]);
				oldCmd->viewangles[0] += ANGLE2SHORT(Aim.vAimAngles[0]);

				MovementFix(oldCmd, SHORT2ANGLE(oldCmd->viewangles[1]), flOldYaw, oldCmd->forwardmove, oldCmd->rightmove);
			}

			if (Settings[auto_shoot].Value.bValue && Aim.isReady[Aim_t::isReadyforFire])
				oldCmd->buttons |= BUTTON_FIRE;

			Aim.Autoshoot();
		}

		Nospread.ApplyNoSpread(oldCmd, oldCmd->servertime);
	}
}
