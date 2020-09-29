#include "stdafx.h"

Sounds_t sounds;

Sounds_t::Sounds_t()
{
	Start_ = true;

	bFirstBlood = false;
	bDoublekill = false;
	bTriplekill = false;
	bExcellent = false;
	bRampage = false;
	bKillingspree = false;
	bMultikill = false;
	bUltrakill = false;
	bDominating = false;
	bMonsterkill = false;
	bWickedSick = false;
	bUnstoppable = false;
	bGodlike = false;
	bLudicrouskill = false;
	bHolyshit = false;
	bImpressive = false;
	blaught_0 = false;
	blaught_1 = false;
	blaught_2 = false;
	blaught_3 = false;
	blaught_4 = false;

}

void Sounds_t::killsound() {

	if (!Settings[kill_sounds].Value.bValue)
		return;

	if (bGetValuesOnce)
	{
		if (Start_) {
			Hook.playSound(sounds.start);
			Start_ = false;
		}

		KillsHolder = pStats->kills;
		DeathHolder = pStats->deaths;
		HeadShotHolder = pStats->headshots;
		AssistHolder = pStats->assists;
		bGetValuesOnce = false;
	}
	if (cg->isIngame)
	{
		Start_ = true;

		if (pStats->deaths > DeathHolder)
		{
			KillsHolder = pStats->kills;
			DeathHolder = pStats->deaths;
			HeadShotHolder = pStats->headshots;
			AssistHolder = pStats->assists;

			bFirstBlood = false;
			bDoublekill = false;
			bTriplekill = false;
			bExcellent = false;
			bRampage = false;
			bKillingspree = false;
			bMultikill = false;
			bUltrakill = false;
			bDominating = false;
			bMonsterkill = false;
			bWickedSick = false;
			bUnstoppable = false;
			bGodlike = false;
			bLudicrouskill = false;
			bHolyshit = false;
			bImpressive = false;

			blaught_0 = false;
			blaught_1 = false;
			blaught_2 = false;
			blaught_3 = false;
			blaught_4 = false;

		}
		else
		{
			if (pStats->headshots > HeadShotHolder)
			{
				Hook.playSound(sounds.headshot);
				HeadShotHolder = pStats->headshots;
			}

			if (pStats->assists > AssistHolder)
			{
				Hook.playSound(sounds.assist);
				AssistHolder = pStats->assists;
			}

			if (pStats->kills > KillsHolder)
			{
				KillTotal = pStats->kills - KillsHolder;

				if (KillTotal == 1 && !bFirstBlood)
				{
					Hook.playSound(sounds.firstblood);
					bFirstBlood = true;
				}
				else if (KillTotal == 2 && !bDoublekill)
				{
					Hook.playSound(sounds.doublekill);
					bDoublekill = true;
				}
				else if (KillTotal == 3 && !bTriplekill)
				{
					Hook.playSound(sounds.tripplekill);
					bTriplekill = true;
				}
				else if (KillTotal == 4 && !bExcellent)
				{
					Hook.playSound(sounds.excellent);
					bExcellent = true;
				}
				else if (KillTotal == 5 && !bImpressive)
				{
					Hook.playSound(sounds.impressive);
					bImpressive = true;
				}
				else if (KillTotal == 6 && !bKillingspree)
				{
					Hook.playSound(sounds.killingspree);
					bKillingspree = true;
				}
				else if (KillTotal >= 7 && !bMultikill)
				{
					Hook.playSound(sounds.multikill);
					bMultikill = true;
				}
				else if (KillTotal >= 10 && !bUltrakill)
				{
					Hook.playSound(sounds.ultrakill);
					bUltrakill = true;
				}
				else if (KillTotal >= 12 && !bDominating)
				{
					Hook.playSound(sounds.dominating);
					bDominating = true;
				}
				else if (KillTotal >= 15 && !bUnstoppable)
				{
					Hook.playSound(sounds.unstoppable);
					bUnstoppable = true;
				}
				else if (KillTotal >= 20 && !bMonsterkill)
				{
					Hook.playSound(sounds.monsterkill);
					bMonsterkill = true;
				}
				else if (KillTotal >= 23 && !bWickedSick)
				{
					Hook.playSound(sounds.wickedsick);
					bWickedSick = true;
				}
				else if (KillTotal >= 25 && !blaught_0)
				{
					Hook.playSound(sounds.laughtspy);
					blaught_0 = true;
				}
				else if (KillTotal >= 30 && !bGodlike)
				{
					Hook.playSound(sounds.godlike);
					bGodlike = true;
				}
				else if (KillTotal >= 35 && !bLudicrouskill)
				{
					Hook.playSound(sounds.ludicrouskill);
					bLudicrouskill = true;
				}
				else if (KillTotal >= 40 && !bHolyshit)
				{
					Hook.playSound(sounds.holyshit);
					bHolyshit = true;
				}
				else if (KillTotal >= 45 && !bRampage)
				{
					Hook.playSound(sounds.rampage);
					bRampage = true;
				}
				else if (KillTotal >= 50 && !blaught_1)
				{
					Hook.playSound(sounds.laughtscout);
					blaught_1 = true;
				}
				else if (KillTotal >= 60 && !blaught_2)
				{
					Hook.playSound(sounds.laughtdemoman);
					blaught_2 = true;
				}
				else if (KillTotal >= 65 && !blaught_3)
				{
					Hook.playSound(sounds.laughtmedic);
					blaught_3 = true;
				}
				else if (KillTotal >= 70 && !blaught_4)
				{
					Hook.playSound(sounds.laughtsniper);
					blaught_4 = true;
				}

			}
		}
	}
}

