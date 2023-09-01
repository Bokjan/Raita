#pragma once

#include <chrono>

class RaitaGuiImpl;

class RaitaGui
{
public:
	RaitaGui();
	~RaitaGui();
	void Initialize();
	void Uninitialize();
	void BeforeMainLoop();
	bool FrameTick(const std::chrono::nanoseconds tDelta);
private:
	RaitaGuiImpl* pImpl = nullptr;
};

namespace GuiStatic
{
	extern bool g_bIsActivated;
	extern std::chrono::nanoseconds g_tEnabledDurationLast;
	extern std::chrono::nanoseconds g_tEnabledDurationTotal;
}
