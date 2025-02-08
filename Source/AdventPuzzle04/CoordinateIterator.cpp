#include "CoordinateIterator.h"

CoordinateIterator::CoordinateIterator()
    : myX(0), myY(0), myMethodX(IterationMethod::None), myMethodY(IterationMethod::None)
{
}

CoordinateIterator::CoordinateIterator(int anX, IterationMethod aMethodX, int aY, IterationMethod aMethodY)
    : myX(anX), myMethodX(aMethodX), myY(aY), myMethodY(aMethodY)
{
}

int CoordinateIterator::GetOffsettedX(const int anOffset) const
{
    return OffsetIndex(myX, myMethodX, anOffset);
}

int CoordinateIterator::GetOffsettedY(const int anOffset) const
{
    return OffsetIndex(myY, myMethodY, anOffset);
}

int CoordinateIterator::OffsetIndex(const int aCoord, const IterationMethod aMethod, const int anOffset) const
{
    switch (aMethod)
    {
        case IterationMethod::Add:
            {
                return aCoord + anOffset;
            }
        case IterationMethod::Subtract:
            {
                return aCoord - anOffset;
            }
        default:
            {
                return aCoord;
            }
    }
}
