#pragma once

namespace Math
{
	class Vector2
	{
	public:
		Vector2()
			: myX(0)
			, myY(0)
		{
		}

		Vector2(int anX, int aY)
			: myX(anX)
			, myY(aY)
		{
		}

		~Vector2() = default;

		Vector2& operator=(const Vector2& aRhs) = default;

	public:
		int myX;
		int myY;
	};

	Vector2 operator+(const Vector2& aLhs, const Vector2& aRhs)
	{
		return Vector2(aLhs.myX + aRhs.myX, aLhs.myY + aRhs.myY);
	}

	Vector2 operator-(const Vector2& aLhs, const Vector2& aRhs)
	{
		return Vector2(aLhs.myX - aRhs.myX, aLhs.myY - aRhs.myY);
	}

	void operator+=(Vector2& aLhs, const Vector2& aRhs)
	{
		aLhs.myX += aRhs.myX;
		aLhs.myY += aRhs.myY;
	}

	void operator-=(Vector2& aLhs, const Vector2& aRhs)
	{
		aLhs.myX -= aRhs.myX;
		aLhs.myY -= aRhs.myY;
	}

	bool operator==(const Vector2& aLhs, const Vector2& aRhs)
	{
		if (aLhs.myX == aRhs.myX &&
			aLhs.myY == aRhs.myY)
		{
			return true;
		}

		return false;
	}
}