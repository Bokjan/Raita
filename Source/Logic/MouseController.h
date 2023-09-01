#pragma once

#include "Tickable.h"

class MouseControllerImpl;

class MouseController : public Tickable
{
public:
	MouseController();
	~MouseController();
	void Tick(const std::chrono::nanoseconds tDelta);
private:
	MouseControllerImpl* pImpl = nullptr;
};
