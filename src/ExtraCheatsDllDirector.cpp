////////////////////////////////////////////////////////////////////////////
//
// This file is part of sc3k-extra-cheats, a DLL Plugin for SimCity 3000
// that adds new cheat codes.
//
// Copyright (c) 2024 Nicholas Hayes
//
// This file is licensed under terms of the MIT License.
// See LICENSE.txt for more information.
//
////////////////////////////////////////////////////////////////////////////

#include "StringViewUtil.h"
#include "cGZMessage.h"
#include "cIGZApp.h"
#include "cIGZCheatCodeManager.h"
#include "cIGZCOM.h"
#include "cIGZFrameWork.h"
#include "cIGZMessageServer.h"
#include "cIGZString.h"
#include "cRZAutoRefCount.h"
#include "cRZBaseString.h"
#include "cRZMessageCOMDirector.h"
#include "cISC3App.h"
#include "cISC3BudgetLayer.h"
#include "cISC3City.h"
#include "GZServPtrs.h"

#include <array>
#include <iostream>
#include <memory>
#include <vector>

// This must be unique for every plugin. Generate a 32-bit random number and use it.
// DO NOT REUSE DIRECTOR IDS EVER.
static const uint32_t kExtraCheatsDirectorID = 0x66B93F29;

static const uint32_t kMessageCheatIssued = 0x230E27AC;
static const uint32_t kSC3MessageFundsChanged = 0x229A8A90;
static const uint32_t kSC3MessageSimSpecificDateChanged = 0x43152585;

static const uint32_t DotComCheatID = 0xCD094405;
static const std::string_view DotComCheatStr = "dotcom";
static const uint32_t MoolahCheatID = 0x35F29FB9;
static const std::string_view MoolahCheatStr = "moolah";
static const uint32_t SimDateCheatID = 0x1F92F286;
static const std::string_view SimDateCheatStr = "SimDate";

static const std::array<std::pair<uint32_t, std::string_view>, 3> CheatRegistration =
{
	std::pair(DotComCheatID, DotComCheatStr),
	std::pair(MoolahCheatID, MoolahCheatStr),
	std::pair(SimDateCheatID, SimDateCheatStr),
};

namespace
{
	void PostSC3Message(uint32_t type, uint32_t data1, uint32_t data2, uint32_t data3)
	{
		cIGZMessageServerPtr pMS;

		if (pMS)
		{
			cGZMessage message(type, data1, data2, data3);

			pMS->MessagePost(message, false);
		}
	}
}

class ExtraCheatsDllDirector : public cRZMessageCOMDirector
{
	uint32_t GetDirectorID() const
	{
		return kExtraCheatsDirectorID;
	}

	void ExecuteDotComCheat()
	{
		cIGZApp* const pApp = mpFrameWork->Application();

		cRZAutoRefCount<cISC3App> pSC3App;

		if (pApp->QueryInterface(GZIID_cISC3App, pSC3App.AsPPVoid()))
		{
			cISC3City* const pCity = pSC3App->GetCity();

			if (pCity)
			{
				cISC3BudgetLayer* const pBudgetLayer = pCity->BudgetLayer();

				if (pBudgetLayer)
				{
					pBudgetLayer->DepositFunds(1000000);
				}
			}
		}
	}