void Sounds_t::MapSoundDirectory(const char *soundsFolder)
{
	sprintf_s(sounds.activated, sizeof(sounds.activated), "%sactivated.wav", soundsFolder);
	sprintf_s(sounds.start, sizeof(sounds.start), "%sStarT.wav", soundsFolder);
	sprintf_s(sounds.menu, sizeof(sounds.menu), "%smenu.wav", soundsFolder);
	sprintf_s(sounds.menu_toggle, sizeof(sounds.menu_toggle), "%smenu_toggle.wav", soundsFolder);

	sprintf_s(sounds.headshot, sizeof(sounds.headshot), "%sheadshot.wav", soundsFolder);
	sprintf_s(sounds.firstblood, sizeof(sounds.firstblood), "%sfirstblood.wav", soundsFolder);
	sprintf_s(sounds.doublekill, sizeof(sounds.doublekill), "%sdoublekill.wav", soundsFolder);
	sprintf_s(sounds.tripplekill, sizeof(sounds.tripplekill), "%stripplekill.wav", soundsFolder);
	sprintf_s(sounds.excellent, sizeof(sounds.excellent), "%sexcellent.wav", soundsFolder);
	sprintf_s(sounds.impressive, sizeof(sounds.impressive), "%simpressive.wav", soundsFolder);
	sprintf_s(sounds.killingspree, sizeof(sounds.killingspree), "%skillingspree.wav", soundsFolder);
	sprintf_s(sounds.multikill, sizeof(sounds.multikill), "%smultikill.wav", soundsFolder);
	sprintf_s(sounds.ultrakill, sizeof(sounds.ultrakill), "%sultrakill.wav", soundsFolder);
	sprintf_s(sounds.dominating, sizeof(sounds.dominating), "%sdominating.wav", soundsFolder);
	sprintf_s(sounds.unstoppable, sizeof(sounds.unstoppable), "%sunstoppable.wav", soundsFolder);
	sprintf_s(sounds.monsterkill, sizeof(sounds.monsterkill), "%smonsterkill.wav", soundsFolder);
	sprintf_s(sounds.wickedsick, sizeof(sounds.wickedsick), "%swickedsick.wav", soundsFolder);
	sprintf_s(sounds.godlike, sizeof(sounds.godlike), "%sgodlike.wav", soundsFolder);
	sprintf_s(sounds.ludicrouskill, sizeof(sounds.ludicrouskill), "%sludicrouskill.wav", soundsFolder);
	sprintf_s(sounds.holyshit, sizeof(sounds.holyshit), "%sholyshit.wav", soundsFolder);
	sprintf_s(sounds.rampage, sizeof(sounds.rampage), "%srampage.wav", soundsFolder);
	sprintf_s(sounds.humiliation, sizeof(sounds.humiliation), "%shumiliation.wav", soundsFolder);

	//TF2 laughs :)
	sprintf_s(sounds.laughtspy, sizeof(sounds.laughtspy), "%slaughts\\spy_laughlong01.wav", soundsFolder);
	sprintf_s(sounds.laughtscout, sizeof(sounds.laughtscout), "%slaughts\\scout_laughlong02.wav", soundsFolder);
	sprintf_s(sounds.laughtdemoman, sizeof(sounds.laughtdemoman), "%slaughts\\demoman_laughlong02.wav", soundsFolder);
	sprintf_s(sounds.laughtmedic, sizeof(sounds.laughtmedic), "%slaughts\\medic_laughlong02.wav", soundsFolder);
	sprintf_s(sounds.laughtsniper, sizeof(sounds.laughtsniper), "%slaughts\\sniper_laughlong02.wav", soundsFolder);

	sprintf_s(sounds.assist, sizeof(sounds.assist), "%sassist.wav", soundsFolder);
}


/************************************************************************/
/* KillIcons                                                            */
/************************************************************************/

A17_Sounds KillSpree;

void A17_Sounds::Init()
{
	CurKills = &pStats->kills;// reinterpret_cast<int*>(/*0x1CE1078*/ 0x1CE5078); //Offsets::stats_offset + 0x240; CheatEngine :P
	LastKills = GetKills();
	Spree = 0;
	Timer = TIMER_VALUE;
	CurTextIndex = 0;
}


