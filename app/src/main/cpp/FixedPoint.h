//
// Created by pgg on 10/08/17.
//

#ifndef APISCORE_FIXEDPOINT_H
#define APISCORE_FIXEDPOINT_H

#include <cstdint>
#include <algorithm>

template <unsigned int P=12>
class UnsignedFixedPoint {
public:
    UnsignedFixedPoint();
    UnsignedFixedPoint(double d);
    double toDouble() const;
    UnsignedFixedPoint const& floor(UnsignedFixedPoint const& precision = UnsignedFixedPoint(1)) const;
    UnsignedFixedPoint const& ceil(UnsignedFixedPoint const& precision = UnsignedFixedPoint(1)) const;
    UnsignedFixedPoint const& round(UnsignedFixedPoint const& precision = UnsignedFixedPoint(1)) const;
    UnsignedFixedPoint const& operator*(UnsignedFixedPoint i) const;
    UnsignedFixedPoint const& operator+(UnsignedFixedPoint i) const;
    UnsignedFixedPoint const& operator-(UnsignedFixedPoint i) const;
    UnsignedFixedPoint const& operator<<(unsigned int i) const;
    UnsignedFixedPoint const& operator>>(unsigned int i) const;
    UnsignedFixedPoint const& operator*(unsigned int i) const;
    UnsignedFixedPoint const& operator/(unsigned int i) const;
    UnsignedFixedPoint const& operator+(unsigned int i) const;
    static unsigned int point() const;
private:
    UnsignedFixedPoint(uint32_t i) : v(i) {}
    uint32_t v;
};

template <unsigned int P>
UnsignedFixedPoint::UnsignedFixedPoint() : v(0)
{

}

template <unsigned int P>
UnsignedFixedPoint::UnsignedFixedPoint(double d) :
        v((1 << P) * std::max(0.0, d))
{

}

template <unsigned int P>
double UnsignedFixedPoint::toDouble() const
{
    return static_cast<double>(v) / (1 << P);
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::floor(const UnsignedFixedPoint &precision) const
{
    uint32_t mod = (v % precision.v);
    return v - mod;
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::round(const UnsignedFixedPoint &precision) const
{
    uint32_t mod = (v % precision.v);
    bool up = mod >= precision.v / 2;
    return  v - mod + up * precision.v;
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::ceil(const UnsignedFixedPoint &precision) const
{
    uint32_t mod = ((v-1) % precision.v) + 1;
    return v + precision.v - mod;
}

template <unsigned int P>
unsigned int UnsignedFixedPoint::point() const
{
    return P;
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::operator*(UnsignedFixedPoint i) const
{
    return static_cast<uint32_t>((static_cast<uint64_t>(this->v) * static_cast<uint64_t>(i.v)) >> P);
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::operator+(UnsignedFixedPoint i) const
{
    return (this->v + i.v);
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::operator-(UnsignedFixedPoint i) const
{
    return (this->v - i.v);
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::operator<<(unsigned int i) const
{
    return this->v << i;
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::operator>>(unsigned int i) const
{
    return this->v >> i;
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::operator*(unsigned int i) const
{
    return this->v * i;
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::operator/(unsigned int i) const
{
    return this->v / i;
}

template <unsigned int P>
UnsignedFixedPoint const& UnsignedFixedPoint::operator+(unsigned int i) const
{
    return this->v + (i << P);
}

#endif //APISCORE_FIXEDPOINT_H
