#include "TickDispatcher.h"

#include "MouseController.h"

TickDispatcher::TickDispatcher()
{
	Objects.emplace_back(std::make_unique<MouseController>());
}

void TickDispatcher::Tick(const std::chrono::nanoseconds tDelta)
{
	for (auto& Object : Objects)
	{
		Object->Tick(tDelta);
	}
}
