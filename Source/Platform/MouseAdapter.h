#pragma once

#include <memory>

class MouseAdapter
{
public:
	struct Position 
	{ 
		int X = 0, Y = 0; 
		bool operator == (const Position& Rhs) const
		{
			return X == Rhs.X && Y == Rhs.Y;
		}
		bool operator != (const Position& Rhs) const
		{
			return !(*this == Rhs);
		}
	};
	virtual ~MouseAdapter() = default;
	virtual Position GetPosition() = 0;
	virtual void SetPosition(Position oPosition) = 0;
	virtual bool HasKeyboardEvents() = 0;
};

std::unique_ptr<MouseAdapter> CreateMouseAdapater();
