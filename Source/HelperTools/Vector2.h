#pragma once

namespace Math
{
    class Vector2
    {
      public:
        Vector2() : myX(0), myY(0)
        {
        }

        Vector2(int anX, int aY) : myX(anX), myY(aY)
        {
        }

        ~Vector2() = default;

        Vector2& operator=(const Vector2& aRhs) = default;

      public:
        int myX;
        int myY;
    };

    // Vector x Vector Operations
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
        if (aLhs.myX == aRhs.myX && aLhs.myY == aRhs.myY)
        {
            return true;
        }

        return false;
    }

    // Vector x Scalar Operations
    Vector2 operator/(const Vector2& aVector, const int& aScalar)
    {
        return Vector2(aVector.myX / aScalar, aVector.myY / aScalar);
    }

    Vector2 operator*(const Vector2& aVector, const int& aScalar)
    {
        return Vector2(aVector.myX * aScalar, aVector.myY * aScalar);
    }

    Vector2 operator+(const Vector2& aVector, const int& aScalar)
    {
        return Vector2(aVector.myX + aScalar, aVector.myY + aScalar);
    }

    Vector2 operator-(const Vector2& aVector, const int& aScalar)
    {
        return Vector2(aVector.myX - aScalar, aVector.myY - aScalar);
    }

    void operator/=(Vector2& aVector, const int& aScalar)
    {
        aVector.myX /= aScalar;
        aVector.myY /= aScalar;
    }

    void operator*=(Vector2& aVector, const int& aScalar)
    {
        aVector.myX *= aScalar;
        aVector.myY *= aScalar;
    }

    void operator+=(Vector2& aVector, const int& aScalar)
    {
        aVector.myX += aScalar;
        aVector.myY += aScalar;
    }

    void operator-=(Vector2& aVector, const int& aScalar)
    {
        aVector.myX -= aScalar;
        aVector.myY -= aScalar;
    }

} // namespace Math

namespace std
{
    template <> struct hash<Math::Vector2>
    {
        std::size_t operator()(Math::Vector2 const& v) const noexcept
        {
            std::size_t x = std::hash<int>{}(v.myX);
            std::size_t y = std::hash<int>{}(v.myY);

            return x ^ y;
        }
    };
} // namespace std
