#pragma once

class CoordinateIterator
{
  public:
    enum class IterationMethod
    {
        Add,
        Subtract,
        None,
    };

    CoordinateIterator();
    CoordinateIterator(int anX, IterationMethod aMethodX, int aY, IterationMethod aMethodY);

    int GetOffsettedX(const int anOffset) const;
    int GetOffsettedY(const int anOffset) const;

  private:
    int OffsetIndex(const int aCoord, const IterationMethod aMethod, const int anOffset) const;

  private:
    int myX;
    int myY;
    IterationMethod myMethodX;
    IterationMethod myMethodY;
};
