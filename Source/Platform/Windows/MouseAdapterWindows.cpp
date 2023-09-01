#ifdef _WIN32

#include "Platform/MouseAdapter.h"

#include <Windows.h>

static LRESULT KeyboardEventCallback(int code, WPARAM wParam, LPARAM lParam);

class MouseAdapterWindows final : public MouseAdapter
{
public:
	MouseAdapterWindows()
	{
		constexpr int kKeyboardHook = 13;
		hKeyboardHook = SetWindowsHookExA(kKeyboardHook, KeyboardEventCallback, 0, 0);
	}

	~MouseAdapterWindows()
	{
		if (hKeyboardHook != nullptr)
		{
			UnhookWindowsHookEx(hKeyboardHook);
		}
	}

	Position GetPosition() override
	{
		POINT oPoint;
		GetCursorPos(&oPoint);
		return Position{ oPoint.x, oPoint.y };
	}

	void SetPosition(Position oPosition) override
	{
		SetCursorPos(oPosition.X, oPosition.Y);
	}

	bool HasKeyboardEvents() override
	{
		bool bRet = bHasKeyboardEvents;
		bHasKeyboardEvents = false;
		return bRet;
	}

	static void SetKeyboardEvents()
	{
		bHasKeyboardEvents = true;
	}

private:
	static bool bHasKeyboardEvents;
	HHOOK hKeyboardHook = nullptr;
};

bool MouseAdapterWindows::bHasKeyboardEvents = false;

static LRESULT KeyboardEventCallback(int code, WPARAM wParam, LPARAM lParam)
{
	MouseAdapterWindows::SetKeyboardEvents();
	return CallNextHookEx(0, code, wParam, lParam);
}

std::unique_ptr<MouseAdapter> CreateMouseAdapater()
{
	return std::make_unique<MouseAdapterWindows>();
}

#endif
