#pragma once

#include <math.h>

namespace AvocadoMath
{
    class Vector
    {
    private:
        double x, y, z;

    public:
        Vector(double _x, double _y, double _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }
        Vector(double _x, double _y)
        {
            x = _x;
            y = _y;
            z = 0;
        }

        Vector operator+(const Vector ref)
        {
            Vector vec(x + ref.x, y + ref.y, z + ref.z);
            return vec;
        }
        Vector operator-(const Vector ref)
        {
            Vector vec(x - ref.x, y - ref.y, z - ref.z);
            return vec;
        }
        Vector operator*(const Vector ref)
        {
            Vector vec(x * ref.x, y * ref.y, z * ref.z);
            return vec;
        }
        Vector operator/(const Vector ref)
        {
            Vector vec(x / ref.x, y / ref.y, z / ref.z);
            return vec;
        }
        Vector operator+=(const Vector ref)
        {
            Vector vec(x += ref.x, y += ref.y, z += ref.z);
            return vec;
        }
        Vector operator-=(const Vector ref)
        {
            Vector vec(x -= ref.x, y -= ref.y, z -= ref.z);
            return vec;
        }
        Vector operator*=(const Vector ref)
        {
            Vector vec(x *= ref.x, y *= ref.y, z *= ref.z);
            return vec;
        }
        Vector operator/=(const Vector ref)
        {
            Vector vec(x /= ref.x, y /= ref.y, z /= ref.z);
            return vec;
        }

        void SetX(double _x)
        {
            x = _x;
        }
        void SetY(double _y)
        {
            y = _y;
        }
        void SetZ(double _z)
        {
            z = _z;
        }
        void SetTransform(double _x, double _y)
        {
            x = _x;
            y = _y;
        }
        void SetTransform(double _x, double _y, double _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }
        void Translate(double _xv, double _yv)
        {
            x += _xv;
            y += _yv;
        }
        void Translate(double _xv, double _yv, double _zv)
        {
            x += _xv;
            y += _yv;
            z += _zv;
        }
        double const GetX()
        {
            return x;
        }
        double const GetY()
        {
            return y;
        }
        double const GetZ()
        {
            return z;
        }
    };

    static float GetDistance(Vector *position, Vector *position2)
    {
        return (float)abs(sqrt(((position->GetX() - position2->GetX()) * (position->GetX() - position2->GetX()) + (position->GetY() - position2->GetY()) * (position->GetY() - position2->GetY()))));
    }

    static float GetAngle(Vector *position, Vector *position2)
    {
        return (float)atan2(position2->GetY() - position->GetY(), position2->GetX() - position->GetX());
    }

    static float GetXv(float moveSpeed, Vector *position, Vector *position2)
    {
        return (float)cos(GetAngle(position, position2)) * moveSpeed;
    }

    static float GetYv(float moveSpeed, Vector *position, Vector *position2)
    {
        return (float)sin(GetAngle(position, position2)) * moveSpeed;
    }

}