void A17_MovingText::Setup(int Icon)
{
	Reset();

	switch (Settings[kill_icons_origin].Value.iValue)
	{
	case 0:
		PosY = refdef->height / 2 - Size / 2;
		PosX = refdef->width / 2 - Size / 2;
		break;
	case 1:
		PosY = 37.5;
		PosX = refdef->width - Size / 2;
		break;
	case 2:
		PosY = 37.5;
		PosX = refdef->width / 2 - Size / 2;
		break;
	case 3:
		PosY = 37.5;
		PosX = 37.5;
		break;
	case 4:
		PosY = refdef->height / 2 - Size / 2;
		PosX = 37.5;
		break;
	case 5:
		PosY = refdef->height / 2 - Size / 2;
		PosX = refdef->width - Size / 2;
		break;
	case 6:
		PosY = refdef->height - Size / 2;
		PosX = refdef->width - Size / 2;
		break;
	case 7:
		PosY = refdef->height / 2 - Size / 2;
		PosX = refdef->width - Size / 2;
		break;
	case 8:
		PosY = refdef->height - Size / 2;
		PosX = refdef->width - Size / 2;
	}

	KillIcon = Icon;

	InUse = true;
	OnceFix = true;
}

void A17_MovingText::Update(int CurCount)
{
	if (!InUse)
		return;
	if (OnceFix)
	{
		Counter = CurCount;
		OnceFix = false;
	}

	switch (Settings[kill_icons_size].Value.iValue)
	{
	case 0:
		Size = 50;
		break;
	case 1:
		Size = 75;
		break;
	case 2:
		Size = 100;
	}

	float DestY;
	float DestX;

	switch (Settings[kill_icons_destination].Value.iValue)
	{
	case 0:
		DestY = refdef->height / 2 - 200 + Counter * (Size + 5);
		DestX = 40;
		break;
	case 1:
		DestY = 40;
		DestX = refdef->width / 2 - 200 + Counter * (Size + 5);
		break;
	case 2:
		DestY = refdef->height / 2 - 200 + Counter * (Size + 5);
		DestX = refdef->width - 40 - Size;
		break;
	case 3:
		DestY = refdef->height - 40 - Size;
		DestX = refdef->width / 2 - 200 + Counter * (Size + 5);
	}

	float DeltaX = DestX - PosX;
	float DeltaY = DestY - PosY;

	PosX += DeltaX / 10;
	PosY += DeltaY / 10;

	if (Settings[kill_icons_rotate].Value.bValue)
	{
		if (Rotate > -360)
			Rotate -= 10;
	}


	static float vColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	DrawRotatedPic(GSM(), PosX, PosY, Size, Size, Rotate, vColor, KillIcon);
}

void A17_Sounds::Update()
{
	if (!((cg_entities[cg->clientNum].valid && (cg_entities[cg->clientNum].IsAlive & 1))))
		return;

	if (!Settings[kill_icons].Value.bValue)
		return;

	NumOfSymbols = Settings[kill_icons_count].Value.iValue;

	switch (Settings[kill_icons_type].Value.iValue)
	{
	case 0:	Shader = HeadShotIcon;
		break;

	case 1: Shader = DiedIcon;
		break;

	case 2: mtRandom.randInt(1) ? Shader = HeadShotIcon : Shader = DiedIcon;
		break;
	}
	if (Timer)
		Timer -= 1;
	else
	{
		for (int i = 0; i < TEXTOJBJECT_SIZE; i++)
		{
			TextObjects[i].Reset();
		}
		Spree = 0;
		KillCounter = 0;
		KillCounter2 = 0;
	}
	if (GetKills() != LastKills)
	{
		Timer = TIMER_VALUE;
		if (CurTextIndex >= 0 && CurTextIndex < TEXTOJBJECT_SIZE)
			TextObjects[CurTextIndex].Setup(Shader);
		CurTextIndex += 1;
		if (CurTextIndex == TEXTOJBJECT_SIZE)
			CurTextIndex = 0;

		int DeltaKills = GetKills() - LastKills;
		if (DeltaKills > 0)
		{
			if (Spree < 14)
			{
				if (Spree + DeltaKills < 15)
					Spree += DeltaKills;
				else
					Spree = 14;
			}
			KillCounter2 += DeltaKills;
		}

		LastKills = GetKills();
	}

	if (KillCounter2 > 0)
	{
		if (CurTextIndex >= 0 && CurTextIndex < TEXTOJBJECT_SIZE)
			TextObjects[CurTextIndex].Setup(Shader);
		CurTextIndex += 1;
		if (CurTextIndex == TEXTOJBJECT_SIZE)
			CurTextIndex = 0;
		KillCounter2 -= 1;
		KillCounter += 1;
	}
	if (KillCounter > NumOfSymbols - 1)
		KillCounter = 0;

	for (int i = 0; i < TEXTOJBJECT_SIZE; i++)
	{
		TextObjects[i].Update(KillCounter);
	}
}
