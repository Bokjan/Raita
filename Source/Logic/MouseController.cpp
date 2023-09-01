#include "MouseController.h"

#include "RaitaGui.h"
#include "Platform/MouseAdapter.h"

using namespace std::chrono_literals;

class MouseControllerImpl
{
	friend class MouseController;

	MouseControllerImpl()
	{
		pMouseAdapater = CreateMouseAdapater();
	}

	void Tick(const std::chrono::nanoseconds tDelta)
	{
		UpdateRecords(tDelta);
		const auto tNow = std::chrono::high_resolution_clock::now();
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
		auto oCurrentPosition = pMouseAdapater->GetPosition();
		if (oCurrentPosition != oLastPosition)
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

	int MoveDirection = 1;
	static constexpr int MoveStep = 20;
	static constexpr std::chrono::seconds tDetectDuration = 5s;
	MouseAdapater::Position oLastPosition;
	std::chrono::steady_clock::time_point tLastChange;
	std::unique_ptr<MouseAdapater> pMouseAdapater = nullptr;
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
