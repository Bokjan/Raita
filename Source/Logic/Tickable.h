#pragma once

#include <chrono>

class Tickable
{
public:
	virtual void Tick(const std::chrono::nanoseconds tDelta) = 0;
};
