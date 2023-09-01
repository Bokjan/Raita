#include "MouseController.h"

#include "RaitaGui.h"
#include "Platform/MouseAdapter.h"

using namespace std::chrono_literals;

struct TimePoint
{
	int iHour = 0;
	int iMinute = 0;
	int iSecond = 0;
};

struct TimePeriod
{
	TimePoint oBegin, oEnd;
};

constexpr TimePeriod g_arrValidPeriods[]
{
	{{9, 0, 0}, {12, 0, 0}},
	{{14, 10, 0}, {18, 30, 0}}
};

class MouseControllerImpl
{
	friend class MouseController;

	MouseControllerImpl()
	{
		pMouseAdapater = CreateMouseAdapater();
	}

	void Tick(const std::chrono::nanoseconds tDelta)
	{
		const auto tNow = std::chrono::high_resolution_clock::now();
		if (!ShouldDetectChange())
		{
			tLastChange = tNow;
			return;
		}
		UpdateRecords(tDelta);
		auto oPos = pMouseAdapater->GetPosition();
		if (tNow - tLastChange > tDetectDuration)
		{
			oPos.X += MoveDirection * MoveStep;
			oPos.Y += MoveDirection * MoveStep;
			MoveDirection = -MoveDirection;
			pMouseAdapater->SetPosition(oPos);
			tLastChange = tNow;
			if (!GuiStatic::g_bIsActivated)
			{
				GuiStatic::g_tEnabledDurationLast = 0ns;
				GuiStatic::g_bIsActivated = true;
			}
		}
		oLastPosition = oPos;
	}

	void UpdateRecords(const std::chrono::nanoseconds tDelta)
	{
		bool bChanged = false;

		// Mouse position
		auto oCurrentPosition = pMouseAdapater->GetPosition();
		if (oCurrentPosition != oLastPosition) { bChanged = true; }
		// Keyboard event
		if (pMouseAdapater->HasKeyboardEvents()) { bChanged = true; }
		
		if (bChanged)
		{
			tLastChange = std::chrono::high_resolution_clock::now();
			GuiStatic::g_bIsActivated = false;
		}
		else 
		{
			if (GuiStatic::g_bIsActivated)
			{
				GuiStatic::g_tEnabledDurationLast += tDelta;
				GuiStatic::g_tEnabledDurationTotal += tDelta;
			}
		}
	}

	bool ShouldDetectChange() const
	{
		if (!GuiStatic::g_bEnableTimePeriodActivation)
		{
			return true;
		}
		const auto tNow = std::chrono::system_clock::now();
		const auto tTime = std::chrono::system_clock::to_time_t(tNow);
		tm stTm;
		(void)localtime_s(&stTm, &tTime);
		auto TimeToInteger = [](int iH, int iM, int iS) -> int
			{
				return iH * 10000 + iM * 100 + iS;
			};
		for (const auto& Pair : g_arrValidPeriods)
		{
			int iNow = TimeToInteger(stTm.tm_hour, stTm.tm_min, stTm.tm_sec);
			int iBegin = TimeToInteger(Pair.oBegin.iHour, Pair.oBegin.iMinute, Pair.oBegin.iSecond);
			int iEnd = TimeToInteger(Pair.oEnd.iHour, Pair.oEnd.iMinute, Pair.oEnd.iSecond);
			if (iNow >= iBegin && iNow <= iEnd)
			{
				return true;
			}
		}
		return false;
	}

	int MoveDirection = 1;
	static constexpr int MoveStep = 20;
	static constexpr std::chrono::seconds tDetectDuration = 10s;
	MouseAdapter::Position oLastPosition;
	std::chrono::steady_clock::time_point tLastChange;
	std::unique_ptr<MouseAdapter> pMouseAdapater = nullptr;
};

MouseController::MouseController()
{
	pImpl = new MouseControllerImpl();
}

MouseController::~MouseController()
{
	if (pImpl == nullptr)
	{
		delete pImpl;
		pImpl = nullptr;
	}
}

void MouseController::Tick(const std::chrono::nanoseconds tDelta)
{
	pImpl->Tick(tDelta);
}
