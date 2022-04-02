#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <type_traits>

template <typename Type, int coords_number> class Point
{
public:
    std::array<Type, coords_number> values = {};
    Point() {}
    Point(Type x, Type y) : values { x, y } { static_assert(coords_number == 2); }
    Point(Type x, Type y, Type z) : values { x, y, z } { static_assert(coords_number == 3); }

    Type& x() { return values[0]; }
    Type x() const { return values[0]; }

    Type& y()
    {
        static_assert(coords_number >= 2);
        return values[1];
    }
    Type y() const
    {
        static_assert(coords_number >= 2);
        return values[1];
    }

    Type& z()
    {
        static_assert(coords_number >= 3);
        return values[2];
    }
    Type z() const
    {
        static_assert(coords_number >= 3);
        return values[2];
    }

    Point& operator+=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::plus<Type>());
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       std::minus<Type>());
        return *this;
    }

    Point& operator*=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       std::multiplies<Type>());
        return *this;
    }

    Point& operator*=(const Type scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(), [scalar](Type v) { return v * scalar; });
        return *this;
    }

    Point operator+(const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator*(const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result;
    }

    Point operator*(const Type scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    Point operator-() const
    {
        Point<Type, coords_number> tmp = {};
        std::transform(values.begin(), values.end(), tmp.values.begin(), [](Type v) { return -v; });
        return tmp;
    }

    Type length() const
    {
        return std::sqrt(std::reduce(values.begin(), values.end(), static_cast<Type>(0),
                                     [](Type a, Type b) { return a + (b * b); }));
    }

    Type distance_to(const Point& other) const { return (*this - other).length(); }

    Point& normalize(const Type target_len = 1)
    {
        const Type current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const Type max_len)
    {
        assert(max_len > 0);

        const Type current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }
};

using Point2D = Point<float, 2>;
using Point3D = Point<float, 3>;

// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}

/*
struct Point2D
{
   float values[2] {};

   Point2D() {}
   Point2D(float x, float y) : values { x, y } {}

   float& x() { return values[0]; }
   float x() const { return values[0]; }

   float& y() { return values[1]; }
   float y() const { return values[1]; }

   Point2D& operator+=(const Point2D& other)
   {
       x() += other.x();
       y() += other.y();
       return *this;
   }

   Point2D& operator*=(const Point2D& other)
   {
       x() *= other.x();
       y() *= other.y();
       return *this;
   }

   Point2D& operator*=(const float scalar)
   {
       x() *= scalar;
       y() *= scalar;
       return *this;
   }

   Point2D operator+(const Point2D& other) const
   {
       Point2D result = *this;
       result += other;
       return result;
   }

   Point2D operator*(const Point2D& other) const
   {
       Point2D result = *this;
       result *= other;
       return result;
   }

   Point2D operator*(const float scalar) const
   {
       Point2D result = *this;
       result *= scalar;
       return result;
   }
};

struct Point3D
{
   std::array<float, 3> values = {};

   Point3D() {}
   Point3D(float x, float y, float z) : values { x, y, z } {}

   float& x() { return values[0]; }
   float x() const { return values[0]; }

   float& y() { return values[1]; }
   float y() const { return values[1]; }

   float& z() { return values[2]; }
   float z() const { return values[2]; }

   Point3D& operator+=(const Point3D& other)
   {
       std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                      std::plus<float>());
       return *this;
   }

   Point3D& operator-=(const Point3D& other)
   {
       std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                      std::minus<float>());
       return *this;
   }

   Point3D& operator*=(const float scalar)
   {
       std::transform(values.begin(), values.end(), values.begin(),
                      [scalar](float v) { return v * scalar; });
       return *this;
   }

   Point3D operator+(const Point3D& other) const
   {
       Point3D result = *this;
       result += other;
       return result;
   }

   Point3D operator-(const Point3D& other) const
   {
       Point3D result = *this;
       result -= other;
       return result;
   }

   Point3D operator*(const float scalar) const
   {
       Point3D result = *this;
       result *= scalar;
       return result;
   }

   Point3D operator-() const { return Point3D { -x(), -y(), -z() }; }

   float length() const
   {
       return std::sqrt(
           std::reduce(values.begin(), values.end(), 0.f, [](float a, float b) { return a + (b * b); }));
   }

   float distance_to(const Point3D& other) const { return (*this - other).length(); }

   Point3D& normalize(const float target_len = 1.0f)
   {
       const float current_len = length();
       if (current_len == 0)
       {
           throw std::logic_error("cannot normalize vector of length 0");
       }

       *this *= (target_len / current_len);
       return *this;
   }

   Point3D& cap_length(const float max_len)
   {
       assert(max_len > 0);

       const float current_len = length();
       if (current_len > max_len)
       {
           *this *= (max_len / current_len);
       }

       return *this;
   }
};
*/
