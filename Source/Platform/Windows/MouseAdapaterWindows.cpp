#ifdef _WIN32

#include "Platform/MouseAdapter.h"

#include <Windows.h>

class MouseAdapaterWindows final : public MouseAdapater
{
	Position GetPosition() override
	{
		POINT oPoint;
		GetCursorPos(&oPoint);
		return Position{ oPoint.x, oPoint.y };
	}

	virtual void SetPosition(Position oPosition) override
	{
		SetCursorPos(oPosition.X, oPosition.Y);
	}
};

std::unique_ptr<MouseAdapater> CreateMouseAdapater()
{
	return std::make_unique<MouseAdapaterWindows>();
}

#endif
