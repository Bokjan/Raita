#pragma once

#include <chrono>

class Tickable
{
public:
	virtual ~Tickable() = default;
	virtual void Tick(const std::chrono::nanoseconds tDelta) = 0;
};
