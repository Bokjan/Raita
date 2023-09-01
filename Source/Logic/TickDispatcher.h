#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include "Tickable.h"

class TickDispatcher
{
public:
	TickDispatcher();
	void Tick(const std::chrono::nanoseconds tDelta);
private:
	std::vector<std::unique_ptr<Tickable>> Objects;
};