	void ExecuteMoolahCheat(const cIGZString* cheatStr)
	{
		if (cheatStr)
		{
			std::vector<std::string_view> arguments;
			arguments.reserve(2);

			StringViewUtil::Split(cheatStr->ToChar(), ' ', arguments);

			if (arguments.size() == 2)
			{
				int64_t totalFunds = 0;

				if (StringViewUtil::TryParse(arguments[1], totalFunds))
				{
					cIGZApp* const pApp = mpFrameWork->Application();

					cRZAutoRefCount<cISC3App> pSC3App;

					if (pApp->QueryInterface(GZIID_cISC3App, pSC3App.AsPPVoid()))
					{
						cISC3City* const pCity = pSC3App->GetCity();

						if (pCity)
						{
							cISC3BudgetLayer* const pBudgetLayer = pCity->BudgetLayer();

							if (pBudgetLayer)
							{
								pBudgetLayer->SetTotalFunds(totalFunds);
								// Broadcast the funds changed message to the other game
								// systems, e.g. the main UI.
								// DepositFunds and WithdrawFunds broadcast this message
								// when they are called, but SetTotalFunds does not.
								//
								// If this is not done the new amount will only be shown
								// in the UI at the start of the next month.
								PostSC3Message(kSC3MessageFundsChanged, 0, 0, 0);
							}
						}
					}
				}
			}
		}
	}

	void ExecuteSimDateCheat(const cIGZString* cheatStr)
	{
		if (cheatStr)
		{
			std::vector<std::string_view> arguments;
			arguments.reserve(4);

			StringViewUtil::Split(cheatStr->ToChar(), ' ', arguments);

			if (arguments.size() == 4)
			{
				uint32_t month = 0;
				uint32_t day = 0;
				uint32_t year = 0;

				if (StringViewUtil::TryParse(arguments[1], month)
					&& StringViewUtil::TryParse(arguments[2], day)
					&& StringViewUtil::TryParse(arguments[3], year))
				{
					cIGZApp* const pApp = mpFrameWork->Application();

					cRZAutoRefCount<cISC3App> pSC3App;

					if (pApp->QueryInterface(GZIID_cISC3App, pSC3App.AsPPVoid()))
					{
						cISC3City* const pCity = pSC3App->GetCity();

						if (pCity)
						{
							pCity->SetDate(month, day, year);

							// Broadcast the specific date changed message to the other game
							// systems, e.g. the main UI.
							//
							// If this is not done the new date will only be shown in the UI
							// at the start of the next month.
							uint32_t newDayOfYear = 0;
							uint32_t newYear = 0;
							pCity->GetDate(newDayOfYear, newYear);

							PostSC3Message(kSC3MessageSimSpecificDateChanged, newYear, newDayOfYear, 0);
						}
					}
				}
			}
		}
	}

	void ProcessCheatCode(const cGZMessage& message)
	{
		uint32_t cheatID = message.dwData1;

		switch (cheatID)
		{
		case DotComCheatID:
			ExecuteDotComCheat();
			break;
		case MoolahCheatID:
			ExecuteMoolahCheat(reinterpret_cast<const cIGZString*>(message.dwData2));
			break;
		case SimDateCheatID:
			ExecuteSimDateCheat(reinterpret_cast<const cIGZString*>(message.dwData2));
			break;
		}
	}

	bool DoMessage(cGZMessage& message)
	{
		switch (message.dwMessageType)
		{
		case kMessageCheatIssued:
			ProcessCheatCode(message);
			break;
		}

		return true;
	}

	bool OnStart(cIGZCOM* pCOM)
	{
		cIGZFrameWork* const pFramework = pCOM->FrameWork();

		pFramework->AddHook(this);

		return true;
	}

	bool PostAppInit()
	{
		cIGZApp* const pApp = mpFrameWork->Application();
		cRZAutoRefCount<cISC3App> pSC3App;

		if (pApp->QueryInterface(GZIID_cISC3App, pSC3App.AsPPVoid()))
		{
			cIGZCheatCodeManager* const pCCM = pSC3App->GetCheatCodeManager();

			if (pCCM)
			{
				for (const auto& pair : CheatRegistration)
				{
					const uint32_t& cheatID = pair.first;
					const std::string_view& cheatName = pair.second;

					pCCM->RegisterCheatCode(cheatID, cRZBaseString(cheatName.data(), cheatName.size()));
					pCCM->AddNotification(this, cheatID);
				}
			}
		}

		return true;
	}
};

cRZCOMDllDirector* RZGetCOMDllDirector()
{
	static ExtraCheatsDllDirector instance;

	return &instance;
}